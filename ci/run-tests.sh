#!/usr/bin/env bash

SCRIPT_DIR="$(dirname "${BASH_SOURCE[0]}")"

. "$SCRIPT_DIR/functions.sh" || exit 1

# Associative array for most options
declare -A options

# Defines we collect separately in an array
defines=()

while [[ $# > 0 ]]
do
  key="$1"

  case $key in
    --test-configuration)
      [[ -r $2 ]] ||
        exitWithError "Error: test configuration '%s' non-existing or not readable.\n" "$2"
      options[${key:2}]="$2"
      shift
      ;;
    --build-dir)
      [[ -d $2 ]] ||
        exitWithError "Error: argument '%s' to %s is not a directory\n" "$2" "$key"

      options[${key:2}]="$2"
      shift
      ;;
    --platform)
      # TODO fix OSX vs. MacOS (everywhere)
      platformRe="(Windows|WindowsUwp|Linux|MacOS|OSX|Android)-(arm32|arm64|x86|x64)-(Debug|Release)"
      [[ $2 =~ ^$platformRe$ ]] ||
        exitWithError "Error: invalid platform specification '%s', expected %s.\n" "$2" "$platformRe"
      options[${key:2}]="$2"
      os="${BASH_REMATCH[1]}"
      arch="${BASH_REMATCH[2]}"
      flavor="${BASH_REMATCH[3]}"
      bitness="$([[ $arch == x86 || $arch == arm32 ]] && echo 32 || echo 64)"
      shift
      ;;
    --define|-D)
      [[ $2 =~ [^=]*=.* ]] ||
        exitWithError "Error: expected key=value arguments for %s option, got '%s'\n" "$key" "$2"
      defines+=(--define "$2")
      shift
      ;;
    --verbose|-v)
      options[verbose]=1
      ;;
    --)
      # Stop processing options, remaining arguments are test files.
      shift
      break
      ;;
    *)
      exitWithError "Error: unrecognized option '%s'\n" "$key"
      ;;
  esac
  shift # past argument or value
done

verbose_switch=
[[ -z ${options[verbose]} ]] ||
  verbose_switch=--verbose

if [[ $# == 0 ]]; then
  # Note: exclude scripts starting with an underscore
  tests=("$SCRIPT_DIR/t/"[^_]*.sh)
else
  for t in "$@"; do
    t="$SCRIPT_DIR/t/$t.sh"
    [[ -x $t ]] || {
      echo Error: cannot find test file $t.
      exit 1
    }
    tests+=("$t")
  done
fi

# Check for required options
for k in test-configuration build-dir platform; do
  [[ -n ${options[$k]} ]] || {
    echo Error: missing --$k option.
    exit 1
  }
done

# Binary directory, with flavor appended for multi-config generators.
binaryDir="${options[build-dir]}/bin"
[[ $os != Windows* ]] || binaryDir+=/$flavor

VARS="$(perl "$SCRIPT_DIR/"evaluate-test-config.pl $verbose_switch --format bash-environment --input "${options[test-configuration]}" "${defines[@]}")" || {
  echo Error: could not evaluate test config. 1>&2
  exit 1
}

eval -- "$VARS"

if [[ $(uname) = Darwin ]]; then
  coreutilsPrefix=g
else
  coreutilsPrefix=
fi

pass=0
total=0
for testfile in "${tests[@]}"; do
  T="$(basename "$testfile" .sh)"
  echo Starting $T with timeout 1200 s
  ${coreutilsPrefix}timeout -k 5s 1200 ${coreutilsPrefix}stdbuf -o0 -e0 \
  "$testfile" "${options[build-dir]}" "${options[platform]}" "$binaryDir"
  exitCode=$?
  if [[ $exitCode == 0 ]]; then
    ((pass++))
    echo Test $T: passed
  else
    vsts_logissue error "${options[platform]}: Test $T failed, exit code $exitCode, source $testfile."
    echo Test $T: failed with error code $exitCode
  fi
  ((total++))
done
echo Pass '(including skip)' $pass / $total.
((pass == total))

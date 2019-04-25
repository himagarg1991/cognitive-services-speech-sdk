//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
// uspcommon.h: common definitions and declaration used by USP internal implementation
//

#pragma once

#include "usperror.h"

#define UNUSED(x) (void)(x)

#define USP_RETURN_NOT_IMPLEMENTED() \
    do { \
        LogError("Not implemented"); \
        return USP_NOT_IMPLEMENTED; \
    } while (0)

#define USE_BUFFER_SIZE    ((size_t)-1)

#if defined _MSC_VER
#define PROTOCOL_VIOLATION(__fmt, ...)  LogError("ProtocolViolation:" __fmt, __VA_ARGS__)
#else
#define PROTOCOL_VIOLATION(__fmt, ...)  LogError("ProtocolViolation:" __fmt, ##__VA_ARGS__)
#endif

typedef struct ProxyServerInfo
{
    std::string host;
    int port;
    std::string username;
    std::string password;
} ProxyServerInfo;

#include <string>

namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace USP {

    namespace endpoint
    {
        const std::string protocol = "wss://";

        const std::string outputFormatQueryParam = "format=";
        const std::string langQueryParam = "language=";
        const std::string deploymentIdQueryParam = "cid=";

        const std::string outputFormatSimple = "simple";
        const std::string outputFormatDetailed = "detailed";

        namespace unifiedspeech
        {
            const std::string hostnameSuffix = ".stt.speech.microsoft.com";
            const std::string pathPrefix = "/speech/recognition/";
            const std::string pathSuffix = "/cognitiveservices/v1";

            const std::vector<std::string> queryParameters = { langQueryParam, deploymentIdQueryParam, outputFormatQueryParam };
        }

        namespace translation
        {
            const std::string hostnameSuffix = ".s2s.speech.microsoft.com";
            const std::string path = "/speech/translation/cognitiveservices/v1";
            const std::string from = "from=";
            const std::string to = "to=";
            const std::string voice = "voice=";
            const std::string features = "features=";
            const std::string requireVoice = "texttospeech";

            const std::vector<std::string> queryParameters = { from, to, voice, deploymentIdQueryParam, outputFormatQueryParam };
        }

        namespace luis
        {
            const std::string hostname = "speech.platform.bing.com";
            const std::string pathPrefix1 = "/speech/";
            const std::string pathPrefix2 = "/recognition/";
            const std::string pathSuffix = "/cognitiveservices/v1";

            const std::vector<std::string> queryParameters = { langQueryParam, outputFormatQueryParam };
        }

        namespace CDSDK
        {
            const std::string url = "speech.platform.bing.com/cortana/api/v1?environment=Home&";
        }

        namespace bot
        {
            const std::string url = "speech.platform.bing.com/convai/api/v2";

            const std::vector<std::string> queryParameters = { langQueryParam };
        }
    }

    namespace path {
        const std::string speechHypothesis = "speech.hypothesis";
        const std::string speechPhrase = "speech.phrase";
        const std::string speechFragment = "speech.fragment";
        const std::string turnStart = "turn.start";
        const std::string turnEnd = "turn.end";
        const std::string speechStartDetected = "speech.startDetected";
        const std::string speechEndDetected = "speech.endDetected";
        const std::string translationHypothesis = "translation.hypothesis";
        const std::string translationPhrase = "translation.phrase";
        const std::string translationSynthesis = "translation.synthesis";
        const std::string translationSynthesisEnd = "translation.synthesis.end";
        const std::string audio = "audio";
    }

    namespace headers {
        constexpr auto userAgent = "User-Agent";
        constexpr auto ocpApimSubscriptionKey = "Ocp-Apim-Subscription-Key";
        constexpr auto authorization = "Authorization";
        constexpr auto searchDelegationRPSToken = "X-Search-DelegationRPSToken";
        constexpr auto audioResponseFormat = "X-Output-AudioCodec";
        constexpr auto contentType = "Content-Type";
        constexpr auto streamId = "X-StreamId";
        constexpr auto requestId = "X-RequestId";
        constexpr auto region = "Ocp-Apim-Subscription-Region";
    }

    namespace json_properties {
        const std::string offset = "Offset";
        const std::string duration = "Duration";
        const std::string text = "Text";
        const std::string recoStatus = "RecognitionStatus";
        const std::string displayText = "DisplayText";
        const std::string context = "context";
        const std::string tag = "serviceTag";

        const std::string nbest = "NBest";
        const std::string confidence = "Confidence";
        const std::string display = "Display";

        const std::string translation = "Translation";
        const std::string translationStatus = "TranslationStatus";
        const std::string translationFailureReason = "FailureReason";
        const std::string translations = "Translations";
        const std::string synthesisStatus = "SynthesisStatus";
        const std::string lang = "Language";
    }

}
}
}
}


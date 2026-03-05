#ifndef __CC_PLIST_H__
#define __CC_PLIST_H__

#include <string>
#include <string_view>

#include "base/CCValue.h"
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

class CC_DLL PList
{
public:
    enum class ErrorCode
    {
        SUCCESS,
        PARSE_ERROR,            // tinyxml2 failed to parse the document
        MISSING_ROOT,           // <plist> exists but contains no child
        MALFORMED_DICT,         // dictionary has an unexpected structure
        UNSUPPORTED_TYPE,       // element type is not recognised by decoder
        PARSE_INTEGER_ERROR,    // failed to parse integer value
        PARSE_REAL_ERROR,       // failed to parse real value
    };

    struct Option
    {
        size_t _sizeHint = 256; // hint for how large the resulting, you can avoid potential reallocations by providing a good hint
    };

    static std::string encode(const Value& value, const Option& option = {});

    static ErrorCode decode(std::string_view content, Value& value);
};

NS_CC_END

#endif // __CC_PLIST_H__

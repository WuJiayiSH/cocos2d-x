#ifndef __CC_JSON_H__
#define __CC_JSON_H__

#include <string>
#include <string_view>

#include "base/CCValue.h"
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

class CC_DLL JSON
{
public:
    enum class ErrorCode
    {
        SUCCESS,
        EMPTY_INPUT,
        PARSE_ERROR,
        INVALID_ROOT,
        MULTIPLE_ROOT,
        MALFORMED_STRUCTURE,
    };

    struct Option
    {
        size_t _sizeHint = 256; // hint for how large the resulting, you can avoid potential reallocations by providing a good hint
        bool _prettyPrint = false; // whether to format the JSON with indentation and newlines
    };

    static std::string encode(const Value& value, const Option& option);
    static std::string encode(const Value& value) { return encode(value, Option{}); }

    static ErrorCode decode(std::string_view content, Value& value);
};

NS_CC_END

#endif // __CC_JSON_H__

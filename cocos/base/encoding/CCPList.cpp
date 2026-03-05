#include "CCPList.h"

#include <charconv>

#include "base/ccMacros.h"
#include "rapidxml/rapidxml_sax3.hpp"

NS_CC_BEGIN

namespace
{
    static void appendEscapedText(std::string_view text, std::string& out)
    {
        for (char ch : text)
        {
            switch (ch)
            {
            case '&':
                out.append("&amp;");
                break;
            case '<':
                out.append("&lt;");
                break;
            case '>':
                out.append("&gt;");
                break;
            case '\"':
                out.append("&quot;");
                break;
            case '\'':
                out.append("&apos;");
                break;
            default:
                out.push_back(ch);
                break;
            }
        }
    }

    static void appendTagWithText(const char* tag, std::string_view text, std::string& out, int indentLevel)
    {
        for (int i = 0; i < indentLevel; ++i)
            out.append("    ");

        out.push_back('<');
        out.append(tag);
        out.push_back('>');
        appendEscapedText(text, out);
        out.append("</");
        out.append(tag);
        out.append(">\n");
    }

    static void appendLine(std::string_view text, std::string& out, int indentLevel)
    {
        for (int i = 0; i < indentLevel; ++i)
            out.append("    ");

        out.append(text);
        out.push_back('\n');
    }

    static bool encodeElement(const Value& value, std::string& out, int indentLevel);

    static bool encodeDict(const ValueMap& dict, std::string& out, int indentLevel)
    {
        appendLine("<dict>", out, indentLevel);
        for (const auto &[key, value] : dict)
        {
            appendTagWithText("key", key, out, indentLevel + 1);
            encodeElement(value, out, indentLevel + 1);
        }
        appendLine("</dict>", out, indentLevel);
        return true;
    }

    static bool encodeArray(const ValueVector& array, std::string& out, int indentLevel)
    {
        appendLine("<array>", out, indentLevel);
        for (const Value& item : array)
        {
            encodeElement(item, out, indentLevel + 1);
        }
        appendLine("</array>", out, indentLevel);
        return true;
    }

    static bool encodeElement(const Value& value, std::string& out, int indentLevel)
    {
        switch (value.getType())
        {
        case Value::Type::STRING:
            appendTagWithText("string", value.asString(), out, indentLevel);
            return true;
        case Value::Type::INTEGER:
        case Value::Type::UNSIGNED:
            appendTagWithText("integer", value.asString(), out, indentLevel);
            return true;
        case Value::Type::BYTE:
            appendTagWithText("integer", std::to_string(static_cast<int>(value.asByte())), out, indentLevel);
            return true;
        case Value::Type::FLOAT:
        case Value::Type::DOUBLE:
            appendTagWithText("real", value.asString(), out, indentLevel);
            return true;
        case Value::Type::BOOLEAN:
            if (value.asBool())
                appendTagWithText("true", "", out, indentLevel);
            else
                appendTagWithText("false", "", out, indentLevel);
            return true;
        case Value::Type::VECTOR:
            return encodeArray(value.asValueVector(), out, indentLevel);
        case Value::Type::MAP:
            return encodeDict(value.asValueMap(), out, indentLevel);
        default:
            CCLOGERROR("CCPList: unsupported value type %d while encoding", static_cast<int>(value.getType()));
            return false;
        }
    }

    // SAX handler that builds the Value hierarchy as events are received.
    struct SaxHandler : public rapidxml::xml_sax2_handler
    {
        PList::ErrorCode _error = PList::ErrorCode::SUCCESS;
        std::vector<Value> _stack;
        std::vector<std::string> _keyStack;
        std::string _currentText;
        bool _capturing = false;

        void xmlSAX2StartElement(const char* name, size_t len, const char** /*atts*/, size_t /*attslen*/) override
        {
            std::string_view element(name, len);
            if (element == "plist" || element == "true" || element == "false")
            {
                // these elements are valid but don't require any special handling at the start tag
            }
            else if (element == "dict")
            {
                _stack.emplace_back(ValueMap());
            }
            else if (element == "array")
            {
                _stack.emplace_back(ValueVector());
            }
            else if (element == "string" || element == "integer" || element == "real" || element == "key")
            {
                _currentText.clear();
                _capturing = true;
            }
            else
            {
                // unknown element type
                CCLOGERROR("CCPList: unsupported element <%s> while decoding", std::string(element).c_str());
                _error = PList::ErrorCode::UNSUPPORTED_TYPE;
            }
        }

        void xmlSAX2EndElement(const char* name, size_t len) override
        {
            std::string_view element(name, len);
            if (element == "plist")
            {
            }
            else if (element == "dict" || element == "array")
            {
                Value finished = std::move(_stack.back());
                _stack.pop_back();
                addValueToParent(std::move(finished));
            }
            else if (element == "key")
            {
                _keyStack.push_back(std::move(_currentText));
                _capturing = false;
                _currentText.clear();
            }
            else if (element == "string")
            {
                addValueToParent(Value(_currentText));
                _capturing = false;
                _currentText.clear();
            }
            else if (element == "integer")
            {
                long long ll = 0;
                std::from_chars_result result = std::from_chars(_currentText.data(), _currentText.data() + _currentText.size(), ll);
                if (result.ec != std::errc())
                {
                    CCLOGERROR("CCPList: failed to parse integer value '%s'", _currentText.c_str());
                    _error = PList::ErrorCode::PARSE_INTEGER_ERROR;
                }
                else if (ll >= 0)
                {
                    addValueToParent(Value(static_cast<unsigned int>(ll)));
                }
                else
                {
                    addValueToParent(Value(static_cast<int>(ll)));
                }

                _capturing = false;
                _currentText.clear();
            }
            else if (element == "real")
            {
                double d = 0;
                std::from_chars_result result = std::from_chars(_currentText.data(), _currentText.data() + _currentText.size(), d);
                if (result.ec != std::errc())
                {
                    CCLOGERROR("CCPList: failed to parse real value '%s'", _currentText.c_str());
                    _error = PList::ErrorCode::PARSE_REAL_ERROR;
                }
                else
                {
                    addValueToParent(Value(d));
                }

                _capturing = false;
                _currentText.clear();
            }
            else if (element == "true")
            {
                addValueToParent(Value(true));
            }
            else if (element == "false")
            {
                addValueToParent(Value(false));
            }
        }

        void xmlSAX2Text(const char* s, size_t len) override
        {
            if (_capturing)
                _currentText.append(s, len);
        }

        void addValueToParent(Value&& val)
        {
            if (_stack.empty())
            {
                _stack.emplace_back(std::move(val));
                return;
            }

            Value& parent = _stack.back();
            if (parent.getType() == Value::Type::MAP)
            {
                if (_keyStack.empty())
                {
                    _error = PList::ErrorCode::MALFORMED_DICT;
                }
                else
                {
                    std::string key = std::move(_keyStack.back());
                    _keyStack.pop_back();
                    parent.asValueMap()[key] = std::move(val);
                }
            }
            else if (parent.getType() == Value::Type::VECTOR)
            {
                parent.asValueVector().push_back(std::move(val));
            }
            else
            {
                _error = PList::ErrorCode::UNSUPPORTED_TYPE;
            }
        }
    };
}

std::string PList::encode(const Value& value, const Option& option)
{
    std::string result;
    result.reserve(option._sizeHint);

    result.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    result.append("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n");
    result.append("<plist version=\"1.0\">\n");
    encodeElement(value, result, 1);
    result.append("</plist>\n");

    return result;
}

PList::ErrorCode PList::decode(std::string_view content, Value& value)
{
    SaxHandler handler;
    rapidxml::xml_sax3_parser<> parser(&handler);
    try
    {
        // the parser will not modify the input string because we use the parse_non_destructive flag,
        // but it requires a non-const char* pointer, so we need to const_cast here.
        // This is safe as long as we don't actually modify the string, and the parser won't because of the flag.
        parser.parse<rapidxml::parse_non_destructive>(const_cast<char*>(content.data()), static_cast<int>(content.size()));
    }
    catch (rapidxml::parse_error&)
    {
        return PList::ErrorCode::PARSE_ERROR;
    }

    if (handler._error != PList::ErrorCode::SUCCESS)
    {
        return handler._error;
    }

    if (handler._stack.empty())
    {
        return PList::ErrorCode::MISSING_ROOT;
    }

    value = std::move(handler._stack.front());
    return PList::ErrorCode::SUCCESS;
}

NS_CC_END

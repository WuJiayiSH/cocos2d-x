#include "CCPList.h"

#include <charconv>
#include <limits>

#include "base/ccMacros.h"
#include "tinyxml2/tinyxml2.h"
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

    static void appendText(std::string_view text, std::string& out, int indentLevel)
    {
        for (int i = 0; i < indentLevel; ++i)
            out.append("    ");

        if (!text.empty())
            out.append(text);
    }

    static bool encodeElement(const Value& value, std::string& out, int indentLevel);

    static bool encodeDict(const ValueMap& dict, std::string& out, int indentLevel)
    {
        appendText("<dict>\n", out, indentLevel);
        for (const auto &[key, value] : dict)
        {
            appendTagWithText("key", key, out, indentLevel + 1);
            encodeElement(value, out, indentLevel + 1);
        }
        appendText("</dict>\n", out, indentLevel);
        return true;
    }

    static bool encodeArray(const ValueVector& array, std::string& out, int indentLevel)
    {
        appendText("<array>\n", out, indentLevel);
        for (const Value& item : array)
        {
            encodeElement(item, out, indentLevel + 1);
        }
        appendText("</array>\n", out, indentLevel);
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
            appendText("", out, indentLevel);
            out.append(value.asBool() ? "<true/>\n" : "<false/>\n");
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
        PList::ErrorCode error = PList::ErrorCode::SUCCESS;
        bool sawPlist = false;
        std::vector<Value> stack;
        std::vector<std::string> keyStack;
        std::string currentText;
        bool capturing = false;

        void xmlSAX2StartElement(const char* name, size_t len, const char** /*atts*/, size_t /*attslen*/) override
        {
            std::string element(name, len);
            if (element == "plist")
            {
                sawPlist = true;
                return;
            }

            if (element == "dict")
            {
                stack.emplace_back(ValueMap());
                return;
            }

            if (element == "array")
            {
                stack.emplace_back(ValueVector());
                return;
            }

            if (element == "string" || element == "integer" || element == "real" || element == "true" || element == "false" || element == "key")
            {
                currentText.clear();
                capturing = true;
                return;
            }

            // unknown element type
            error = PList::ErrorCode::UNSUPPORTED_TYPE;
        }

        void xmlSAX2EndElement(const char* name, size_t len) override
        {
            std::string element(name, len);
            if (element == "plist")
            {
                return;
            }

            if (element == "dict" || element == "array")
            {
                Value finished = std::move(stack.back());
                stack.pop_back();
                addValueToParent(std::move(finished));
                return;
            }

            if (element == "key")
            {
                keyStack.push_back(currentText);
                capturing = false;
                currentText.clear();
                return;
            }

            if (element == "string")
            {
                Value v(currentText);
                addValueToParent(std::move(v));
                capturing = false;
                currentText.clear();
                return;
            }

            if (element == "integer")
            {
                long long ll = 0;
                std::from_chars_result result = std::from_chars(currentText.data(), currentText.data() + currentText.size(), ll);
                if (result.ec != std::errc())
                {
                    error = PList::ErrorCode::PARSE_INTEGER_ERROR;
                }
                else
                {
                    if (ll >= 0)
                    {
                        Value v(static_cast<unsigned int>(ll));
                        addValueToParent(std::move(v));
                    }
                    else
                    {
                        Value v(static_cast<int>(ll));
                        addValueToParent(std::move(v));
                    }
                }
                capturing = false;
                currentText.clear();
                return;
            }

            if (element == "real")
            {
                double d = 0;
                std::from_chars_result result = std::from_chars(currentText.data(), currentText.data() + currentText.size(), d);
                if (result.ec != std::errc())
                {
                    error = PList::ErrorCode::PARSE_REAL_ERROR;
                }
                else
                {
                    Value v(d);
                    addValueToParent(std::move(v));
                }
                capturing = false;
                currentText.clear();
                return;
            }

            if (element == "true")
            {
                Value v(true);
                addValueToParent(std::move(v));
                return;
            }

            if (element == "false")
            {
                Value v(false);
                addValueToParent(std::move(v));
                return;
            }
        }

        void xmlSAX2Text(const char* s, size_t len) override
        {
            if (capturing)
            {
                currentText.append(s, len);
            }
        }

        void addValueToParent(Value&& val)
        {
            if (stack.empty())
            {
                stack.emplace_back(std::move(val));
                return;
            }

            Value& parent = stack.back();
            if (parent.getType() == Value::Type::MAP)
            {
                if (keyStack.empty())
                {
                    error = PList::ErrorCode::MALFORMED_DICT;
                    return;
                }
                std::string key = std::move(keyStack.back());
                keyStack.pop_back();
                parent.asValueMap()[key] = std::move(val);
            }
            else if (parent.getType() == Value::Type::VECTOR)
            {
                parent.asValueVector().push_back(std::move(val));
            }
            else
            {
                error = PList::ErrorCode::UNSUPPORTED_TYPE;
            }
        }
    };
}

std::string PList::encode(const Value& value)
{
    std::string result;
    result.reserve(256);

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

    if (!handler.sawPlist)
    {
        return PList::ErrorCode::MISSING_PLIST;
    }

    if (handler.error != PList::ErrorCode::SUCCESS)
    {
        return handler.error;
    }

    if (handler.stack.empty())
    {
        return PList::ErrorCode::MISSING_ROOT;
    }

    value = std::move(handler.stack.front());
    return PList::ErrorCode::SUCCESS;
}

NS_CC_END

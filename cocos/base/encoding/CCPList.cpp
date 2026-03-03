#include "CCPList.h"

#include <charconv>

#include "base/ccMacros.h"
#include "tinyxml2/tinyxml2.h"

NS_CC_BEGIN

namespace
{
    static tinyxml2::XMLElement* encodeElement(const Value& value, tinyxml2::XMLDocument& doc);

    static tinyxml2::XMLElement* encodeDict(const ValueMap& dict, tinyxml2::XMLDocument& doc)
    {
        tinyxml2::XMLElement* rootNode = doc.NewElement("dict");
        for (const auto &[key, value] : dict)
        {
            tinyxml2::XMLElement* keyNode = doc.NewElement("key");
            keyNode->LinkEndChild(doc.NewText(key.c_str()));
            rootNode->LinkEndChild(keyNode);

            if (tinyxml2::XMLElement* valueNode = encodeElement(value, doc))
            {
                rootNode->LinkEndChild(valueNode);
            }
        }
        return rootNode;
    }

    static tinyxml2::XMLElement* encodeArray(const ValueVector& array, tinyxml2::XMLDocument& doc)
    {
        tinyxml2::XMLElement* rootNode = doc.NewElement("array");
        for (const Value& item : array)
        {
            if (tinyxml2::XMLElement* valueNode = encodeElement(item, doc))
            {
                rootNode->LinkEndChild(valueNode);
            }
        }
        return rootNode;
    }

    static tinyxml2::XMLElement* encodeElement(const Value& value, tinyxml2::XMLDocument& doc)
    {
        switch (value.getType())
        {
        case Value::Type::STRING:
        {
            tinyxml2::XMLElement* node = doc.NewElement("string");
            node->LinkEndChild(doc.NewText(value.asString().c_str()));
            return node;
        }
        case Value::Type::INTEGER:
        case Value::Type::UNSIGNED:
        {
            tinyxml2::XMLElement* node = doc.NewElement("integer");
            node->LinkEndChild(doc.NewText(value.asString().c_str()));
            return node;
        }
        case Value::Type::BYTE:
        {
            tinyxml2::XMLElement* node = doc.NewElement("integer");
            std::string text = std::to_string(static_cast<int>(value.asByte()));
            node->LinkEndChild(doc.NewText(text.c_str()));
            return node;
        }
        case Value::Type::FLOAT:
        case Value::Type::DOUBLE:
        {
            tinyxml2::XMLElement* node = doc.NewElement("real");
            node->LinkEndChild(doc.NewText(value.asString().c_str()));
            return node;
        }
        case Value::Type::BOOLEAN:
        {
            return doc.NewElement(value.asBool() ? "true" : "false");
        }
        case Value::Type::VECTOR:
            return encodeArray(value.asValueVector(), doc);
        case Value::Type::MAP:
            return encodeDict(value.asValueMap(), doc);
        default:
            CCLOGERROR("CCPList: unsupported value type %d while encoding", static_cast<int>(value.getType()));
            return nullptr;
        }
    }

    static PList::ErrorCode decodeElement(const tinyxml2::XMLElement* element, Value& outValue);

    static PList::ErrorCode decodeDict(const tinyxml2::XMLElement* dictElement, ValueMap& outDict)
    {
        const tinyxml2::XMLElement* child = dictElement->FirstChildElement();
        while (child)
        {
            if (strcmp(child->Name(), "key") != 0)
            {
                return PList::ErrorCode::MALFORMED_DICT;
            }

            const char* keyText = child->GetText();
            if (!keyText)
            {
                keyText = "";
            }

            const tinyxml2::XMLElement* valueElement = child->NextSiblingElement();
            if (!valueElement)
            {
                return PList::ErrorCode::MALFORMED_DICT;
            }

            Value childValue;
            const PList::ErrorCode ret = decodeElement(valueElement, childValue);
            if (ret != PList::ErrorCode::SUCCESS)
            {
                return ret;
            }

            outDict[keyText] = std::move(childValue);
            child = valueElement->NextSiblingElement();
        }

        return PList::ErrorCode::SUCCESS;
    }

    static PList::ErrorCode decodeArray(const tinyxml2::XMLElement* arrayElement, ValueVector& outArray)
    {
        for (const tinyxml2::XMLElement* child = arrayElement->FirstChildElement(); child; child = child->NextSiblingElement())
        {
            Value childValue;
            const PList::ErrorCode ret = decodeElement(child, childValue);
            if (ret != PList::ErrorCode::SUCCESS)
            {
                return ret;
            }
            outArray.emplace_back(std::move(childValue));
        }
        return PList::ErrorCode::SUCCESS;
    }

    static PList::ErrorCode decodeElement(const tinyxml2::XMLElement* element, Value& outValue)
    {
        if (!element)
        {
            return PList::ErrorCode::INVALID_ELEMENT;
        }

        const char* name = element->Name();
        if (!name)
        {
            return PList::ErrorCode::INVALID_ELEMENT;
        }

        if (strcmp(name, "dict") == 0)
        {
            ValueMap dict;
            const PList::ErrorCode ret = decodeDict(element, dict);
            if (ret != PList::ErrorCode::SUCCESS)
            {
                return ret;
            }
            outValue = Value(std::move(dict));
            return PList::ErrorCode::SUCCESS;
        }

        if (strcmp(name, "array") == 0)
        {
            ValueVector array;
            const PList::ErrorCode ret = decodeArray(element, array);
            if (ret != PList::ErrorCode::SUCCESS)
            {
                return ret;
            }
            outValue = Value(std::move(array));
            return PList::ErrorCode::SUCCESS;
        }

        if (strcmp(name, "string") == 0)
        {
            const char* text = element->GetText();
            outValue = Value(text ? text : "");
            return PList::ErrorCode::SUCCESS;
        }

        if (strcmp(name, "integer") == 0)
        {
            const char* text = element->GetText();
            if (!text)
            {
                outValue = Value(0);
                return PList::ErrorCode::SUCCESS;
            }

            long long ll;
            std::from_chars_result result = std::from_chars(text, text + std::strlen(text), ll);
            if (result.ec != std::errc())
            {
                return PList::ErrorCode::PARSE_INTEGER_ERROR;
            }
            outValue = ll >= 0 ? Value(static_cast<unsigned int>(ll)) : Value(static_cast<int>(ll));
            return PList::ErrorCode::SUCCESS;
        }

        if (strcmp(name, "real") == 0)
        {
            const char* text = element->GetText();

            double d;
            std::from_chars_result result = std::from_chars(text, text + std::strlen(text), d);
            if (result.ec != std::errc())
            {
                return PList::ErrorCode::PARSE_REAL_ERROR;
            }
            outValue = Value(d);
            return PList::ErrorCode::SUCCESS;
        }

        if (strcmp(name, "true") == 0)
        {
            outValue = Value(true);
            return PList::ErrorCode::SUCCESS;
        }

        if (strcmp(name, "false") == 0)
        {
            outValue = Value(false);
            return PList::ErrorCode::SUCCESS;
        }

        return PList::ErrorCode::UNSUPPORTED_TYPE;
    }
}

std::string PList::encode(const Value& value)
{
    tinyxml2::XMLDocument doc;

    tinyxml2::XMLDeclaration* declaration = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.LinkEndChild(declaration);

    tinyxml2::XMLUnknown *docType = doc.NewUnknown("!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");
    doc.LinkEndChild(docType);

    tinyxml2::XMLElement* root = doc.NewElement("plist");
    root->SetAttribute("version", "1.0");
    doc.LinkEndChild(root);

    if (tinyxml2::XMLElement* payload = encodeElement(value, doc))
    {
        root->LinkEndChild(payload);
    }

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    return printer.CStr();
}

PList::ErrorCode PList::decode(std::string_view content, Value& value)
{
    tinyxml2::XMLDocument doc;
    const tinyxml2::XMLError ret = doc.Parse(content.data(), content.size());
    if (ret != tinyxml2::XML_SUCCESS)
    {
        return PList::ErrorCode::PARSE_ERROR;
    }

    const tinyxml2::XMLElement* plist = doc.FirstChildElement("plist");
    if (!plist)
    {
        return PList::ErrorCode::MISSING_PLIST;
    }

    const tinyxml2::XMLElement* root = plist->FirstChildElement();
    if (!root)
    {
        return PList::ErrorCode::MISSING_ROOT;
    }

    return decodeElement(root, value);
}

NS_CC_END

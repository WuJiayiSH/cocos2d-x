#include "CCJSON.h"

#include "json/memorystream.h"
#include "json/prettywriter.h"
#include "json/reader.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include <cstdint>
#include <limits>

NS_CC_BEGIN

namespace
{

template <typename WriterType>
void valueToJson(const Value& value, WriterType& writer)
{
    switch (value.getType())
    {
    case Value::Type::NONE:
        writer.Null();
        break;
    case Value::Type::BYTE:
    case Value::Type::INTEGER:
        writer.Int(value.asInt());
        break;
    case Value::Type::UNSIGNED:
        writer.Uint(value.asUnsignedInt());
        break;
    case Value::Type::FLOAT:
    case Value::Type::DOUBLE:
        writer.Double(value.asDouble());
        break;
    case Value::Type::BOOLEAN:
        writer.Bool(value.asBool());
        break;
    case Value::Type::STRING:
    {
        const std::string& stringValue = value.asString();
        writer.String(stringValue.c_str(), static_cast<rapidjson::SizeType>(stringValue.size()));
        break;
    }
    case Value::Type::VECTOR:
    {
        const ValueVector& valueVector = value.asValueVector();
        writer.StartArray();
        ValueVector::const_iterator iter = valueVector.begin();
        for (; iter != valueVector.end(); ++iter)
        {
            valueToJson(*iter, writer);
        }
        writer.EndArray(static_cast<rapidjson::SizeType>(valueVector.size()));
        break;
    }
    case Value::Type::MAP:
    {
        const ValueMap& valueMap = value.asValueMap();
        writer.StartObject();
        ValueMap::const_iterator iter = valueMap.begin();
        for (; iter != valueMap.end(); ++iter)
        {
            const std::string& key = iter->first;
            writer.Key(key.c_str(), static_cast<rapidjson::SizeType>(key.size()));
            valueToJson(iter->second, writer);
        }
        writer.EndObject(static_cast<rapidjson::SizeType>(valueMap.size()));
        break;
    }
    case Value::Type::INT_KEY_MAP:
    {
        CCLOGERROR("JSON does not support integer keys in objects, skipping");
        break;
    }
    }
}

struct JsonSaxHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, JsonSaxHandler>
{
    struct ContainerFrame
    {
        explicit ContainerFrame(Value&& containerValue)
            : container(std::move(containerValue))
        {
        }

        Value container;
        std::string pendingKey;
    };

    bool Null()
    {
        return addValue(Value::Null);
    }

    bool Bool(bool b)
    {
        return addValue(Value(b));
    }

    bool Int(int i)
    {
        return addValue(Value(i));
    }

    bool Uint(unsigned u)
    {
        return addValue(Value(u));
    }

    bool Int64(int64_t i)
    {
        CCLOGERROR("cocos2d::Value does not support 64-bit integer: %lld", static_cast<long long>(i));
        return false;
    }

    bool Uint64(uint64_t u)
    {
        CCLOGERROR("cocos2d::Value does not support 64-bit unsigned integer: %llu", static_cast<unsigned long long>(u));
        return false;
    }

    bool Double(double d)
    {
        return addValue(Value(d));
    }

    bool String(const char* str, rapidjson::SizeType length, bool /*copy*/)
    {
        return addValue(Value(std::string(str, length)));
    }

    bool StartObject()
    {
        _stack.emplace_back(Value(ValueMap()));
        return true;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool /*copy*/)
    {
        if (_stack.empty() || _stack.back().container.getType() != Value::Type::MAP)
        {
            return fail(JSON::ErrorCode::MALFORMED_STRUCTURE);
        }

        ContainerFrame& frame = _stack.back();
        if (!frame.pendingKey.empty())
        {
            return fail(JSON::ErrorCode::MALFORMED_STRUCTURE);
        }

        frame.pendingKey.assign(str, length);
        return true;
    }

    bool EndObject(rapidjson::SizeType /*memberCount*/)
    {
        if (_stack.empty() || _stack.back().container.getType() != Value::Type::MAP)
        {
            return fail(JSON::ErrorCode::MALFORMED_STRUCTURE);
        }

        ContainerFrame finished = std::move(_stack.back());
        _stack.pop_back();

        if (!finished.pendingKey.empty())
        {
            return fail(JSON::ErrorCode::MALFORMED_STRUCTURE);
        }

        return addValue(std::move(finished.container));
    }

    bool StartArray()
    {
        _stack.emplace_back(Value(ValueVector()));
        return true;
    }

    bool EndArray(rapidjson::SizeType /*elementCount*/)
    {
        if (_stack.empty() || _stack.back().container.getType() != Value::Type::VECTOR)
        {
            return fail(JSON::ErrorCode::MALFORMED_STRUCTURE);
        }

        Value finished = std::move(_stack.back().container);
        _stack.pop_back();
        return addValue(std::move(finished));
    }

    bool hasResult() const
    {
        return _hasRoot && _error == JSON::ErrorCode::SUCCESS;
    }

    Value&& takeResult()
    {
        return std::move(_root);
    }

    JSON::ErrorCode error() const
    {
        return _error;
    }

    private:
    bool addValue(Value value)
    {
        if (_stack.empty())
        {
            if (_hasRoot)
            {
                return fail(JSON::ErrorCode::MULTIPLE_ROOT);
            }

            _root = std::move(value);
            _hasRoot = true;
            return true;
        }

        ContainerFrame& parent = _stack.back();
        if (parent.container.getType() == Value::Type::VECTOR)
        {
            parent.container.asValueVector().push_back(std::move(value));
            return true;
        }

        if (parent.container.getType() != Value::Type::MAP || parent.pendingKey.empty())
        {
            return fail(JSON::ErrorCode::MALFORMED_STRUCTURE);
        }

        std::string key = std::move(parent.pendingKey);
        parent.pendingKey.clear();
        parent.container.asValueMap()[key] = std::move(value);
        return true;
    }

    bool fail(JSON::ErrorCode error)
    {
        if (_error == JSON::ErrorCode::SUCCESS)
        {
            _error = error;
        }
        return false;
    }

    std::vector<ContainerFrame> _stack;
    Value _root;
    bool _hasRoot = false;
    JSON::ErrorCode _error = JSON::ErrorCode::SUCCESS;
};

} // namespace

std::string JSON::encode(const Value& value, const Option& option)
{
    rapidjson::StringBuffer buffer;
    buffer.Reserve(option._sizeHint);

    if (option._prettyPrint)
    {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        valueToJson(value, writer);
    }
    else
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        valueToJson(value, writer);
    }

    return std::string(buffer.GetString(), buffer.GetSize());
}

JSON::ErrorCode JSON::decode(std::string_view content, Value& value)
{
    rapidjson::Reader reader;
    rapidjson::MemoryStream stream(content.data(), content.size());
    JsonSaxHandler handler;

    if (!reader.Parse(stream, handler))
    {
        switch (reader.GetParseErrorCode())
        {
        case rapidjson::kParseErrorDocumentEmpty:
            return JSON::ErrorCode::EMPTY_INPUT;
        default:
            return JSON::ErrorCode::PARSE_ERROR;
        }
    }

    if (handler.error() != JSON::ErrorCode::SUCCESS)
    {
        return handler.error();
    }

    if (!handler.hasResult())
    {
        return ErrorCode::INVALID_ROOT;
    }

    value = handler.takeResult();
    return ErrorCode::SUCCESS;
}

NS_CC_END

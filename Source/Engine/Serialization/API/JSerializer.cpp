#include "JSerializer.h"

#include <Engine/Platform/PlatformFile.h>

#include <nlohmann/json.hpp>

namespace Hollow
{
	JObject* ToObjectRecursive(const nlohmann::json& json, const String& name = "");
	JArray* ToArrayRecursive(const nlohmann::json& json, const String& name = "");

	void ObjectToJson(JObject* object, nlohmann::json& json);
	void ArrayToJson(JArray* array, nlohmann::json& json);

	JObject* JSerializer::ToObject(const String& path)
	{
		String binary;
		if (!PlatformFile::Read(path, binary))
		{
			CORE_LOG(HE_ERROR, "Failed to read file [%s].", path.c_str());
			return nullptr;
		}

		nlohmann::json json;
		try
		{
			json = nlohmann::json::parse(binary);
		}
		catch (const std::exception& e)
		{
			CORE_LOG(HE_ERROR, "Failed to parse json file [%s].", path.c_str());
			return nullptr;
		}

		return ToObjectRecursive(json);
	}

	void JSerializer::ToJson(JObject* object, const String& writePath)
	{
		nlohmann::json outJson;

		ObjectToJson(object, outJson);

		String binary = outJson.dump(4);

		if (!PlatformFile::Write(writePath, binary))
			CORE_LOG(HE_ERROR, "Failed to write file [%s].", writePath.c_str());

		printf("%s\n", binary.c_str());
	}

	JObject* ToObjectRecursive(const nlohmann::json& json, const String& name)
	{
		JObject* object = new JObject(name);

		for (auto& [key, value] : json.items())
		{
			switch (value.type())
			{
			case nlohmann::detail::value_t::object:
			{
				JObject* child = ToObjectRecursive(value, key);
				object->AddChild(key, std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::array:
			{
				JArray* child = ToArrayRecursive(value, key);
				object->AddChild(key, std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::string:
			{
				auto child = new JString(key, value.get<std::string>());
				object->AddChild(key, std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::number_integer:
			{
				auto child = new JNumber<i64>(key, value.get<i64>());
				object->AddChild(key, std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::number_unsigned:
			{
				auto child = new JNumber<u64>(key, value.get<u64>());
				object->AddChild(key, std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::number_float:
			{
				auto child = new JNumber<f64>(key, value.get<f64>());
				object->AddChild(key, std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::boolean:
			{
				auto child = new JBool(key, value.get<bool>());
				object->AddChild(key, std::move(child));
				continue;
			}
			default:
				continue;
			}
		}

		return object;
	}

	JArray* ToArrayRecursive(const nlohmann::json& json, const String& name)
	{
		JArray* array = new JArray(name);


		for (auto& [key, value] : json.items())
		{
			switch (value.type())
			{
			case nlohmann::detail::value_t::object:
			{
				JObject* child = ToObjectRecursive(value, key);
				array->AddNode(std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::array:
			{
				JArray* child = ToArrayRecursive(value, key);
				array->AddNode(std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::string:
			{
				auto child = new JString(key, value.get<std::string>());
				array->AddNode(std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::number_integer:
			{
				auto child = new JNumber<i64>(key, value.get<i64>());
				array->AddNode(std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::number_unsigned:
			{
				auto child = new JNumber<u64>(key, value.get<u64>());
				array->AddNode(std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::number_float:
			{
				auto child = new JNumber<f64>(key, value.get<f64>());
				array->AddNode(std::move(child));
				continue;
			}
			case nlohmann::detail::value_t::boolean:
			{
				auto child = new JBool(key, value.get<bool>());
				array->AddNode(std::move(child));
				continue;
			}
			default:
				continue;
			}
		}

		return array;
	}

	void ObjectToJson(JObject* object, nlohmann::json& json)
	{
		for (auto& [key, value] : object->GetChildren())
		{
			switch (value->GetType())
			{
			case NodeType::Object:
			{
				nlohmann::json child;
				ObjectToJson(static_cast<JObject*>(value), child);
				json[key] = child;
				continue;
			}
			case NodeType::Array:
			{
				nlohmann::json child;
				ArrayToJson(static_cast<JArray*>(value), child);
				json[key] = child;
				continue;
			}
			case NodeType::String:
			{
				json[key] = static_cast<JString*>(value)->GetValue();
				continue;
			}
			case NodeType::Float:
			{
				json[key] = static_cast<JNumber<f64>*>(value)->GetValue();
				continue;
			}
			case NodeType::Int:
			{
				json[key] = static_cast<JNumber<i64>*>(value)->GetValue();
				continue;
			}
			case NodeType::UInt:
			{
				json[key] = static_cast<JNumber<u64>*>(value)->GetValue();
				continue;
			}
			case NodeType::Boolean:
			{
				json[key] = static_cast<JBool*>(value)->GetValue();
				continue;
			}
			default:
				continue;
			}
		}
	}

	void ArrayToJson(JArray* array, nlohmann::json& json)
	{
		for (auto& node : array->GetNodes())
		{
			switch (node->GetType())
			{
			case NodeType::Object:
			{
				nlohmann::json child;
				ObjectToJson(static_cast<JObject*>(node), child);
				json.push_back(child);
				break;
			}
			case NodeType::Array:
			{
				nlohmann::json child;
				ArrayToJson(static_cast<JArray*>(node), child);
				json.push_back(child);
				break;
			}
			case NodeType::String:
			{
				json.push_back(static_cast<JString*>(node)->GetValue());
				break;
			}
			case NodeType::Float:
			{
				json.push_back(static_cast<JNumber<f64>*>(node)->GetValue());
				break;
			}
			case NodeType::Int:
			{
				json.push_back(static_cast<JNumber<i64>*>(node)->GetValue());
				break;
			}
			case NodeType::UInt:
			{
				json.push_back(static_cast<JNumber<u64>*>(node)->GetValue());
				break;
			}
			case NodeType::Boolean:
			{
				json.push_back(static_cast<JBool*>(node)->GetValue());
				break;
			}
			default:
				break;
			}
		}
	}
}

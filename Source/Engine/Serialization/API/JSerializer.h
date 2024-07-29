#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Engine/Serialization/JObject.h>
#include <Engine/Serialization/JBool.h>
#include <Engine/Serialization/JArray.h>
#include <Engine/Serialization/JNumber.h>
#include <Engine/Serialization/JString.h>

namespace Hollow
{
	class JSerializer : public API<JSerializer>
	{
	public:
		JSerializer() = default;
		~JSerializer() = default;

		JObject* ToObject(const String& path);
		void ToJson(JObject* object, const String& writePath);
	};
}

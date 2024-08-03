#include <Engine/Platform/API/PlatformAPI.h>
#include "TestApp.h"

#include <Engine/Math/ColorConverter.h>

#include <Engine/Serialization/JNumber.h>
#include <Engine/Serialization/JObject.h>
#include <Engine/Serialization/JArray.h>
#include <Engine/Serialization/JString.h>
#include <Engine/Serialization/JBool.h>
#include <Engine/Serialization/API/JSerializer.h>

using namespace Hollow;

#include <chrono>
#include <iostream>
#include <type_traits>

int main(i32 argC, char** argV)
{
	PlatformAPI::GetAPI()->InitializeArguments(argC, argV);

	SharedPtr<TestApp> app = MakeShared<TestApp>();
	app->Run();
}

//enum class PrimitiveType
//{
//	Int8,
//	Int16,
//	Int32,
//	Int64,
//	UInt8,
//	UInt16,
//	UInt32,
//	UInt64,
//	Float32,
//	Float64,
//	String,
//	Bool,
//};
//
//
//template<typename T>
//struct TypeReflection;
//
//template<>
//struct TypeReflection<String>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::String; }
//};
//
//template<>
//struct TypeReflection<i8>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Int8; }
//};
//
//template<>
//struct TypeReflection<i16>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Int16; }
//};
//
//template<>
//struct TypeReflection<i32>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Int32; }
//};
//
//template<>
//struct TypeReflection<i64>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Int64; }
//};
//
//template<>
//struct TypeReflection<u8>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::UInt8; }
//};
//
//template<>
//struct TypeReflection<u16>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::UInt16; }
//};
//
//template<>
//struct TypeReflection<u32>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::UInt32; }
//};
//
//template<>
//struct TypeReflection<u64>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::UInt64; }
//};
//
//template<>
//struct TypeReflection<f32>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Float32; }
//};
//
//template<>
//struct TypeReflection<f64>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Float64; }
//};
//
//template<>
//struct TypeReflection<bool>
//{
//	static constexpr PrimitiveType GetType() { return PrimitiveType::Bool; }
//};
//
//#define REGISTER_BEGIN_REFLECTION(className) \
//	public: \
//	class className##Reflect \
//	{ \
//	public:
//
//#define REGISTER_END_REFLECTION() \
//	};
//
//#define REGISTER_TYPE(varName) \
//		static constexpr PrimitiveType varName##GetType() { \
//		return TypeReflection<std::remove_reference_t<decltype(varName)>>::GetType(); \
//	}\
//
//#define REFLECT_OBJECT(className) \
//	friend class className##Reflect;
//
//class Test
//{
//	REFLECT_OBJECT(Test);
//public:
//	Test() {}
//	~Test() {}
//
//private:
//	i8 a;
//	f32 b;
//	std::string c;
//	bool d;
//
//	REGISTER_BEGIN_REFLECTION(Test);
//	REGISTER_TYPE(a);
//	REGISTER_TYPE(b);
//	REGISTER_TYPE(c);
//	REGISTER_TYPE(d);
//	REGISTER_END_REFLECTION();
//};
//
//int main()
//{
//	Test t;
//
//	int a = (int)Test::TestReflect::aGetType();
//	int b = (int)Test::TestReflect::bGetType();
//	int c = (int)Test::TestReflect::cGetType();
//	int d = (int)Test::TestReflect::dGetType();
//
//	printf("type for a: %d\n", a);
//	printf("type for b: %d\n", b);
//	printf("type for c: %d\n", c);
//	printf("type for d: %d\n", d);
//}

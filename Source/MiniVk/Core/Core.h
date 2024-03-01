#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1910

typedef unsigned char uint8, byte;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

#define uint64_max 0xFFFFFFFFFFFFFFFF

#define NODISCARD [[nodiscard]]
#endif

#define GENERATE_ENUM_FLAG(EnumType, primitiveType) \
FORCEINLINE static EnumType operator | (EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<primitiveType>(a) | static_cast<primitiveType>(b)); } \
FORCEINLINE static bool operator & (EnumType a, EnumType b) { return (static_cast<primitiveType>(a) & static_cast<primitiveType>(b)) != 0; } \

namespace MiniVk
{
	enum class LogLevel
	{
		Info,
		Verbose,
		Warning,
		Error,
		Fatal
	};

	void RUNTIME_API DevLog(LogLevel type, const char* pMessage, ...);
	void RUNTIME_API DevAssert(bool condition, const char* pTitle, const char* pFailed, ...);
	void RUNTIME_API CoreLog(LogLevel type, const char* pTitle, const char* pMessage, ...);
}

#if defined(MINIVK_DEBUG)

#define HE_INFO MiniVk::LogLevel::Info
#define HE_VERBOSE MiniVk::LogLevel::Verbose
#define HE_WARNING MiniVk::LogLevel::Warning
#define HE_ERROR MiniVk::LogLevel::Error
#define HE_FATAL MiniVk::LogLevel::Fatal

#define DEV_ASSERT(condition, title, message, ...) MiniVk::DevAssert(condition, title, message, __VA_ARGS__)
#define DEV_LOG(logType, message, ...) MiniVk::DevLog(logType, message, __VA_ARGS__)
#define CORE_LOG(logType, title, message, ...) MiniVk::CoreLog(logType, title, message, __VA_ARGS__)

#else

#define HE_INFO
#define HE_VERBOSE
#define HE_WARNING
#define HE_ERROR
#define HE_FATAL

#define DEV_ASSERT(condition, title, message, ...)
#define DEV_LOG(logType, message, ...)
#define CORE_LOG(logType, title, message, ...)

#endif

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

template<typename T>
using Array = std::vector<T>;
using String = std::string;
using WString = std::wstring;
template<typename T>
using OwnedPtr = std::unique_ptr<T>;
template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;
template<typename T, typename U>
using Pair = std::pair<T, U>;
template<typename T, typename U>
using HashMap = std::unordered_map<T, U>;

template<typename T>
using EnableSharedFromThis = std::enable_shared_from_this<T>;

#include <DirectXMath.h>
using namespace DirectX;

using Vector2f = XMFLOAT2;
using Vector3f = XMFLOAT3;
using Vector4f = XMFLOAT4;

using Vector2i = XMINT2;
using Vector3i = XMINT3;
using Vector4i = XMINT4;

using Vector2u = XMUINT2;
using Vector3u = XMUINT3;
using Vector4u = XMUINT4;

using Matrix4f = XMMATRIX;


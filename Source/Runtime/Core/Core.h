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

namespace Hollow
{
	enum class LogLevel
	{
		Info,
		Verbose,
		Warning,
		Error
	};

	void RUNTIME_API DevLog(LogLevel type, const char* pMessage, ...);
	void RUNTIME_API DevAssert(bool condition, const char* pTitle, const char* pFailed, ...);
}

#if defined(HOLLOW_DEBUG)

#define HE_INFO Hollow::LogLevel::Info
#define HE_VERBOSE Hollow::LogLevel::Verbose
#define HE_WARNING Hollow::LogLevel::Warning
#define HE_ERROR Hollow::LogLevel::Error

#define DEV_ASSERT(condition, title, message, ...) Hollow::DevAssert(condition, title, message, __VA_ARGS__)
#define DEV_LOG(logType, message, ...) Hollow::DevLog(logType, message, __VA_ARGS__)
#else
#define DEV_ASSERT(condition, title, message, ...) (condition)
#endif

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

template<typename T>
using ArrayList = std::vector<T>;
template<typename T, size_t N>
using Array = std::array<T, N>;
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


#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1910

#if defined(_WIN64)
typedef unsigned char uint8, byte;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64, size64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef float float32;
typedef double float64;
#endif

#define uint64_max 0xFFFFFFFFFFFFFFFF
#define uint32_max 0xFFFFFFFF
#define uint16_max 0xFFFF
#define uint8_max 0xFF

#endif

#define MB_TO_BYTE(mb) (mb * 1000.0 * 1000.0)
#define KB_TO_BYTE(kb) (kb * 1000.0)
#define BYTE_TO_MB(bt) (bt / 1000.0 / 1000.0)
#define BYTE_TO_KB(bt) (bt / 1000.0)

#define GENERATE_ENUM_FLAG(EnumType, primitiveType) \
FORCEINLINE static EnumType operator | (EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<primitiveType>(a) | static_cast<primitiveType>(b)); } \
FORCEINLINE static bool operator & (EnumType a, EnumType b) { return (static_cast<primitiveType>(a) & static_cast<primitiveType>(b)) != 0; } \
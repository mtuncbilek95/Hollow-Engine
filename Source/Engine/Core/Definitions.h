#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1910

#if defined(_WIN64)
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64, s64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;
#endif

#define u64_max 0xFFFFFFFFFFFFFFFF
#define u32_max 0xFFFFFFFF
#define u16_max 0xFFFF
#define u8_max 0xFF

#define DEPRECATED_CLASS [[deprecated("This class is deprecated and will be removed in the future")]]
#define DEPRECATED_FUNCTION [[deprecated("This function is deprecated and will be removed in the future")]]
#define DEPRECATED_VARIABLE [[deprecated("This variable is deprecated and will be removed in the future")]]
#define DEPRECATED_ENUM [[deprecated("This enum is deprecated and will be removed in the future")]]
#define DEPRECATED_STRUCT [[deprecated("This struct is deprecated and will be removed in the future")]]

#endif

#if defined(__GNUC__)
#if defined(__x86_64__)
#define DEPRECATED_CLASS __attribute__((deprecated("This class is deprecated and will be removed in the future")))
#define DEPRECATED_FUNCTION __attribute__((deprecated("This function is deprecated and will be removed in the future")))
#define DEPRECATED_VARIABLE __attribute__((deprecated("This variable is deprecated and will be removed in the future")))
#define DEPRECATED_ENUM __attribute__((deprecated("This enum is deprecated and will be removed in the future")))
#define DEPRECATED_STRUCT __attribute__((deprecated("This struct is deprecated and will be removed in the future")))
#endif
#endif

#define MB_TO_BYTE(mb) (mb * 1024.0f * 1024.0f)
#define KB_TO_BYTE(kb) (kb * 1024.0f)
#define BYTE_TO_MB(bt) (bt / 1024.0f / 1024.0f)
#define BYTE_TO_KB(bt) (bt / 1024.0f)

#define GENERATE_ENUM_FLAG(EnumType, primitiveType) \
FORCEINLINE static EnumType operator | (EnumType a, EnumType b) { return static_cast<EnumType>(static_cast<primitiveType>(a) | static_cast<primitiveType>(b)); } \
FORCEINLINE static bool operator & (EnumType a, EnumType b) { return static_cast<bool>(static_cast<primitiveType>(a) & static_cast<primitiveType>(b)); }





#pragma once

#include "Definitions.h"
#include "StdFix.h"
#include "GlobalConsole.h"
#include "Guid.h"

#if defined(HOLLOW_DEBUG)

#define HE_INFO Hollow::ConsoleType::Info
#define HE_VERBOSE Hollow::ConsoleType::Verbose
#define HE_WARNING Hollow::ConsoleType::Warning
#define HE_ERROR Hollow::ConsoleType::Error

#define CORE_LOG(level, title, message, ...) Hollow::GlobalConsole::Log(level, title, message, __VA_ARGS__)
#define CORE_PRINT(message, ...) Hollow::GlobalConsole::Print(message, __VA_ARGS__)
#define CORE_ASSERT(condition, title, message, ...) Hollow::GlobalConsole::Assert(condition, title, message, __VA_ARGS__)
#else

#define HE_INFO
#define HE_VERBOSE
#define HE_WARNING
#define HE_ERROR

#define CORE_LOG(level, title, message, ...) 
#define PRINT_LOG(message, ...)
#define CORE_ASSERT(condition, title, message, ...) (condition)
#endif


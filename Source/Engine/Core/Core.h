#pragma once

#include "Definitions.h"
#include "StdFix.h"
#include "GlobalConsole.h"
#include "Guid.h"

#define HE_INFO Hollow::ConsoleType::Info
#define HE_VERBOSE Hollow::ConsoleType::Verbose
#define HE_WARNING Hollow::ConsoleType::Warning
#define HE_ERROR Hollow::ConsoleType::Error

#define CORE_LOG(level, message, ...) Hollow::GlobalConsole::Log(level, message, __VA_ARGS__)
#define CORE_PRINT(message, ...) Hollow::GlobalConsole::Print(message, __VA_ARGS__)
#define CORE_ASSERT(condition, title, message, ...) Hollow::GlobalConsole::Assert(condition, title, message, __VA_ARGS__)


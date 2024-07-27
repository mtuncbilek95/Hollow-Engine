cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# JSON Library
FetchContent_Declare(
        json
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(json)
#pragma once

#include <String>
using String = std::string;

#include <vector>
template<typename T>
using DArray = std::vector<T>;

#include <map>
template<typename K, typename V>
using Map = std::map<K, V>;

template<typename K, typename V>
using Pair = std::pair<K, V>;

#include <set>
template<typename T>
using Set = std::set<T>;

#include <unordered_map>
template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

#include <unordered_set>
template<typename T>
using HashSet = std::unordered_set<T>;

#include <memory>
template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using OwnedPtr = std::unique_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;

template<typename T, typename...Args>
inline SharedPtr<T> MakeShared(Args&&...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename...Args>
inline OwnedPtr<T> MakeOwned(Args&&...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using Vec2f = glm::vec2;
using Vec3f = glm::vec3;
using Vec4f = glm::vec4;

using Mat4f = glm::mat4;
using Mat3f = glm::mat3;

using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;
using Vec4i = glm::ivec4;

using Vec2u = glm::uvec2;
using Vec3u = glm::uvec3;
using Vec4u = glm::uvec4;

namespace Math = glm;
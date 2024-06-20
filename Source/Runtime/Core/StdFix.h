#pragma once

#include <String>
using String = std::string;

#include <vector>
template<typename T>
using DArray = std::vector<T>;

#include <map>
template<typename K, typename V>
using Map = std::map<K, V>;

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


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;

using Matrix4f = glm::mat4;
using Matrix3f = glm::mat3;

using Vector2i = glm::ivec2;
using Vector3i = glm::ivec3;
using Vector4i = glm::ivec4;

using Vector2u = glm::uvec2;
using Vector3u = glm::uvec3;
using Vector4u = glm::uvec4;

using Vec2f = Vector2f;
using Vec3f = Vector3f;
using Vec4f = Vector4f;

using Mat4f = Matrix4f;
using Mat3f = Matrix3f;

using Vec2i = Vector2i;
using Vec3i = Vector3i;
using Vec4i = Vector4i;

using Vec2u = Vector2u;
using Vec3u = Vector3u;
using Vec4u = Vector4u;

namespace Math = glm;
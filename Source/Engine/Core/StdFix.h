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

#include <list>
template<typename T>
using List = std::list<T>;

#include <memory>
template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using OwnedPtr = std::unique_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;

#include <functional>
template<typename T>
using Function = std::function<T>;

#include <deque>
template<typename T>
using Deque = std::deque<T>;

#include <queue>
template<typename T>
using Queue = std::queue<T>;

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

using Vec2d = glm::dvec2;
using Vec3d = glm::dvec3;
using Vec4d = glm::dvec4;

using Mat4f = glm::mat4;
using Mat3f = glm::mat3;

using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;
using Vec4i = glm::ivec4;

using Vec2u = glm::uvec2;
using Vec3u = glm::uvec3;
using Vec4u = glm::uvec4;

using Vec2b = glm::bvec2;
using Vec3b = glm::bvec3;
using Vec4b = glm::bvec4;

using Vec2s = glm::vec<2, short>;
using Vec3s = glm::vec<3, short>;
using Vec4s = glm::vec<4, short>;


namespace Math = glm;
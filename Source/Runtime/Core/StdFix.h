#pragma once

#include <String>
using String = std::string;

#include <vector>
template<typename T>
using ArrayList = std::vector<T>;

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
using UniquePtr = std::unique_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;

#include <DirectXMath.h>
using Vector2f = DirectX::XMFLOAT2;
using Vector3f = DirectX::XMFLOAT3;
using Vector4f = DirectX::XMFLOAT4;
using Matrix4f = DirectX::XMFLOAT4X4;
using Matrix3f = DirectX::XMFLOAT3X3;

using Vector2i = DirectX::XMINT2;
using Vector3i = DirectX::XMINT3;
using Vector4i = DirectX::XMINT4;

using Vector2u = DirectX::XMUINT2;
using Vector3u = DirectX::XMUINT3;
using Vector4u = DirectX::XMUINT4;

using VectorSIMD = DirectX::XMVECTOR;
using MatrixSIMD = DirectX::XMMATRIX;

using namespace DirectX;

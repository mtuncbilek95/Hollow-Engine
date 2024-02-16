#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API ViewportDesc
	{
		Vector2u ImageSize;
		Vector2u OffsetSize;
		Vector2f DepthRange;
	};
}
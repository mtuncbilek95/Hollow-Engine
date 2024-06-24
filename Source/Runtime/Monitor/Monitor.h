#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Monitor/MonitorDesc.h>

namespace Hollow
{
	class RUNTIME_API Monitor
	{
	public:
		Monitor() = default;
		Monitor(const MonitorDesc& desc) : mMonitorName(desc.MonitorName), 
			mMonitorResolution(desc.MonitorResolution), mMonitorPosition(desc.MonitorPosition),
			mIsPrimary(desc.IsPrimary) 
		{}
		~Monitor() = default;

		const String& GetMonitorName() const { return mMonitorName; }
		const Vec2u& GetMonitorResolution() const { return mMonitorResolution; }
		const Vec2i& GetMonitorPosition() const { return mMonitorPosition; }
		const bool IsPrimary() const { return mIsPrimary; }

	private:
		String mMonitorName;
		Vec2u mMonitorResolution;
		Vec2i mMonitorPosition;
		bool mIsPrimary;
	};
}

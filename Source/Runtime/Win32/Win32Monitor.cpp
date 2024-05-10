#include "Win32Monitor.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Hollow
{
#if defined(HOLLOW_PLATFORM_WINDOWS)
	BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
	{
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &monitorInfo);

		DEVMODE devMode = {};
		devMode.dmSize = sizeof(devMode);
		devMode.dmDriverExtra = 0;
		EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

		ArrayList<Vector2i> resolutions;
		DEVMODE resMode = {};
		resMode.dmSize = sizeof(resMode);
		resMode.dmDriverExtra = 0;
		int modeNum = 0;
		while (EnumDisplaySettings(NULL, modeNum, &resMode))
		{
			if (resMode.dmDisplayFrequency == devMode.dmDisplayFrequency && resMode.dmBitsPerPel == devMode.dmBitsPerPel && resMode.dmDisplayFixedOutput == devMode.dmDisplayFixedOutput)
			{
				Vector2i resolution;
				resolution.x = resMode.dmPelsWidth;
				resolution.y = resMode.dmPelsHeight;
				resolutions.push_back(resolution);
			}
			modeNum++;
		}

		MonitorDesc monitorDesc = {};
		monitorDesc.MonitorName = monitorInfo.szDevice;
		monitorDesc.BitsPerPixel = devMode.dmBitsPerPel;
		monitorDesc.RefreshRate = devMode.dmDisplayFrequency;
		monitorDesc.MonitorResolution.x = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		monitorDesc.MonitorResolution.y = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
		monitorDesc.SupportedResolutions = resolutions;

		SharedPtr<Monitor> monitor = Monitor::CreateMonitorInfo(monitorDesc);
		((ArrayList<SharedPtr<Monitor>>*)dwData)->push_back(monitor);

		return true;
	}
#endif

	SharedPtr<Monitor> Win32Monitor::GetPrimaryMonitor()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

		DEVMODE devMode = {};
		devMode.dmSize = sizeof(devMode);
		devMode.dmDriverExtra = 0;
		EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

		ArrayList<Vector2i> resolutions;
		DEVMODE resMode = {};
		resMode.dmSize = sizeof(resMode);
		resMode.dmDriverExtra = 0;
		int modeNum = 0;
		while (EnumDisplaySettings(monitorInfo.szDevice, modeNum, &resMode))
		{
			if (resMode.dmDisplayFrequency == devMode.dmDisplayFrequency && resMode.dmBitsPerPel == devMode.dmBitsPerPel && resMode.dmDisplayFixedOutput == devMode.dmDisplayFixedOutput)
			{
				Vector2i resolution;
				resolution.x = resMode.dmPelsWidth;
				resolution.y = resMode.dmPelsHeight;
				resolutions.push_back(resolution);
			}
			modeNum++;
		}

		MonitorDesc monitorDesc = {};
		monitorDesc.MonitorName = monitorInfo.szDevice;
		monitorDesc.BitsPerPixel = devMode.dmBitsPerPel;
		monitorDesc.RefreshRate = devMode.dmDisplayFrequency;
		monitorDesc.MonitorResolution.x = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		monitorDesc.MonitorResolution.y = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
		monitorDesc.SupportedResolutions = resolutions;

		return Monitor::CreateMonitorInfo(monitorDesc);
#else
		return nullptr;
#endif
	}

	ArrayList<SharedPtr<Monitor>> Win32Monitor::GetAllMonitors()
	{
		ArrayList<SharedPtr<Monitor>> monitors;
#if defined(HOLLOW_PLATFORM_WINDOWS)
		uint32 index = 0;
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&monitors);

		return monitors;
#else
		return monitors;
#endif
	}
}


/* -----------------------------------------
   Libraries
----------------------------------------- */

// None


/* -----------------------------------------
   Unicode
----------------------------------------- */

#ifndef UNICODE
	#undef _UNICODE
#else
	#ifndef _UNICODE
		#define _UNICODE
	#endif
#endif


/* -----------------------------------------
   Directives
----------------------------------------- */

#define _WIN32_WINNT 0x0500 // Needed for MonitorFromWindow()


/* -----------------------------------------
   Includes
----------------------------------------- */

#include <windows.h>
#include <string>

#include <iostream>


/* -----------------------------------------
   Functions
----------------------------------------- */

int fn_display_get_width(HWND hWnd) {
	
	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	
	int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
	
	return monitor_width;
	
};

int fn_display_get_height(HWND hWnd) {
	
	HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	
	int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
	
	return monitor_height;
	
};

int fn_display_set_size(double mwidth, double mheight)
{
       
	DEVMODE newSettings;	
	EnumDisplaySettings(NULL, 0, &newSettings);
	
	//newSettings.dmDisplayFrequency  = mfreq;
	//newSettings.dmBitsPerPel  = mpixels;
 	newSettings.dmPelsWidth  = mwidth;		
	newSettings.dmPelsHeight = mheight;	
	
	//newSettings.dmFields = DM_DISPLAYFREQUENCY | DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
 	newSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	
	//if (ChangeDisplaySettings ( &newSettings, CDS_TEST) {
	//	return 0;
	//} else {
	//	return 1;
	//}
	
	
	// not temporary: CDS_NORESET | CDS_UPDATEREGISTRY
	
	if (ChangeDisplaySettings( &newSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL) {
		return 0;
	} else {
		return 1;
	}
	
};

int fn_display_get_sizes() 
{
	
	DEVMODE DevMode;
	LPCSTR sDisplayInfo;
	BOOL bResult = TRUE;
	DWORD dwCurrentSettings = 0;
	DevMode.dmSize = sizeof(DEVMODE);
	int lastHeight = 0; // needed so we get only unique resolution sizes
	
	while (bResult) {
		
		bResult = EnumDisplaySettings(NULL, dwCurrentSettings, &DevMode);
		
		if (bResult) {
			
			if (lastHeight != DevMode.dmPelsHeight ) {
				std::cout << DevMode.dmPelsWidth << "x" <<DevMode.dmPelsHeight << " @" <<DevMode.dmDisplayFrequency << std::endl;
			}
			
			lastHeight = DevMode.dmPelsHeight;
			dwCurrentSettings++;
		}
		
		
	}
	
	std::cout << std::endl;
	
	return 1;
	
};

/*int fn_display_get_devices() {
	
	DISPLAY_DEVICE dd;

    dd.cb = sizeof(DISPLAY_DEVICE);

    DWORD deviceNum = 0;
    while( EnumDisplayDevices(NULL, deviceNum, &dd, 0) ){
        
		//DumpDevice( dd, 0 );
        DISPLAY_DEVICE newdd = {0};
        newdd.cb = sizeof(DISPLAY_DEVICE);
        DWORD monitorNum = 0;
        while ( EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
        {
            std::cout << newdd.DeviceName << std::endl;
            monitorNum++;
        }
        
        deviceNum++;
    }

	std::cout << std::endl;
	
    return 1;
	
};*/

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	std::cout << "Display Width: " << fn_display_get_width(NULL) << std::endl;
	std::cout << "Display Height: " << fn_display_get_height(NULL) << std::endl << std::endl;
	
	//std::cout << "Get Possible Devices: " << std::endl;
	//std::cout << fn_display_get_devices() << std::endl;
	
	std::cout << "Get Possible Screen Resolutions: " << std::endl;
	std::cout << fn_display_get_sizes() << std::endl;
	
	std::cin.get();
	std::cout << "Display Set Screen Resolution: " << fn_display_set_size(1600, 900) << std::endl;	
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}


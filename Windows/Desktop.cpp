
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
   Includes
----------------------------------------- */

#include <windows.h>
#include <string>

#include <iostream>

#include <vector>


/* -----------------------------------------
   Functions
----------------------------------------- */

double fn_desktop_icons_hide()
{
	HWND desk = FindWindowEx(NULL, NULL, L"Progman", NULL); // desktop
	EnableWindow(desk, FALSE);                              // disable the desktop, use TRUE to undo
	ShowWindow(desk, FALSE);                                // hide the desktop use TRUE to undo
	return 1;
};

double fn_desktop_icons_show()
{
	HWND desk = FindWindowEx(NULL, NULL, L"Progman", NULL); // desktop
	EnableWindow(desk, TRUE);                               // disable the desktop, use TRUE to undo
	ShowWindow(desk, TRUE);                                 // hide the desktop use TRUE to undo
	return 1;
};

HWND fn_desktop_get_handle()
{
	HWND desk = FindWindowEx(NULL, NULL, L"Progman", NULL); //desktop
	return desk;
};

HWND fn_desktop_taskbar_get_handle()
{
	HWND taskhwnd = FindWindow(L"Shell_traywnd", NULL); //taskbar
	return taskhwnd;
};

HWND fn_desktop_startbutton_get_handle()
{
	HWND srthwnd = FindWindowEx(NULL, NULL, L"Button", NULL); //start button
	return srthwnd;
};


double fn_desktop_taskbar_hide()
{
	HWND taskhwnd = FindWindow(L"Shell_TrayWnd", NULL); //taskbar
	HWND startbt = FindWindowEx(NULL, NULL, L"Button", NULL); //start button

	EnableWindow(taskhwnd, FALSE); //disable the taskbar, use TRUE to undo
	ShowWindow(taskhwnd, FALSE); //hide the taskbar, use TRUE to undo
	EnableWindow(startbt, FALSE); //disable the start button, use TRUE to undo
	ShowWindow(startbt, FALSE); //hide the start button, use TRUE to undo

	return 1;
}

double fn_desktop_taskbar_show()
{
	HWND taskhwnd = FindWindow(L"Shell_TrayWnd", NULL); //taskbar
	HWND startbt  = FindWindowEx(NULL,NULL,L"Button", NULL); //start button

	EnableWindow(taskhwnd, TRUE); //disable the taskbar, use TRUE to undo
	ShowWindow(taskhwnd, TRUE); //hide the taskbar, use TRUE to undo
	EnableWindow(startbt, TRUE); //disable the start button, use TRUE to undo
	ShowWindow(startbt, TRUE); //hide the start button, use TRUE to undo

	return 1;
}

double fn_desktop_get_size()
{
	RECT wrect = {0, 0, 0, 0};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	std::cout << "Desktop X: " << wrect.left << std::endl;
	std::cout << "Desktop Y: " << wrect.top << std::endl;
	std::cout << "Desktop Width: " << wrect.right << std::endl;
	std::cout << "Desktop Height: " << wrect.bottom << std::endl;
	
	return 1;
};

double fn_desktop_taskbar_get_size()
{
	HWND taskhwnd = FindWindow(L"Shell_traywnd", NULL);
	RECT wrect = {0, 0, 0, 0};

    if (taskhwnd && GetWindowRect(taskhwnd, &wrect)) {
		std::cout << "Taskbar X: " << wrect.left << std::endl;
		std::cout << "Taskbar Y: " << wrect.top << std::endl;
		std::cout << "Taskbar Width: " << wrect.right << std::endl;
		std::cout << "Taskbar Height: " << wrect.bottom << std::endl;
    } else {
		std::cout << "Error when getting Taskbar dimensions" << std::endl;
	}
	
	return 1;
	
};

double fn_desktop_is_locked()
{

	wchar_t DesktopName[8] = L"default";

	HDESK hDesktop = OpenDesktop(DesktopName, 0, false, DESKTOP_SWITCHDESKTOP);
	int rtn = 0;
	
	if (hDesktop != 0) {
		rtn = SwitchDesktop(hDesktop);
		if (rtn == 0) {
			// Locked
			CloseDesktop(hDesktop);
			return 1;
		} else {
			// Not locked
			CloseDesktop(hDesktop);
			return 0;
		}
	} else {
		return 0;
	}

}

static std::string utf16ToUTF8( const std::wstring &s )
{
    const int size = ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL );

    std::vector<char> buf( size );
    ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL );

    return std::string( &buf[0] );
}

static std::wstring utf8ToUTF16(const std::string &utf8)
{
    // Special case of empty input string
	if (utf8.empty()) {
		return std::wstring();
	}

	// Get length (in wchar_t's) of resulting UTF-16 string
	const int utf16_length = ::MultiByteToWideChar(
		CP_UTF8,            // convert from UTF-8
		0,                  // default flags
		utf8.data(),        // source UTF-8 string
		utf8.length(),      // length (in chars) of source UTF-8 string
		NULL,               // unused - no conversion done in this step
		0                   // request size of destination buffer, in wchar_t's
	);

	if (utf16_length == 0) {
		// Error
		DWORD error = ::GetLastError();
		throw ;
	}


	// // Allocate properly sized destination buffer for UTF-16 string
	std::wstring utf16;
	utf16.resize(utf16_length);

	// // Do the actual conversion from UTF-8 to UTF-16
	if ( ! ::MultiByteToWideChar(
		CP_UTF8,            // convert from UTF-8
		0,                  // default flags
		utf8.data(),        // source UTF-8 string
		utf8.length(),      // length (in chars) of source UTF-8 string
		&utf16[0],          // destination buffer
		utf16.length()      // size of destination buffer, in wchar_t's
		) )
	{
		DWORD error = ::GetLastError();
		throw;
	}

	return utf16;
}

std::string fn_desktop_get_wallpaper()
{
	wchar_t path[MAX_PATH + 1] = {0};

	HRESULT retn = SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH+1, path, 0);
	
	if (retn) {
		return utf16ToUTF8(path);
	} else {
		return "";
	}
	
};

double fn_desktop_set_wallpaper(std::wstring fname, BOOL stretched, BOOL tiled)
{

	std::wstring file = fname; 
	std::wstring* filename = &file; // change path to pointer
	
	HRESULT retn = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)filename->c_str(), SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
	
	std::string WallPaperStyle = "0\0";
	std::string TileWallPaper = "0\0";
	
	if (stretched == true) { WallPaperStyle = "2\0"; }
	if (tiled == true) { WallPaperStyle = "1\0"; TileWallPaper = "1\0"; }
	
	// string to LPBYTE (unsigned char *)
	unsigned char *val1 = new unsigned char[WallPaperStyle.length()+1];
	strcpy((char *)val1, WallPaperStyle.c_str());
	
	unsigned char *val2 = new unsigned char[TileWallPaper.length()+1];
	strcpy((char *)val2, TileWallPaper.c_str());
	
	// update registry
	HKEY WallpaperSettings;
	RegOpenKeyEx(HKEY_CURRENT_USER, (LPCWSTR)L"Control Panel\\Desktop", 0, KEY_ALL_ACCESS, &WallpaperSettings);
		RegSetValueEx(WallpaperSettings, (LPCWSTR)L"WallpaperStyle", 0, REG_SZ, (LPBYTE)val1, 2); //0=center, 1=tiled, 2=stretch, 
		RegSetValueEx(WallpaperSettings, (LPCWSTR)L"TileWallpaper", 0, REG_SZ, (LPBYTE)val2, 2); // 0=notile, 1=tiled
	RegCloseKey(WallpaperSettings); 
	
	if (retn) {
		// refresh background
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_UPDATEINIFILE); 
		return 1;
	} else {
		return 0;
	}
}

/* -----------------------------------------
   Application
----------------------------------------- */

int main(void) {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	fn_desktop_get_size();
	std::cin.get();
	
	fn_desktop_taskbar_get_size();
	std::cin.get();
	
	std::cout << "Get Desktop Wallpaper" << std::endl;
	std::cout << fn_desktop_get_wallpaper() << std::endl;
	std::cin.get();
	
	std::cout << "Set Desktop Wallpaper" << std::endl;
	//fn_desktop_set_wallpaper(L"C:\\wallpaper.jpg", true, false);
	std::cin.get();
	
	std::cout << "Hide Desktop Icons" << std::endl;
	fn_desktop_icons_hide();
	std::cin.get();
	std::cout << "Show Desktop Icons" << std::endl;
	fn_desktop_icons_show();
	std::cin.get();
	
	std::cout << "Hide Taskbar" << std::endl;
	fn_desktop_taskbar_hide();
	std::cin.get();
	std::cout << "Show Taskbar" << std::endl;
	fn_desktop_taskbar_show();
	std::cin.get();
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	
	return 0;
}


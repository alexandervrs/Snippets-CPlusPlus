
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
   Definitions
----------------------------------------- */

#define _WIN32_WINNT 0x0500
#define _WIN32_IE 0x0500

#define CSIDL_MYMUSIC 0x000D
#define CSIDL_MYVIDEO 0x000E


/* -----------------------------------------
   Includes
----------------------------------------- */

#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

#include <iostream>


/* -----------------------------------------
   Functions
----------------------------------------- */

char special_directory_buffer[(MAX_PATH*4)+1];

const char* fn_directory_get_special()
{
    wchar_t path[MAX_PATH+1] = {0};

	/*
	
		Special Directory Constants
	
		CSIDL_COMMON_DESKTOPDIRECTORY // All Users Desktop
		CSIDL_COMMON_DOCUMENTS        // All Users Documents
		CSIDL_COMMON_MUSIC            // All Users Music
		CSIDL_COMMON_PICTURES         // All Users Pictures
		CSIDL_COMMON_VIDEO            // All Users Video
		CSIDL_COMMON_PROGRAMS         // All Users Programs
		CSIDL_COMMON_TEMPLATES        // All Users Templates
		CSIDL_COMMON_STARTUP          // All Users Startup
		CSIDL_COMMON_STARTMENU        // All users Start Menu
		
		CSIDL_DESKTOPDIRECTORY        // User Desktop
		CSIDL_APPDATA                 // User Roaming AppData
		CSIDL_LOCAL_APPDATA           // User Local AppData
		CSIDL_MYDOCUMENTS             // User Documents
		CSIDL_MYMUSIC                 // User Music
		CSIDL_MYPICTURES              // User Pictures
		CSIDL_MYVIDEO                 // User Video
		CSIDL_PROFILE                 // User Profile
		CSIDL_SENDTO                  // User Send To
		CSIDL_STARTMENU               // User Start Menu
		CSIDL_STARTUP                 // User Start Up
		CSIDL_TEMPLATES               // User Document Templates
		
		CSIDL_FONTS                   // Fonts
		CSIDL_RESOURCES               // Resources
		CSIDL_PROGRAM_FILES           // Program Files
		CSIDL_PROGRAM_FILESX86        // Program Files x86
		CSIDL_SYSTEM                  // System 32
		CSIDL_WINDOWS                 // Windows
	
	*/
	
    if (SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path) != S_OK)
    {
        //MessageBoxW(NULL, L"ERROR in SHGetFolderPathW", L"TEST", MB_OK);
        return NULL;
    }

	wcscat(path, L"\\"); // trailing slash
	
    //MessageBoxW(NULL, path, L"Special Directory", MB_OK);

    int buflen = WideCharToMultiByte(CP_UTF8, 0, path, lstrlenW(path), special_directory_buffer, MAX_PATH*4, NULL, NULL);
    if (buflen <= 0)
    {
        //MessageBoxW(NULL, L"ERROR in WideCharToMultiByte", L"TEST", MB_OK);
        return NULL;
    }

    special_directory_buffer[buflen] = 0;

    return special_directory_buffer;
}

const char* fn_directory_get_temp()
{
    wchar_t path[MAX_PATH+1] = {0};

	if (!GetTempPathW(MAX_PATH, path)) {
		
		MessageBoxW(NULL, L"ERROR in GetTempPathW", L"TEST", MB_OK);
        return NULL;
		
	}
	
	int buflen = WideCharToMultiByte(CP_UTF8, 0, path, lstrlenW(path), special_directory_buffer, MAX_PATH*4, NULL, NULL);
    if (buflen <= 0)
    {
        //MessageBoxW(NULL, L"ERROR in WideCharToMultiByte", L"TEST", MB_OK);
        return NULL;
    }

    special_directory_buffer[buflen] = 0;

    return special_directory_buffer;
	
}

const char* fn_directory_get_working()
{
    wchar_t path[MAX_PATH+1] = {0};

	if (!GetCurrentDirectoryW(MAX_PATH, path)) {
		
		MessageBoxW(NULL, L"ERROR in GetCurrentDirectoryW", L"TEST", MB_OK);
        return NULL;
		
	}
	
	wcscat(path, L"\\"); // trailing slash
	
	int buflen = WideCharToMultiByte(CP_UTF8, 0, path, lstrlenW(path), special_directory_buffer, MAX_PATH*4, NULL, NULL);
    if (buflen <= 0)
    {
        //MessageBoxW(NULL, L"ERROR in WideCharToMultiByte", L"TEST", MB_OK);
        return NULL;
    }

    special_directory_buffer[buflen] = 0;

    return special_directory_buffer;
	
}

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	const char* _out = fn_directory_get_special();
	std::cout << "Desktop directory is: " << _out << std::endl;
	
	const char* _out2 = fn_directory_get_temp();
	std::cout << "Temp directory is: " << _out2 << std::endl;
	
	const char* _out3 = fn_directory_get_working();
	std::cout << "Working directory is: " << _out3 << std::endl;
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}


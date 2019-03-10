
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

static std::string fn_string_utf16_to_utf8( const std::wstring &utf16_string )
{

	if (utf16_string.empty()) {
		return std::string();
	}

    const int utf8_length = WideCharToMultiByte(CP_UTF8, 0, utf16_string.data(), -1, NULL, 0, 0, NULL);

	if (utf8_length == 0) {
		DWORD error = GetLastError();
		throw;
	}
	
	std::string utf8_string;
	utf8_string.resize(utf8_length);
	
    if (!WideCharToMultiByte(CP_UTF8, 0, utf16_string.data(), -1, &utf8_string[0], utf8_string.length(), 0, NULL)) {
		DWORD error = GetLastError();
		throw;		
	}

    return std::string(&utf8_string[0]);
};

static std::wstring fn_string_utf8_to_utf16( const std::string &utf8_string )
{
	if (utf8_string.empty()) {
		return std::wstring();
	}

	const int utf16_length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.data(), utf8_string.length(), NULL, 0);
	
	if (utf16_length == 0) {
		DWORD error = GetLastError();
		throw;
	}
	
	std::wstring utf16_string;
	utf16_string.resize(utf16_length);

	if (!MultiByteToWideChar(CP_UTF8, 0, utf8_string.data(), utf8_string.length(), &utf16_string[0], utf16_string.length())) {
		DWORD error = GetLastError();
		throw;
	}
	
	return utf16_string;
};

const char* fn_system_get_workstation_name()
{

	std::wstring hostname;
    wchar_t nbtName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD length = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(nbtName, &length);
    hostname = nbtName;
	
	std::string utf8String = fn_string_utf16_to_utf8( hostname );
	const char* outstring = utf8String.c_str();
	
	return outstring;

};

const char* fn_system_get_username()
{

	std::wstring username;
	
    wchar_t nbtName[32767];
    DWORD length = 32767;
    GetUserName(nbtName, &length);
    username = nbtName;
	
	std::string utf8String = fn_string_utf16_to_utf8( username );
	const char* outstring = utf8String.c_str();
	
	return outstring;

};

double fn_system_has_mouse()
{
	bool rtn = GetSystemMetrics(SM_MOUSEPRESENT);
	if (rtn) {
		return 1;
	} else {
		return 0;
	}
};

double fn_system_is_mouse_inverted()
{
	bool rtn = GetSystemMetrics(SM_SWAPBUTTON);
	if (rtn) {
		return 1;
	} else {
		return 0;
	}	
};

double fn_system_get_mouse_buttons()
{
	int rtn = GetSystemMetrics(SM_CMOUSEBUTTONS);
	if (!rtn) {
		return 0;
	} else {
		return (double)rtn;
	}	
};

double fn_system_has_mouse_wheel()
{
	bool rtn = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
	if (rtn) {
		return 1;
	} else {
		return 0;
	}	
};


/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	std::cout << "PC Name: " << fn_system_get_workstation_name() << std::endl;
	std::cout << "Username: " << fn_system_get_username() << std::endl;
	std::cout << "Has Mouse: " << fn_system_has_mouse() << std::endl;
	std::cout << "Mouse Click Inverted: " << fn_system_is_mouse_inverted() << std::endl;
	std::cout << "Mouse Buttons: " << fn_system_get_mouse_buttons() << std::endl;
	std::cout << "Has Mouse Wheel: " << fn_system_has_mouse_wheel() << std::endl;
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}


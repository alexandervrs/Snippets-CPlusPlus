
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


/* -----------------------------------------
   Functions
----------------------------------------- */

BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));

};

// DLL handle
HINSTANCE dll = NULL;

// external function: ShowMessageBox()
// define the external function type
typedef int (__cdecl* FUNCTION_MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT); // returnType (callingConvention name)(arguments...)
FUNCTION_MESSAGEBOXA ShowMessageBox = NULL; // Set new function ShowMessageBox() to be of the above type and use it as function name
// ...

double fn_dll_load() {
	
	// for local DLL
	//if (!FileExists(L"User32.dll")) {
	//	dll = NULL;
	//	return 0;
	//}
	
	dll = LoadLibrary( L"User32.dll" );
	
	if (!dll) {
		dll = NULL;
		return 0;
	}
	
	// get procaddress from DLL
	ShowMessageBox = (FUNCTION_MESSAGEBOXA)( GetProcAddress(dll, "MessageBoxA") );
	
	return 1;
	
};

double fn_dll_call() {
	
	if (!dll) {
		return 0;
	}
	
	if (!ShowMessageBox) {
		return 0;
	}
	
	// call the external function
	int result = ShowMessageBox( NULL, "Test MessageBoxA", "Hello", MB_OK );
	//(*ShowMessageBox)( NULL, "Test MessageBoxA", "Hello", MB_OK );
	
	// function result
	//std::cout << "Function result: " << result << std::endl;
	
	return 1;
};	

double fn_dll_free() {
	
	if (!dll) {
		dll = NULL;
		return 0;
	}
	
	FreeLibrary(dll);
	dll = NULL;
	
	return 1;
};

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	std::cout << "DLL Loaded: " << fn_dll_load() << std::endl;
	std::cout << "DLL Called: " << fn_dll_call() << std::endl;
	std::cin.get();
	std::cout << "DLL Unloaded: " << fn_dll_free() << std::endl;
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}


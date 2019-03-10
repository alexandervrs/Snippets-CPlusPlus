
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

#include <iostream>

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	
	// note: NULL is hWnd
	MessageBox(NULL, L"Message", L"Title", MB_OK | MB_ICONINFORMATION);
	
	int msgbox_result = MessageBox(NULL, L"Message?", L"Title", MB_YESNOCANCEL | MB_ICONQUESTION); // MB_YESNO
	switch (msgbox_result) {
		case IDCANCEL:
			std::cout << "Cancel button pressed" << std::endl;
			break;
		case IDYES:
			std::cout << "Yes button pressed" << std::endl;
			break;
		case IDNO:
			std::cout << "No button pressed" << std::endl;
			break;
	}
	
	MessageBox(NULL, L"Message", L"Title", MB_OK | MB_ICONWARNING | MB_HELP); // Help button sends WM_HELP message
	
	MessageBox(NULL, L"Message", L"Error", MB_OK | MB_ICONERROR);
	
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}



/* -----------------------------------------
   Libraries
----------------------------------------- */

// -lcomdlg32


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
   Functions
----------------------------------------- */

double fn_color_choose(double default_color) {
	
	CHOOSECOLOR color;                       // struct for the dialog
	//COLORREF colors_default[16];           // custom colors
	
	COLORREF colors_default[16] = { 
		RGB(0,     5,   5),
		RGB(0,    15,  55),
		RGB(0,    25, 155),
		RGB(0,    35, 255),
		RGB(10,    0,   5),
		RGB(10,   20,  55),
		RGB(10,   40, 155),
		RGB(10,   60, 255),
		RGB(100,   5,   5),
		RGB(100,  25,  55),
		RGB(100,  50, 155),
		RGB(100, 125, 255),
		RGB(200, 120,   5),
		RGB(200, 150,  55),
		RGB(200, 200, 155),
		RGB(200, 250, 255)
	};
	
	COLORREF selected_color = default_color; // default selected color
 
	memset(&color, 0, sizeof(color)); // ZeroMemory(&color, sizeof(color));
	color.lStructSize  = sizeof(color);
	color.hwndOwner    = GetActiveWindow(); // hWnd
	color.lpCustColors = (LPDWORD)colors_default;
	color.rgbResult    = (int)selected_color;
	color.Flags        = CC_RGBINIT | CC_FULLOPEN;
 
	if (ChooseColor(&color) == TRUE)
	{
		return color.rgbResult;
	} else {
		return default_color;	
	}

}

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	DWORD result = fn_color_choose(563728);
	// application run
	std::cout << "Color selected (RGB): " << result << std::endl;
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}



/* -----------------------------------------
   Libraries
----------------------------------------- */

// -lgdi32 // for CreateSolidBrush()


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

static std::string utf16ToUTF8( const std::wstring &s )
{
    const int size = ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL );

    std::vector<char> buf( size );
    ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL );

    return std::string( &buf[0] );
};

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
};


double fn_window_show(HWND window_handle)
{
	ShowWindow(window_handle, SW_SHOW);
	SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	return 1;
};

double fn_window_hide(HWND window_handle)
{
	ShowWindow(window_handle, SW_HIDE);
	return 1;
};

int fn_window_get_width(HWND window_handle)
{
	RECT rect;

	if (GetClientRect(window_handle, &rect)) {
		return (rect.right - rect.left);
	}
	
};

int fn_window_get_height(HWND window_handle)
{
	RECT rect;

	if (GetClientRect(window_handle, &rect)) {
		return (rect.bottom - rect.top);
	}

};

bool fn_window_center(HWND window_handle) {
	
	RECT _rect;

	GetWindowRect(window_handle, &_rect);

	int _width = (_rect.right - _rect.left);
	int _height = (_rect.bottom - _rect.top);

	int _x = ((1920 - _width)  / 2); // note: 1920 is displayWidth, need to get dynamically
	int _y = ((1080 - _height) / 2); // note: 1080 is displayHeight, need to get dynamically

	if (MoveWindow(window_handle, _x, _y, _width, _height, true)) {
		return true;
	} else {
		return false;
	}
	
};

double fn_window_maximize(HWND window_handle)
{
	SendMessage(window_handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	return 1;
};

double fn_window_restore(HWND window_handle)
{
	SendMessage(window_handle, WM_SYSCOMMAND, SC_RESTORE, 0);
	SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	return 1;
};

double fn_window_minimize(HWND window_handle)
{
	CloseWindow(window_handle);
	SendMessage(window_handle, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return 1;
};

bool fn_window_set_on_top(HWND window_handle, bool state = true)
{
	if (state == true) {
		if (SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_ASYNCWINDOWPOS)) {
			return true;
		} else {
			return false;
		}
	} else {
		if (SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_ASYNCWINDOWPOS)) {
			return true;
		} else {
			return false;
		}
	}
};

bool fn_window_set_size(HWND window_handle, int width, int height)
{
	RECT rect;
	GetWindowRect(window_handle, &rect);
	
	if (MoveWindow(window_handle, rect.left, rect.top, width, height, true)) {
		return true;
	} else {
		return false;
	}
};

bool fn_window_set_position(HWND window_handle, int x, int y)
{
	RECT rect;
	GetWindowRect(window_handle, &rect);
	
	if (MoveWindow(window_handle, x, y, rect.right - rect.left, rect.bottom - rect.top, true)) {
		return true;
	} else {
		return false;
	}
};

void fn_window_set_border(HWND window_handle, bool state = true)
{
	
	if (state == false) {
		LONG lStyle = GetWindowLong(window_handle, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
		SetWindowLongPtr(window_handle, GWL_STYLE, lStyle);
		
		LONG lExStyle = GetWindowLong(window_handle, GWL_EXSTYLE);
		lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		SetWindowLongPtr(window_handle, GWL_EXSTYLE, lExStyle);
		
		SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		
	} else {
		LONG lStyle = GetWindowLong(window_handle, GWL_STYLE);
		lStyle |= (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
		SetWindowLongPtr(window_handle, GWL_STYLE, lStyle);
		
		LONG lExStyle = GetWindowLong(window_handle, GWL_EXSTYLE);
		lExStyle |= (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		SetWindowLongPtr(window_handle, GWL_EXSTYLE, lExStyle);
		
		SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);	
	}

};

int fn_window_get_focus(HWND window_handle)
{
	if (GetFocus() == window_handle) {
		return 1;
	} else {
		return 0;
	}   
}

void fn_window_set_title(HWND window_handle, std::string title = "") {
	SetWindowText(window_handle, utf8ToUTF16(title).c_str());
}

const char* fn_window_get_title(HWND window_handle) {
	
	std::wstring utitle;
	int length = 1024;
	wchar_t get_title[length];
	
	GetWindowText(window_handle, get_title, length);
	
	// wide (LPWSTR) to const char* (LPCSTR)
	utitle = get_title;
	std::string utf8String = utf16ToUTF8( utitle );
	const char* lpcStr = utf8String.c_str();
	
	return lpcStr;

}

/* -----------------------------------------
   Application
----------------------------------------- */

// window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
			
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		break;
		case WM_ENTERSIZEMOVE: case WM_MOVE: case WM_EXITSIZEMOVE:
			
			// refresh window while and before/after moving
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
			
		break;
		case WM_ACTIVATEAPP:
		
			std::cout << "Window activated" << std::endl;
			// refresh window after activating
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);	
			
		break;
		case WM_NCACTIVATE:
			return true;
		break;
		case WM_SETFOCUS:
			std::cout << "Window has focus" << std::endl << std::endl;
		break;
		
		case WM_KILLFOCUS:
			std::cout << "Window lost focus" << std::endl << std::endl;
		break;
	}
	
	return DefWindowProc (hWnd, message, wParam, lParam);
};

// main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	// create window class
	HWND hWnd;
    WNDCLASSEX window_class;

    ZeroMemory(&window_class, sizeof(WNDCLASSEX));

    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = hInstance;
	window_class.lpszMenuName = NULL;
	window_class.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hIcon = LoadIcon(hInstance, IDI_APPLICATION); //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RCICON));
    window_class.lpszClassName = L"WindowClassName";

    RegisterClassEx(&window_class);

	// create window
	DWORD window_style = WS_OVERLAPPEDWINDOW;
	hWnd = CreateWindowEx(0, L"WindowClassName", L"Window Title", window_style, 0, 0, 1280, 720, NULL, NULL, hInstance, NULL);
	
	// set window options
	LONG _winstyle;
	_winstyle = GetWindowLongPtr(hWnd, GWL_STYLE);
	
	// remove maximize and resize window feature
	_winstyle &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX | WS_THICKFRAME);
	
	// borderless
	//_winstyle &= ~(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_THICKFRAME);
	
	SetWindowLongPtr(hWnd, GWL_STYLE, _winstyle);
	
	// show window
	ShowWindow(hWnd, nCmdShow);
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// test functions
	std::cout << "Window Center" << std::endl; fn_window_center(hWnd);
	//std::cout << "Window Maximize" << std::endl; fn_window_maximize(hWnd);
	//std::cout << "Window Restore" << std::endl; fn_window_restore(hWnd);
	//std::cout << "Window Minimize" << std::endl; fn_window_minimize(hWnd);
	//std::cout << "Window Set On Top" << std::endl; fn_window_set_on_top(hWnd, true);
	//std::cout << "Window Set Size" << std::endl; fn_window_set_size(hWnd, 800, 600);
	//std::cout << "Window Set Position" << std::endl; fn_window_set_position(hWnd, 32, 32);
	//std::cout << "Window Set Border" << std::endl; fn_window_set_border(hWnd, false);
	//std::cout << "Window Set Border" << std::endl; fn_window_set_border(hWnd, true);
	//std::cout << "Window Hide" << std::endl; fn_window_hide(hWnd);
	//std::cout << "Window Show" << std::endl; fn_window_show(hWnd);
	//std::cout << "Window Set Title" << std::endl; fn_window_set_title(hWnd, "Testing Τέστ");
	//std::cout << "Window Get Title: " << fn_window_get_title(hWnd) << std::endl;
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	
	
	// loop
	MSG msg;
	while(TRUE) {
		
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) {
			break;
		}
	
	}
	return msg.wParam;

}





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
#include <string>
#include <iostream>

#include <shlobj.h>
#include <sstream>

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

std::string fn_file_browse_open(HWND window_handle, std::string title = "File Open", std::string filter = "All Files (*.*)|*.*", std::string initial_directory = "")
{
    OPENFILENAME ofn = {0};
    wchar_t szFile[MAX_PATH+1] = {0};

	std::string filters = filter;
	filters.append("|");
	//std::cout << filters << std:endl;
	int len = filters.length();
	for (int i=0; i<(len); ++i) {
		
		if (filters[i] == '|') {
			filters[i] = '\0';
		}
		
	}
	//std::cout << filters << std:endl;
	std::wstring filtersw = fn_string_utf8_to_utf16(filters);
	
	
	std::wstring titlew   = fn_string_utf8_to_utf16(title);
	std::wstring initial_directoryw = fn_string_utf8_to_utf16(initial_directory);
	
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = window_handle;
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrFilter     = filtersw.c_str();
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = NULL;
    ofn.nMaxFileTitle   = 0;
	ofn.lpstrTitle      = titlew.c_str();
    ofn.lpstrInitialDir = initial_directoryw.c_str();
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    GetOpenFileName(&ofn);
	
	return fn_string_utf16_to_utf8(szFile).c_str();
    
};

std::string fn_file_browse_save(HWND window_handle, std::string title = "File Save As", std::string filter = "All Files (*.*)|*.*", std::string filename_default = "", std::string fileext_default = "", std::string initial_directory = "")
{
    OPENFILENAME ofn = {0};
	
	std::wstring filename_defaultw;
	filename_defaultw = fn_string_utf8_to_utf16(filename_default);
	filename_defaultw.resize(MAX_PATH +1);
	
	if (filename_defaultw.length() > MAX_PATH+1) {
		return "";
	}
	
	std::string filters = filter;
	filters.append("|");
	//std::cout << filters << std:endl;
	int len = filters.length();
	for (int i=0; i<(len); ++i) {
		
		if (filters[i] == '|') {
			filters[i] = '\0';
		}
		
	}
	std::wstring filtersw = fn_string_utf8_to_utf16(filters);

	std::wstring titlew             = fn_string_utf8_to_utf16(title);
	std::wstring fileext_defaultw   = fn_string_utf8_to_utf16(fileext_default);
	std::wstring initial_directoryw = fn_string_utf8_to_utf16(initial_directory);
	
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = window_handle;
    ofn.lpstrFile       = (LPWSTR)filename_defaultw.c_str();
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrFilter     = filtersw.c_str();
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = NULL;
    ofn.nMaxFileTitle   = 0;
	ofn.lpstrTitle      = titlew.c_str();
    ofn.lpstrInitialDir = initial_directoryw.c_str();
    ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt     = fileext_defaultw.c_str();

    GetSaveFileName(&ofn);
	
	return fn_string_utf16_to_utf8(filename_defaultw).c_str();
	
};


std::string fn_directory_browse(HWND window_handle, std::string title = "")
{
	wchar_t path[MAX_PATH];

    //const char * path_param = saved_path.c_str();

	std::wstring titlew = fn_string_utf8_to_utf16(title);
	
    BROWSEINFO bi = { 0 };
	bi.hwndOwner  = window_handle;
    bi.lpszTitle  = titlew.c_str();
    bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
   // bi.lParam     = NULL;//(LPARAM) path_param;

    LPITEMIDLIST pidl = SHBrowseForFolder( &bi );

    if ( pidl != 0 )
    {
        //get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, path );

        //free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
		
        return fn_string_utf16_to_utf8(path).append("\\");
    }

    return "";
};

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	std::string audio_filter = "Audio Files (*.wav, *mp3, *.ogg)|*.wav;*.mp3;*.ogg|WAV Files (*.wav)|*.wav|MP3 Files (*.mp3)|*.mp3|OGG Files (*.ogg)|*.ogg|All Files (*.*)|*.*";
	
	std::cout << "File Open As: " << std::endl;
	std::string audioFileOpen = fn_file_browse_open(NULL, "Open audio files", audio_filter);
	
	if (audioFileOpen.length() > 0) {
		std::cout << audioFileOpen << std::endl;
	} else {
		std::cout << "No file to open..." << std::endl;
	}
	
	std::cout << std::endl << "File Save As: " << std::endl;
	std::string audioFileSave = fn_file_browse_save(NULL, "Save as audio file", audio_filter, "myfile.wav", "wav");
	
	if (audioFileSave.length() > 0) {
		std::cout << audioFileSave << std::endl;
	} else {
		std::cout << "No file to save..." << std::endl;
	}

	std::cout << std::endl << "Directory Browse: " << std::endl;
	std::string folder = fn_directory_browse(NULL, "Select a folder");
	std::cout << folder << std::endl;

	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}


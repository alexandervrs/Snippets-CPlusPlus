
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
#include <climits>

/* -----------------------------------------
   Functions
----------------------------------------- */

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


bool fn_file_exists(const std::string file)
{
	DWORD attr = GetFileAttributes(fn_string_utf8_to_utf16(file).c_str());
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
	} else {
		return true;
	}
};

bool fn_file_write(const std::string file, const std::string contents)
{
	HANDLE hfile = CreateFile(
       fn_string_utf8_to_utf16(file).c_str(),
       GENERIC_READ | GENERIC_WRITE, 
       0,
       NULL,
       CREATE_ALWAYS,
       FILE_ATTRIBUTE_NORMAL,
       NULL);
		
	if (hfile == INVALID_HANDLE_VALUE) {
		return false;
	}
	
	//DWORD bytesWrittenMarker;

	//unsigned char marker[3];

	//marker[0] = 0xEF;
	//marker[1] = 0xBB;
	//marker[2] = 0xBF;
	//WriteFile(hfile, marker, 3, &bytesWrittenMarker, NULL);
	
	DWORD bytesWrittenContent;
	
	if (WriteFile(hfile, contents.c_str(), contents.length(), &bytesWrittenContent, NULL)) {
		//std::cout << bytesWrittenContent;
		CloseHandle(hfile);
		return true;
	} else {
		CloseHandle(hfile);
		return false;
	}

};

std::string fn_file_read(const std::string file)
{
	
	HANDLE hfile = CreateFile(
       fn_string_utf8_to_utf16(file).c_str(),
       GENERIC_READ, 
       0,
       NULL,
       OPEN_EXISTING,
       FILE_ATTRIBUTE_NORMAL,
       NULL
	);
		
	if (hfile == INVALID_HANDLE_VALUE) {
		return "";
	}

    char buffer_read[32767];
	DWORD bytes_read = 0;
	
	SetFilePointer(hfile, 0, NULL, FILE_BEGIN);
	
	ReadFile(hfile, &buffer_read, 32767, &bytes_read, NULL);
	
	return buffer_read;

};

bool fn_file_delete(const std::string file)
{
	if (DeleteFile(fn_string_utf8_to_utf16(file).c_str())) {
		return true;
	} else {
		return false;
	}
};

bool fn_directory_create(const std::string path)
{
	if (CreateDirectory(fn_string_utf8_to_utf16(path).c_str(), NULL)) {
		return true;
	} else {
		return false;
	}
};

bool fn_directory_rename(const std::string path, const std::string newpath)
{
	DWORD attr = GetFileAttributes(fn_string_utf8_to_utf16(path).c_str());
	if ((attr & !FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
	}
	
	if (MoveFile(fn_string_utf8_to_utf16(path).c_str(), fn_string_utf8_to_utf16(newpath).c_str())) {
		return true;
	} else {
		return false;
	}
};

bool fn_directory_delete(const std::string path)
{
	if (RemoveDirectory(fn_string_utf8_to_utf16(path).c_str())) {
		return true;
	} else {
		return false;
	}
};

bool fn_directory_exists(const std::string path)
{
	DWORD attr = GetFileAttributes(fn_string_utf8_to_utf16(path).c_str());
	if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		if (attr == INVALID_FILE_ATTRIBUTES) {
			return false;
		} else {
			return true;
		}
	}
};

/* -----------------------------------------
   Application
----------------------------------------- */

int main() {
	
	// application start
	std::cout << "--------------/ Start \\-----------------" << std::endl << std::endl;
	
	// application run
	std::cout << "File Exists (C:\\test.txt): " << fn_file_exists("C:\\test.txt") << std::endl;
	std::cout << "Directory Exists (C:\\test\\): " << fn_directory_exists("C:\\test\\") << std::endl;
	
	std::cout << "File Write (C:\\test.txt): " << fn_file_write("C:\\test.txt", "Rδερ Boom Καλημέρα\nhello \nworld!!\n\0") << std::endl;
	std::cout << "File Read (C:\\test.txt): " << "\"" << fn_file_read("C:\\test.txt") << "\"" << std::endl;
	
	std::string cc = fn_file_read("C:\\test.txt");
	std::cout << "File Write: " << fn_file_write("C:\\testX.txt", cc) << std::endl;
	
	//std::cout << "File Delete (C:\\test.txt): " << fn_file_delete("C:\\test.txt") << std::endl;
	
	std::cout << "Directory Create: " << fn_directory_create("C:\\test\\") << std::endl;
	std::cout << "Directory Rename: " << fn_directory_rename("C:\\test\\", "C:\\test2\\") << std::endl;
	std::cout << "Directory Delete: " << fn_directory_delete("C:\\test2\\") << std::endl;
	
	// application end
	std::cout << std::endl << "--------------/ Finish \\-----------------" << std::endl;
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return 0;
}


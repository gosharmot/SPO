#include <iostream>
#include <windows.h>

int main() {
	HANDLE hFile;
	char charBuffer[] = "Chars yopt ШУЕ";
	DWORD dwBytesToWrite = (DWORD)strlen(charBuffer);
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = false;

	hFile = CreateFile(TEXT("char.txt"),
		FILE_APPEND_DATA,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile = INVALID_HANDLE_VALUE) {
		std::cout << "Unable open " << charBuffer << " file" << std::endl;
		return 0;
	}

	bErrorFlag = WriteFile(hFile,
		charBuffer,
		dwBytesToWrite,
		&dwBytesWritten,
		NULL);

	if (bErrorFlag == false) {
		std::cout << "Unable write to " << charBuffer << std::endl;
	}

	CloseHandle(hFile);
}
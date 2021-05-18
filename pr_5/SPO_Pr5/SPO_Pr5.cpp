#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <locale.h>
#include <iostream>

using namespace std;
//HANDLE hDisk = NULL; //описатель файла
BYTE bootBuff[512]; //буфер для бут-сектора
//значения параметров бут-сектора вынесены в глобальные переменные для использования их в функциях работы с кластерами
UINT nCopyFat, nRes, nSecPerClus;
DWORD nFatSectors, nRootDir, nFirstDataSector;
BYTE* fat;
string drive_path = "\\\\.\\E:";

//чтение сектора
bool readSector(DWORD num, BYTE* lpBuf) {
	//два параметра номер сектора и куда читаем 
	__int64 pos = (__int64)num * 512; //позиция в файле требует очень большого числа
	LARGE_INTEGER li;
	li.QuadPart = pos;
	//SetFilePointer(hDisk,   // handle of file 
	//	li.LowPart,         // number of bytes to move file pointer 
	//	&li.HighPart,       // address of high-order word of distance to move  
	//	FILE_BEGIN          // how to move 
	//);
	DWORD nCount;
	/*ReadFile(hDisk, lpBuf, 512, &nCount, NULL);*/

	ifstream infile(drive_path.c_str(), ios::in | ios::binary);
	if (!infile) {
		cout << "Device input error" << endl;
	}

	infile.ignore(pos);

	infile.read((char*)lpBuf, 512);

	if (infile.gcount() == 0) {
		cout << "Data read error" << endl;
	}

	//if (nCount != 512) {
	//	return false;
	//}
	return true;
}

//чтение некоторого числа байт с массива
void getBytes(BYTE* arr, const int amount = 1) {
	//принимаем место в массиве и кол-во символов на чтение
	//ПОКА НЕТ//читаем наоборот, тк биты записаны наоборот
	printf("%X", *arr);
	for (int i = 1; i < amount; i++) {
		printf(" %X", *(arr + i));
	}

}

void main() {
	setlocale(LC_ALL, ""); // установим русскую кодировку
	// откроем файл как физическое устройство
	// чтобы открыть дискету, необходимо в вписать \\\\.\\A:
	
	//hDisk = CreateFile(L"\\\\.\\D:", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, 0);

	/*if (hDisk == INVALID_HANDLE_VALUE) {
		std::cout << "Error" << GetLastError();
		return;
	}*/

	//считать в буфер бут-сектор
	//в 22 ячейке FAT хранится размер FAT в секторах, но в FAT32 там 0, так как для FAT32 размер FAT находится по смещению 36
	readSector(0, bootBuff); //1. читаем первый сектор - это MBR - в буфер 
	if (*(bootBuff + 22) != 0) {
		std::cout << "Диск не FAT32.\n";
		return;
	}

	//2. выводим расшифрованную таблицу бут сектора
	std::cout << "Таблица BIOS Parameter Block(BPB)";
	std::cout << "\nКоманда перехода JMP на начало загрузчика	"; getBytes(bootBuff, 3);
	std::cout << "\nТекстовый идентификатор версии ОС	"; getBytes(bootBuff + 0x3, 8);
	std::cout << "\nБайт в секторе	"; getBytes(bootBuff + 0xB, 2);
	std::cout << "\nЧисло секторов в кластере	"; getBytes(bootBuff + 0xD);
	std::cout << "\nЧисло резервных секторов	"; getBytes(bootBuff + 0xE, 2);
	std::cout << "\nЧисло таблиц FAT	"; getBytes(bootBuff + 0x10);
	std::cout << "\nдля FAT32=0	"; getBytes(bootBuff + 0x11, 2);
	std::cout << "\nОбщее число секторов в разделе	"; getBytes(bootBuff + 0x13, 2);
	std::cout << "\nТип носителя	"; getBytes(bootBuff + 0x15);
	std::cout << "\nДля FAT32 имеет значение 0	"; getBytes(bootBuff + 0x16, 2);
	std::cout << "\nЧисло секторов на дорожке	"; getBytes(bootBuff + 0x18, 2);
	std::cout << "\nЧисло рабочих поверхностей	"; getBytes(bootBuff + 0x1A, 2);
	std::cout << "\nЧисло скрытых секторов перед разделом	";  getBytes(bootBuff + 0x1C, 4);
	std::cout << "\nОбщее число секторов в разделе.	";  getBytes(bootBuff + 0x20, 4);
	std::cout << "\nКоличество секторов занимаемых одной FAT	";  getBytes(bootBuff + 0x24, 4);
	std::cout << "\nНомер первого кластера корневого каталога	";  getBytes(bootBuff + 0x2C, 4);
	std::cout << "\nИмя файловой системы	";  getBytes(bootBuff + 0x52, 8);
	std::cout << "\nСигнатура 55AAh – конец загрузочного сектора	";  getBytes(bootBuff + 0x1FE, 2);

	//определяет положение и размер системных данных FAT, ROOT, Boot
	int bootSize = (*(bootBuff + 0xE + 1)) * 256 + (int)*(bootBuff + 0xE);
	std::cout << "\n------------------------\nРазмер загрузочного диска, байт " << bootSize * 512;
	std::cout << "\nFAT начинается на смещении "; printf("%X", bootSize);
	int fatAmount = *(bootBuff + 0x10);
	int sectorSize = *(bootBuff + 0xC) * 256 + *(bootBuff + 0xB);
	UINT sectorsInFat = *(bootBuff + 0x27) * 256 * 256 * 256 + *(bootBuff + 0x26) * 256 * 256 + *(bootBuff + 0x25) * 256 + *(bootBuff + 0x24);
	std::cout << "\nРазмер FAT, байт " << sectorSize * sectorsInFat;
	std::cout << "\nАдрес корневого каталога	"; printf("%X", bootSize * 512 + (sectorsInFat * sectorSize * fatAmount));
	//Адрес области данных вычисляется: размер загрузочного сектора + таблицы FAT
	//Адрес корневого каталога вычисляется : (номер первого кластера корневого каталога — 2) * размер кластера + адрес начала области данных
	//CloseHandle(hDisk);
}

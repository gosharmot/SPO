#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "Resource.h"
LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
WCHAR szWinName[255];
//WCHAR szWinNameChildOne[255];
//WCHAR szWinNameChildTwo[255];
WCHAR szTitle[255];
WCHAR szTitleChildOne[255];
WCHAR szTitleChildTwo[255];
char str[255] = "";
int x = 200;
int y = 200;
int height = 300;
int width = 500;

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	HWND hwnd;
	// Дескриптор окна
	MSG msg;
	// Структура сообщения Windows
	WNDCLASSEX wcl;
	// Структура, определяющая класс окна

	LoadStringW(hThisInst, IDS_APP_TITLE, szTitle, 255);
	LoadStringW(hThisInst, IDC_WINDOWSPROJECTPR, szWinName, 255);

	/* Определение класса окна */
	wcl.hInstance = hThisInst; // Работа с данным экземпляром
	wcl.lpszClassName = szWinName; // Имя класса окна
	wcl.lpfnWndProc = WindowFunc; // Оконная функция
	wcl.style = 0; // Использовать стиль окна по умолчанию
	wcl.cbSize = sizeof(WNDCLASSEX); // Установка размера
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Большие иконки
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // Маленькие
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW); // Стиль курсора
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = CreateSolidBrush(RGB(60, 129, 23));//(HBRUSH)GetStockObject(HOLLOW_BRUSH);
	/* Регистрация класса окна */
	if (!RegisterClassEx(&wcl)) return 0;
	/* Создаём само окно */

	hwnd = CreateWindow(szWinName, // Имя класса окна
		szTitle,
		// Заголовок
		WS_OVERLAPPEDWINDOW,
		// Стиль окна - нормальный
		x,
		// Координата X - выбирает Windows
		y,
		// Ширина - выбирает Windows
		width, height,
		// Высота - выбирает Windows
		nullptr,
		// Нет родительского окна
		nullptr,
		// Нет меню
		hThisInst, // Работа с данным экземпляром программы
		// Дополнительные аргументы отсутствуют
		nullptr
	);

	/* Вывод окна */
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);


	HWND hwndChildOne;
	// Дескриптор окна
	WNDCLASSEX wclChildOne;
	// Структура, определяющая класс окна

	LPCWSTR szWinNameChildOne = L"childOne";
	LPCWSTR szTitleChildOne = L"childOne";

	wclChildOne.hInstance = hThisInst; // Работа с данным экземпляром
	wclChildOne.lpszClassName = szWinNameChildOne; // Имя класса окна
	wclChildOne.lpfnWndProc = WindowFunc; // Оконная функция
	wclChildOne.style = 0; // Использовать стиль окна по умолчанию
	wclChildOne.cbSize = sizeof(WNDCLASSEX); // Установка размера
	wclChildOne.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Большие иконки
	wclChildOne.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // Маленькие
	wclChildOne.hCursor = LoadCursor(NULL, IDC_ARROW); // Стиль курсора
	wclChildOne.lpszMenuName = NULL;
	wclChildOne.cbClsExtra = 0;
	wclChildOne.cbWndExtra = 0;
	wclChildOne.hbrBackground = CreateSolidBrush(RGB(249, 8, 213));

	if (!RegisterClassEx(&wclChildOne)) return 0;
	//RegisterClassEx(&wclChildOne);

	hwndChildOne = CreateWindow(szWinNameChildOne, // Имя класса окна
		szTitleChildOne,
		// Заголовок

		WS_CHILDWINDOW | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		// Стиль окна - нормальный
		x * 0.4,
		// Координата X - выбирает Windows
		y * 0.4,
		// Координата Y
		width - 200,
		// Ширина - выбирает Windows
		height - 200,
		// Высота - выбирает Windows
		hwnd,
		// родительское окно
		nullptr,
		// Нет меню
		hThisInst, // Работа с данным экземпляром программы
		// Дополнительные аргументы отсутствуют
		nullptr
	);

	ShowWindow(hwndChildOne, SW_NORMAL);
	UpdateWindow(hwndChildOne);

	HWND hwndChildTwo;
	WNDCLASSEX wclChildTwo;

	LPCWSTR szWinNameChildTwo = L"childTwo";
	LPCWSTR szTitleChildTwo = L"childTwo";

	wclChildTwo.hInstance = hThisInst; // Работа с данным экземпляром
	wclChildTwo.lpszClassName = szWinNameChildTwo; // Имя класса окна
	wclChildTwo.lpfnWndProc = WindowFunc; // Оконная функция
	wclChildTwo.style = 0; // Использовать стиль окна по умолчанию
	wclChildTwo.cbSize = sizeof(WNDCLASSEX); // Установка размера
	wclChildTwo.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Большие иконки
	wclChildTwo.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // Маленькие
	wclChildTwo.hCursor = LoadCursor(NULL, IDC_ARROW); // Стиль курсора
	wclChildTwo.lpszMenuName = NULL;
	wclChildTwo.cbClsExtra = 0;
	wclChildTwo.cbWndExtra = 0;
	wclChildTwo.hbrBackground = CreateSolidBrush(RGB(29, 7, 249));

	if (!RegisterClassEx(&wclChildTwo)) return 0;

	hwndChildTwo = CreateWindow(szWinNameChildTwo,
		szTitleChildTwo, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER,
		x * 0.4 * 0.8, y * 0.4 * 0.4, width * 0.8 * 0.4, height * 0.8 * 0.8, hwndChildOne, nullptr, hThisInst, nullptr
	);

	ShowWindow(hwndChildTwo, SW_NORMAL);
	UpdateWindow(hwndChildTwo);



	/* Создание цикла обработки сообщений */
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); //Разрешить использование клавиатуры
		DispatchMessage(&msg); // Вернуть управление Windows
	}
	return msg.wParam;
}
// WinMain
/* Оконная функция обработки сообщений, полученных из оче-
реди сообщений. */
LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	switch (message)
	{
	case WM_CHAR: // Символьное сообщение
	//от клавиатуры
		break;
	case WM_LBUTTONDOWN:  // Обработчик
	//нажатия левой кнопки мыши
		break;
	case WM_RBUTTONDOWN: // Обработчик
	//нажатия правой кнопки мыши
		break;
	case WM_DESTROY:
		// Завершение программы
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam,
			lParam);
	}
	return 0;
}
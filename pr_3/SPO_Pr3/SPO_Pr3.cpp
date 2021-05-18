// SPO_Pr3.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SPO_Pr3.h"
#include <windows.h>
#include "commdlg.h" //для openfileName
#include "resource.h" //для всяких IDC штук, чтоб были видны
#include <commctrl.h>
#pragma comment(lib, "ComCtl32.lib") //без нее ошибка линковки для hStatusWindow
#include <string.h>
#include <stdio.h>
#include <string>


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
LPCWSTR szDialogClass = L"Dialog";
int butWidth = 300;
int butHeight = 50;
int textColor = 0;
HWND hWnd, buttonModal, buttonNonModal, buttonDialog, edit;
HWND slider[2] = { nullptr, nullptr };

// Отправить объявления функций, включенных в этот модуль кода: 
ATOM                RegisterClass(HINSTANCE hInstance);
ATOM                RegisterDialogClass(HINSTANCE hInstance);
BOOL                InitInstances(HINSTANCE, int);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK ModalFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK NonModalFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPOPR3, szWindowClass, MAX_LOADSTRING);
    RegisterClass(hInstance);
    RegisterDialogClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstances (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPOPR3));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPOPR3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPOPR3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM RegisterDialogClass(HINSTANCE hInstance)
{
    WNDCLASSEX child;

    child.hInstance = hInstance;
    child.lpszClassName = szDialogClass; // Имя класса окна
    child.lpfnWndProc = WndProc; // Оконная функция childWindowFunc
    child.style = 0; // Использовать стиль окна по умолчанию
    child.cbSize = sizeof(WNDCLASSEX); // Установка размера WNDCLASSEX
    child.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    child.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    child.hCursor = LoadCursor(NULL, IDC_ARROW);
    child.lpszMenuName = NULL;
    child.cbClsExtra = 0; child.cbWndExtra = 0;
    child.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    return RegisterClassExW(&child);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstances(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   buttonDialog = CreateWindowW(L"button", L"Показать диалоговое окно",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       100, 400, butWidth, butHeight, hWnd, (HMENU)IDM_DIALOG, hInstance, nullptr);

   buttonNonModal = CreateWindowW(L"button", L"Показать немодальное диалоговое окно", 
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       450, 400, butWidth, butHeight, hWnd, (HMENU)IDM_NONMODAL, hInstance, nullptr);

   buttonModal = CreateWindowW(L"button", L"Показать модальное диалоговое окно",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       800, 400, butWidth, butHeight, hWnd, (HMENU)IDM_MODAL, hInstance, nullptr);

   edit = CreateWindowW(L"edit", L"Текст по умолчанию",
       WS_CHILDWINDOW | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
       100, 100, butWidth, butHeight * 4, hWnd, NULL, hInstance, NULL);


   /*HWND hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, L"", hWnd, 5000); //создание строки состояний
   hMenu = CreateMenu();
   AppendMenu(hMenu, MF_STRING, 100, L"File"); //элемент меню с уникальным ид 10
   SetMenu(hWnd, hMenu);*/

   for (int i = 0; i < sizeof(slider) / sizeof(slider[0]); i++) {
       slider[i] = CreateWindowW(TRACKBAR_CLASS, L"", WS_CHILDWINDOW | TBS_AUTOTICKS, 450, 150 + i * (butHeight + 25), butWidth, butHeight, hWnd, NULL, hInstance, NULL);
       
       ShowWindow(slider[i], nCmdShow);
       UpdateWindow(slider[i]);
       SendMessage(slider[i], TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 10));
   };

   if (!hWnd || !edit)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ShowWindow(edit, nCmdShow);
   UpdateWindow(edit);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    CHAR position[64];
    OPENFILENAME ofn; // common dialog box structure
    wchar_t szFile[260]; //for filename save
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_DIALOG: 
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, ModalFunc);
                break;
            case IDM_NONMODAL:
                buttonDialog = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, NonModalFunc);
                ShowWindow(buttonDialog, SHOW_OPENWINDOW);
                UpdateWindow(buttonDialog);
                break;
            case IDM_MODAL:
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szFile;
                ofn.lpstrFile[0] = '\0'; // Set lpstrFile[0] to '\0' so that GetOpenFileName does not initialized
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                if (GetOpenFileName(&ofn))
                {
                    SendMessage(edit, WM_SETTEXT, 0, (LPARAM)ofn.lpstrFile);
                }
                else {
                    if (CommDlgExtendedError()) SendMessage(edit, WM_SETTEXT, 0, (LPARAM)L"Произошла ошибка");
                    else SendMessage(edit, WM_SETTEXT, 0, (LPARAM)L"Отмена");
                }
                break;
            case WM_CTLCOLORSTATIC:
                hdc = (HDC)wParam;
                switch (textColor) {
                case IDM_DIALOG: SetTextColor(hdc, RGB(255, 0, 0)); break;
                case IDM_NONMODAL: SetTextColor(hdc, RGB(0, 255, 0)); break;
                case IDM_MODAL: SetTextColor(hdc, RGB(0, 0, 255)); break;
                }
                break;
            case WM_HSCROLL:
                switch (LOWORD(wParam)) {
                case TB_THUMBTRACK: case TB_THUMBPOSITION:
                    if (slider[0] == (HWND)lParam) { // обработка события 1-го trackbar
                        wsprintfA(position, "Позиция-1: %d", HIWORD(wParam));
                        SetWindowTextA(edit, position);
                    }
                    else if (slider[1] == (HWND)lParam) { // обработка события 2-ого trackbar
                        wsprintfA(position, "Позиция-2: %d", HIWORD(wParam));
                        SetWindowTextA(edit, position);
                    }
                    break;
                } break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL CALLBACK ModalFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    LPTSTR text;
    switch (message) {
    case WM_INITDIALOG:
        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Красный");
        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Зеленый");
        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Синий");
        return FALSE;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDCANCEL: EndDialog(hwnd, LOWORD(wParam)); return TRUE;
        case IDOK:
            MessageBox(0, L"Модальное окно", L"Системное модальное окно", 0);
            return TRUE;
        case IDC_LIST1:
            switch (HIWORD(wParam)) {
            case LBN_SELCHANGE:
                char name[100];
                memset(name, 0, sizeof(name));
                //1.узнаем позицию выделеного элемента в списке
                int num = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
                //2.считываем этот элемент списка
                SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, (LPARAM)num, (WPARAM)name);
                //3.записывваем элемент в поле
                int len = GetWindowTextLength(edit) + 1;
                text = new TCHAR[len];
                GetWindowText(edit, text, len);
                SendMessage(edit, WM_SETTEXT, 0, (LPARAM)text);
                textColor = num + 1;
                return FALSE;
            }
            return FALSE;
        } break;
    case WM_CLOSE: EndDialog(hwnd, LOWORD(wParam)); return FALSE;
    case WM_DESTROY: return TRUE;
    }
    return FALSE;
}

BOOL CALLBACK NonModalFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
    case WM_INITDIALOG:
        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Пустое окно."); return FALSE; break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL: DestroyWindow(hwnd); return TRUE;
        case IDOK:
            MessageBox(0, L"Немодальное окно", L"Cистемное модальное окно", 0);
            return TRUE;
        }
        break;
    case WM_CLOSE: DestroyWindow(hwnd); return FALSE;
    case WM_DESTROY: return TRUE;
    }
    return FALSE;
}

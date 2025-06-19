#include <windows.h>
#include <string>
#include <cmath>

HWND hEdit;
std::wstring buffer;
double result = 0;
wchar_t lastOp = 0;
bool newInput = true;

const wchar_t* buttons[] = {
    L"AC", L"C", L"√", L"^",
    L"sin", L"cos", L"tan", L"cot",
    L"7", L"8", L"9", L"/",
    L"4", L"5", L"6", L"*",
    L"1", L"2", L"3", L"-",
    L"0", L".", L"=", L"+"
};

void UpdateDisplay() {
    SetWindowTextW(hEdit, buffer.empty() ? L"0" : buffer.c_str());
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hEdit = CreateWindowW(
            L"EDIT", L"0",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
            10, 10, 350, 40,
            hwnd, NULL, NULL, NULL
        );

        for (int i = 0; i < 24; i++) {
            int x = 10 + (i % 4) * 90;
            int y = 60 + (i / 4) * 50;
            CreateWindowW(
                L"BUTTON", buttons[i],
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                x, y, 80, 40,
                hwnd, (HMENU)(1000 + i), NULL, NULL
            );
        }
        break;
    }

    case WM_COMMAND: {
    int buttonId = LOWORD(wParam);
    if (buttonId >= 1000 && buttonId <= 1023) {
        int index = buttonId - 1000;

        
        if ((index >= 8 && index <= 10) ||   
            (index >= 11 && index <= 13) ||
            (index >= 14 && index <= 16) ||
            (index >= 17 && index <= 19) ||  
            index == 20 || index == 21) {    
            
            if (newInput) {
                buffer.clear();
                newInput = false;
            }
            
            
            if (index == 20) {
                if (!buffer.empty()) {
                    buffer += L"0";
                }
            }
            else if (index == 21) {
                if (buffer.empty()) {
                    buffer = L"0.";
                }
                else if (buffer.find(L'.') == std::wstring::npos) {
                    buffer += L".";
                }
            }
            else {
                buffer += buttons[index][0]; 
            }
            
            UpdateDisplay();
            return 0;
        }

            double num = buffer.empty() ? result : _wtof(buffer.c_str());

            switch (lastOp) {
            case L'+': result += num; break;
            case L'-': result -= num; break;
            case L'*': result *= num; break;
            case L'/': result /= num; break;
            case L'^': result = pow(result, num); break;
            case L'√': result = sqrt(num); break;
            case L's': result = sin(num * 3.141592653589793 / 180.0); break;
            case L'c': result = cos(num * 3.141592653589793 / 180.0); break;
            case L't': result = tan(num * 3.141592653589793 / 180.0); break;
            case L'o': result = 1.0 / tan(num * 3.141592653589793 / 180.0); break;
            default: result = num; break;
            }

            if (index == 2) {
                result = sqrt(num);
                buffer = std::to_wstring(result);
                UpdateDisplay();
                newInput = true;
                return 0;
            }

            if (index == 22) {
                buffer = std::to_wstring(result);
                UpdateDisplay();
                newInput = true;
                return 0;
            }

            if (index >= 4 && index <= 7) {
                wchar_t ops[] = { L's', L'c', L't', L'o' };
                lastOp = ops[index - 4];
            }
            else {
                lastOp = buttons[index][0];
            }

            buffer.clear();
            UpdateDisplay();
            newInput = true;
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SciCalcWindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Калькулятор",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 450,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

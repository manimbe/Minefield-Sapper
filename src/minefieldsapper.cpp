#include <windows.h>

LRESULT CALLBACK wProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){

    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //is there any other better?
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); //is there any other better?
    wc.hInstance = hInst;
    wc.lpszClassName = L"wClass";
    wc.lpfnWndProc = wProcedure;

    if(!RegisterClassW(&wc)) return -1;
    CreateWindowW(L"wClass", L"Minefield Sapper", WS_OVERLAPPEDWINDOW|WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
};

LRESULT CALLBACK wProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
};

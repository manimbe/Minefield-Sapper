#include <windows.h>

LRESULT CALLBACK wProcedure(HWND, UINT, WPARAM, LPARAM);
void createGame(HWND);

int row = 6, column = 5;
int space = row*column;
HWND* grid = new HWND[space];
int* loc = new int[space];

void locReset(){
    for(int i = 0; i<space; i++){
        loc[i] = 0;
    }
}

void locSet(){ //just for testing some things
    
   void locReset();

    loc[6] = -1;
    loc[12] = -1;
    loc[24] = -1;
}

void bDisable(){
    for(int i = 0; i<space; i++){
        EnableWindow(grid[i], 0);
    }
}

void bEnable(){
    for(int i = 0; i<space; i++){
        EnableWindow(grid[i], 1);
    }
}

void mCount(WPARAM &wParam){
    int c = 0;
    int X = wParam / column;
    int Y = wParam % column;

    int temp;

    for(int i = (X-1); i<(X+2); i++){
        if(i>-1 && i<row){
            for(int j = (Y-1); j<(Y+2); j++){
                //temp = grid[i*column + j]
                if(j>-1 && j<column && loc[i*column+j] == -1){
                    c++;
                }
            }
        }
    }

    if(c == 0) SetWindowTextW(grid[wParam], L"0");
    if(c == 1) SetWindowTextW(grid[wParam], L"1");
    if(c == 2) SetWindowTextW(grid[wParam], L"2");
    if(c == 3) SetWindowTextW(grid[wParam], L"3");
    if(c == 4) SetWindowTextW(grid[wParam], L"4");
    if(c == 5) SetWindowTextW(grid[wParam], L"5");
    if(c == 6) SetWindowTextW(grid[wParam], L"6");
    if(c == 7) SetWindowTextW(grid[wParam], L"7");
    if(c == 8) SetWindowTextW(grid[wParam], L"8");
    if(c == 9) SetWindowTextW(grid[wParam], L"9");

}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){

    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //is there any other better?
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); //is there any other better?
    wc.hInstance = hInst;
    wc.lpszClassName = L"wClass";
    wc.lpfnWndProc = wProcedure;

    if(!RegisterClassW(&wc)) return -1;
    CreateWindowW(L"wClass", L"Minefield Sapper", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
};

LRESULT CALLBACK wProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
    case WM_CREATE:
        createGame(hWnd);
        break;
    case WM_COMMAND:

        if(loc[wParam] == -1){
            bDisable();
            MessageBox(NULL, "You Lose", "KABOOM!", MB_OK);
        }

        if(loc[wParam] != -1){
            EnableWindow(grid[wParam], 0);
            mCount(wParam);
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
}

int sz = 5;
int size = 35;
int sz1 = 5;
int sz2 = 5;

void createGame(HWND hWnd){

    locReset();
    locSet();

    for(int i=0; i<column; i++){
        for(int j=0; j<row; j++){
            grid[(i + j*column)] = CreateWindowW(L"button", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, sz1 + size*i, sz2 + size*j, size, size, hWnd, (HMENU)(i + j*column), NULL, NULL);
        }
    }
}

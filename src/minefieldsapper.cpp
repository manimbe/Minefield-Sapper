#include <windows.h>
#include <ctime>

LRESULT CALLBACK wProcedure(HWND, UINT, WPARAM, LPARAM);
void createGame(HWND);
void createConfig(HINSTANCE);
void displayConfig(HWND);
void changeSize();

HWND hErow;
HWND hEcol;
HWND hEmines;

HWND hWnd;
HWND sWnd;
HWND hApply;

HMENU hMenu;

const int MAX_CAP = 999;
int row = 5, column = 6;
int space = row*column;
HWND* grid = new HWND[MAX_CAP];
int* loc = new int[MAX_CAP];

int mines = 5;
int none = space - mines;
bool fPlay = 0;
bool sConfig = 0;
int sLeft = space - mines;

int random(){
    return (std::rand()%(space-1));
}

void addMenus(HWND hWnd);

void locSet(WPARAM &wParam){
    fPlay = 1;
    void locReset();
    int place;
    int mineAmount = mines; //should I verify there are more "space" than "mines"?

    while(mineAmount > 0){
        place = random();
        if(place != wParam && loc[place] != -1){
            loc[place] = -1;
            mineAmount--;
        }
    }
}

void displayMines(){
    for(int i = 0; i < space; i++){
        if(loc[i] == -1) SetWindowTextW(grid[i], L"X");
    }
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

void locReset(){
    fPlay = 0;
    for(int i = 0; i<space; i++){
        loc[i] = 0;
        SetWindowTextW(grid[i], NULL);
    }
    sLeft = space - mines;
    bEnable();
}

void openbutton(WPARAM &wParam){
    EnableWindow(grid[wParam], 0);
    int mCount = 0;

    int X = wParam / column;
    int Y = wParam % column;

    for(int i = (X-1); i<(X+2); i++){
        if(i>-1 && i<row){
            for(int j = (Y-1); j<(Y+2); j++){
                if(j>-1 && j<column && loc[i*column+j] == -1){
                    mCount++;
                }
            }
        }
    }

    if(mCount == 0){
        for(int i = (X-1); i<(X+2); i++){
            if(i>-1 && i<row){
                for(int j = (Y-1); j<(Y+2); j++){
                    if(j>-1 && j<column){
                        WPARAM temp = i*column+j;
                        if(IsWindowEnabled(grid[temp])) openbutton(temp);
                    }
                }
            }
        }
    }

    if(mCount == 0) SetWindowTextW(grid[wParam], L"0"); //I need to find a better way to do this...
    if(mCount == 1) SetWindowTextW(grid[wParam], L"1");
    if(mCount == 2) SetWindowTextW(grid[wParam], L"2");
    if(mCount == 3) SetWindowTextW(grid[wParam], L"3");
    if(mCount == 4) SetWindowTextW(grid[wParam], L"4");
    if(mCount == 5) SetWindowTextW(grid[wParam], L"5");
    if(mCount == 6) SetWindowTextW(grid[wParam], L"6");
    if(mCount == 7) SetWindowTextW(grid[wParam], L"7");
    if(mCount == 8) SetWindowTextW(grid[wParam], L"8");
    if(mCount == 9) SetWindowTextW(grid[wParam], L"9");

    sLeft--;

    if(sLeft == 0){
        bDisable();
        MessageBox(NULL, "You Win", "Congratulations!", MB_OK);
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){

    std::srand(std::time(NULL));

    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //is there any other better?
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); //is there any other better?
    wc.hInstance = hInst;
    wc.lpszClassName = L"wClass";
    wc.lpfnWndProc = wProcedure;

    if(!RegisterClassW(&wc)) return -1;

    createConfig(hInst);

    hWnd = CreateWindowW(L"wClass", L"Minefield Sapper", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 230, 235, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
};

LRESULT CALLBACK wProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
    case WM_CREATE:
        addMenus(hWnd);
        createGame(hWnd);
        break;
    case WM_COMMAND:
        if(fPlay == 0 && wParam != 1002){
            locSet(wParam);
        }

        if(loc[wParam] != -1 && wParam != 1001 && wParam != 1002){
            openbutton(wParam);
        }

        if(loc[wParam] == -1 && fPlay == 1){
            bDisable();
            displayMines();
            MessageBox(NULL, "You Lose", "KABOOM!", MB_OK);
        }

        if(wParam == 1001) locReset();
        if(wParam == 1002 && sConfig == 0){
            displayConfig(hWnd);
            //EnableMenuItem(hMenu, 1002, MF_DISABLED);
            sConfig = 1;
        }

        break;
    case WM_DESTROY:
        delete[] grid;
        delete[] loc;
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

void addMenus(HWND hWnd){
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, 1001, L"Reset"); //Define 1001
    SetMenu(hWnd, hMenu);

    AppendMenuW(hMenu, MF_STRING, 1002, L"Config"); //Define 1002
    SetMenu(hWnd, hMenu);
}

void createGame(HWND hWnd){

    locReset();

    for(int i=0; i<column; i++){
        for(int j=0; j<row; j++){
            grid[(i + j*column)] = CreateWindowW(L"button", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, sz1 + size*i, sz2 + size*j, size, size, hWnd, (HMENU)(i + j*column), NULL, NULL);
        }
    }
}

LRESULT CALLBACK sProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
    case WM_CLOSE:
        DestroyWindow(sWnd);
        sConfig = 0;
        //EnableMenuItem(hMenu, 1002, MF_ENABLED);
        break;
    case WM_COMMAND:
        if(wParam == 1003) changeSize();
        break;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
}

void createConfig(HINSTANCE hInst){
    WNDCLASSW wConfig = {0};

    wConfig.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wConfig.hCursor = LoadCursor(NULL, IDC_ARROW);
    wConfig.hInstance = hInst;
    wConfig.lpszClassName = L"sClass";
    wConfig.lpfnWndProc = sProcedure;

    RegisterClassW(&wConfig);
}

void displayConfig(HWND hWnd){
    sWnd = CreateWindowW(L"sClass", NULL, WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 200, 200, 185, 205, hWnd, NULL, NULL, NULL);

    CreateWindowW(L"static", L"rows", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 15, 100, 30, sWnd, NULL, NULL, NULL);
    CreateWindowW(L"static", L"columns", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 50, 100, 30, sWnd, NULL, NULL, NULL);
    CreateWindowW(L"static", L"mines", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 85, 100, 30, sWnd, NULL, NULL, NULL);

    hErow = CreateWindowW(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 15, 35, 30, sWnd, (HMENU)9991, NULL, NULL);
    HWND hEcol = CreateWindowW(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 50, 35, 30, sWnd, (HMENU)9992, NULL, NULL);
    HWND hEmines = CreateWindowW(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 85, 35, 30, sWnd, (HMENU)9993, NULL, NULL);

    hApply = CreateWindowW(L"button", L"APPLY", WS_VISIBLE | WS_CHILD | SS_CENTER, 15, 125, 140, 30, sWnd, (HMENU)1003, NULL, NULL);

}

void changeSize(){
    int aRow = GetDlgItemInt(sWnd, 9991, NULL, FALSE); //verify parameters
    int aCol = GetDlgItemInt(sWnd, 9992, NULL, FALSE);
    int lMines = GetDlgItemInt(sWnd, 9993, NULL, FALSE);
    int sGrid = aRow*aCol;

    if(sGrid > 999 || sGrid < 5 || (sGrid - lMines) < 2){
        MessageBeep(MB_ICONERROR);
        return;
    }

    for(int i = 0; i<1000; i++){
        DestroyWindow(grid[i]);
    }

    int sRow = 60 + size*aRow;
    int sCol = 20 + size*aCol;

    row = aRow;
    column = aCol;
    space = row*column;
    mines = lMines;
    sLeft = space - mines;

    SetWindowPos(hWnd, NULL, NULL, NULL, sCol, sRow, SWP_NOMOVE);
    createGame(hWnd);
}

#include <windows.h>
#include "header.h"
#include "function.h"
#include "variable.h"
#include "resource.h"
#include "child_window.h"

/* --- WinMain ------------------------------------------------------ */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    LPCTSTR szClassName = TEXT("TIMER");

    HWND hwnd;
    MSG messages;
    HACCEL hAccel;

    HBRUSH hbrush_Gray_White = CreateSolidBrush(RGB(240, 240, 240));
    if(!InitWndClass(hInstance, szClassName, hbrush_Gray_White))
		return -1;

    if(!(hwnd = InitInstance(hInstance, szClassName)))
		return -1;

    hAccel = LoadAccelerators(hInstance, TEXT("ACCELERATOR"));

    while(GetMessage(&messages, NULL, 0, 0)) {
        if(!TranslateAccelerator(hwnd, hAccel, &messages)) {
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
    }

    DeleteObject(hbrush_Gray_White);
    return messages.wParam;
}


/* --- Window Procedure --------------------------------------------- */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/* Control Handle */
    static HWND hEdit, hListAlarm, hListTimer;

    /* Alarm / Timer Stock */
    static AlarmSt AlarmStock[128];
    static TimerSt TimerStock[128];

    static int nStockTimer, nStockAlarm;

    static int newTime_tSet;
    static TCHAR szTimeStr[24];


    /* -------------------- Message Event ------------------------ */
    switch(message) {
    case WM_CREATE:
        /* Init */
        InitControl(hwnd);

        /* Set 1Hz timer */
        SetTimer(hwnd, TIMER_SECOND, 1000, NULL);

        /* Get initTime in edit*/
        hEdit = CreateEditChild(hwnd, pos_edit, EDIT_ONE);
        UpdateLocalTimeinEdit(hEdit);

        /* Button group */
        for(int i = 0; i < 5; ++i)
            CreateButtonChild(hwnd, pos_button[i], (BUTTON_NEW + i), buttonLabel[i]);

        /* ListBox */
        hListAlarm = CreateListBoxChild(hwnd, pos_listAlarm, LISTBOX_ALARM, FALSE);
        hListTimer = CreateListBoxChild(hwnd, pos_listTimer, LISTBOX_TIMER, FALSE);
    	break;

    case WM_COMMAND:
		switch(LOWORD(wParam)) {
        case BUTTON_NEW:
            newTime_tSet = DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                                        TEXT("DIALOG_NEW"), hwnd, DialogProcedure);
            if(!newTime_tSet)
                break;

            if(newTime_tSet >> 24) {
                SetAlarm_inStock(&AlarmStock[nStockAlarm], newTime_tSet);
                SetAlarm_toString(&AlarmStock[nStockAlarm], szTimeStr);
                SendMessage(hListAlarm, LB_INSERTSTRING, nStockAlarm, (LPARAM)szTimeStr);
                nStockAlarm++;
            }
            else {
                SetTimer_inStock(&TimerStock[nStockTimer], newTime_tSet);
                SetTimer_toString(&TimerStock[nStockTimer], szTimeStr);
                SendMessage(hListTimer, LB_INSERTSTRING, nStockTimer, (LPARAM)szTimeStr);
                nStockTimer++;
            }
            break;

        case BUTTON_DELETE:
            if(SendMessage(hListAlarm, LB_GETCURSEL, 0, 0) != -1) {
                DeleteAlarm(hListAlarm, AlarmStock, nStockAlarm);
                nStockAlarm--;
            }
            if(SendMessage(hListTimer, LB_GETCURSEL, 0, 0) != -1) {
                DeleteTimer(hListTimer, TimerStock, nStockTimer);
                nStockTimer--;
            }
            break;

        case BUTTON_ALT:
            if(SendMessage(hListAlarm, LB_GETCURSEL, 0, 0) != -1)
                ModifyAlarmState(hListAlarm, AlarmStock, nStockAlarm);
            break;

        case BUTTON_MOVEUP:
            if(SendMessage(hListAlarm, LB_GETCURSEL, 0, 0) != -1)
                SwapStockAlarm(hListAlarm, AlarmStock, nStockAlarm, UP);
            if(SendMessage(hListTimer, LB_GETCURSEL, 0, 0) != -1)
                SwapStockTimer(hListTimer, TimerStock, nStockTimer, UP);
            break;

        case BUTTON_MOVEDOWN:
            if(SendMessage(hListAlarm, LB_GETCURSEL, 0, 0) != -1)
                SwapStockAlarm(hListAlarm, AlarmStock, nStockAlarm, DOWN);
            if(SendMessage(hListTimer, LB_GETCURSEL, 0, 0) != -1)
                SwapStockTimer(hListTimer, TimerStock, nStockTimer, DOWN);
            break;

		case MENUID_QUIT:
		    PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}

        if(HIWORD(wParam) == LBN_SELCHANGE)
            MoveCurSelect(hListAlarm, hListTimer, lParam);

		break;

    case WM_TIMER:
        UpdateLocalTimeinEdit(hEdit);

        /* Alarm */
        NotifyAlarm(hwnd, hListAlarm, AlarmStock, nStockAlarm);

        /* Timer */
        UpdateTimerRemainder(hListTimer, TimerStock, nStockTimer);
        NotifyTimer(hwnd, TimerStock, nStockTimer);
        break;

    case WM_CLOSE:
        (MessageBox(hwnd, TEXT("Finish?              "), TEXT("Quit"),
           MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) ? DestroyWindow(hwnd) : 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CTLCOLOREDIT:
        return (INT_PTR)CreateSolidBrush(RGB(250, 250, 250));

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}


/* --- Dialog Procedure ---------------------------------------------- */
BOOL CALLBACK DialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int nReturnValue;
    static HWND hHour, hMinute, hSecond;

    static bool is_Alarm = true;

    switch(message) {
    case WM_INITDIALOG:
        GetComboBoxHandle(hwnd, &hHour, &hMinute, &hSecond);
        InitNewDlgComboBox(hwnd, hHour, hMinute, hSecond);

        CheckRadioButton(hwnd, RB_ALARM, RB_TIMER, (is_Alarm) ? RB_ALARM : RB_TIMER);
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case RB_ALARM:
        case RB_TIMER:
            is_Alarm = (LOWORD(wParam) == RB_ALARM);
            CheckRadioButton(hwnd, RB_ALARM, RB_TIMER, (is_Alarm) ? RB_ALARM : RB_TIMER);
            break;

        case BUTTON_SET:
            nReturnValue = GetTimeSet_Ed(hwnd, hHour, hMinute, hSecond, is_Alarm);
            if(nReturnValue == -1)
                break;
            EndDialog(hwnd, nReturnValue);
            break;

        case BUTTON_CANCEL:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;

    default:
        return FALSE;
    }
    return TRUE;
}


/* --- Create Window Class ------------------------------------------ */
BOOL InitWndClass(HINSTANCE hInstance, LPCTSTR lpszClassName, HBRUSH hBackGroundColor)
{
	WNDCLASS winc;

	winc.style          = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc    = WindowProcedure;
    winc.cbClsExtra     = 0;
    winc.cbWndExtra     = 0;
    winc.hInstance      = hInstance;
    winc.hIcon          = LoadIcon(hInstance, TEXT("TIMER"));
    winc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    winc.hbrBackground  = (HBRUSH)hBackGroundColor;
    winc.lpszMenuName   = TEXT("MENUITEMS");
    winc.lpszClassName  = lpszClassName;

    return RegisterClass(&winc);
}


/* --- Init Window Instance ----------------------------------------- */
HWND InitInstance(HINSTANCE hInstance, LPCTSTR lpszClassName)
{
	HWND hwnd;
    hwnd = CreateWindow(lpszClassName, TEXT("Explorer : : SeLFtimeR"),
                        (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_VISIBLE,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        419, 390,
                        NULL, NULL, hInstance, NULL);

    UpdateWindow(hwnd);

    HMENU hMenu = GetSystemMenu(hwnd, FALSE);
    for(int i = 0; i < 3; ++i)
        DeleteMenu(hMenu, i, MF_BYPOSITION);

    DrawMenuBar(hwnd);
    return hwnd;
}

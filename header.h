#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <windows.h>

/* --- Prototype ------------------------------------------------------------------------- */

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL InitWndClass(HINSTANCE hInstance, LPCTSTR lpszClassName, HBRUSH hBackGroundColor);
HWND InitInstance(HINSTANCE hInstance, LPCTSTR lpszClassName);

/* --- End Prototype --------------------------------------------------------------------- */

typedef struct tagTimer{
    int hour;
    int minute;
    int second;
    bool is_zero;
}TimerSt;


typedef struct tagAlarm{
    int hour;
    int minute;
    int second;
    bool state;
}AlarmSt;


enum TimerID {
    TIMER_SECOND = 500,
    TIMER_MINUTE,
};

enum ButtonID {
	BUTTON_NEW = 1000,
	BUTTON_ALT,
	BUTTON_DELETE,
	BUTTON_MOVEUP,
	BUTTON_MOVEDOWN,
};

enum EditID {
    EDIT_ONE = 2000,
};

enum ListBoxID {
    LISTBOX_ALARM = 3000,
    LISTBOX_TIMER,
};

#endif // HEADER_H_INCLUDED

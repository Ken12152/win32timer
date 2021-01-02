#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <windows.h>
#include "header.h"
#define UP 1
#define DOWN 0

/* Window Procedure */
VOID InitControl(HWND hwnd);

VOID UpdateLocalTimeinEdit(HWND _hEdit);

VOID MoveCurSelect(HWND _hListAlarm, HWND _hListTimer, LPARAM _lParam);


/* Timer */
VOID SetTimer_inStock(TimerSt* _stock, int _time_to_set);
VOID SetTimer_toString(TimerSt* _stock, LPTSTR _szDestiny);

VOID UpdateTimerRemainder(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer);
VOID NotifyTimer(HWND hwnd, TimerSt* _totalStock, int _nStockTimer);

VOID SwapStockTimer(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer, bool _is_up);
VOID DeleteTimer(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer);

VOID RefreshTimerList(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer, int _CurrentSelect, int _TopIndex);


/* Alarm */
VOID SetAlarm_inStock(AlarmSt* _stock, int _time_to_set);
VOID SetAlarm_toString(AlarmSt* _stock, LPTSTR _szDestiny);

VOID NotifyAlarm(HWND hwnd, HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm);

VOID SwapStockAlarm(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm, bool _is_up);
VOID DeleteAlarm(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm);
VOID ModifyAlarmState(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm);

VOID RefreshAlarmList(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm, int _CurrentSelect, int _TopIndex);



/* Dialog Procedure */
VOID InitNewDlgComboBox(HWND hwnd, HWND _hHour, HWND _hMinute, HWND _hSecond);

VOID GetComboBoxHandle(HWND hwnd, HWND* _hHour, HWND* _hMinute, HWND* _hSecond);

INT GetTimeSet_Ed(HWND hwnd, HWND _hHour, HWND _hMinute, HWND _hSecond, bool _is_Alarm);


#endif // FUNCTION_H_INCLUDED

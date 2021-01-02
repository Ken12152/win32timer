#include "function.h"
#include "resource.h"
#include "variable.h"
#include "child_window.h"

/* ----------------------- In Window Procedure --------------------------------------- */

/* Init Control(GroupBox, Static) */
VOID InitControl(HWND hwnd)
{
    // GroupBox
    CreateGroupBox(hwnd, pos_parentAlarm,  TEXT("BUTTON"), TEXT("Alarm"));
    CreateGroupBox(hwnd, pos_parentTimer,  TEXT("BUTTON"), TEXT("Timer"));
    CreateGroupBox(hwnd, pos_parentButton, TEXT("BUTTON"), TEXT("Button"));
    // Static
    CreateStaticChild(hwnd, pos_staticEdit,     -1, TEXT("Now Time: "), FALSE);
    CreateStaticChild(hwnd, pos_staticAlarm[0], -1, TEXT(" time" ), TRUE);
    CreateStaticChild(hwnd, pos_staticAlarm[1], -1, TEXT(" state"), TRUE);
    CreateStaticChild(hwnd, pos_staticTimer,    -1, TEXT(" rem"  ), TRUE);
}


/* LocalTime to LPSTR */
VOID UpdateLocalTimeinEdit(HWND _hEdit)
{
    static SYSTEMTIME _stTime;
    TCHAR _week[7][12] = {"Sunday", "Monday", "Tuesday",
                         "Wednesday", "Thursday", "Friday", "Saturday"};

    GetLocalTime(&_stTime);

    TCHAR _lpNowTime[32];
    wsprintf(_lpNowTime, TEXT("%s %02d/%02d/%04d %02d:%02d:%02ds"),
        _week[_stTime.wDayOfWeek], _stTime.wDay, _stTime.wMonth,
        _stTime.wYear, _stTime.wHour, _stTime.wMinute, _stTime.wSecond);

    unsigned int _sel = SendMessage(_hEdit, EM_GETSEL, 0, 0);

    SetWindowText(_hEdit, _lpNowTime);
    SendMessage(_hEdit, EM_SETSEL, LOWORD(_sel), HIWORD(_sel));
}


/* Select Current */
VOID MoveCurSelect(HWND _hListAlarm, HWND _hListTimer, LPARAM _lParam)
{
    int _TopIndex_Alarm, _TopIndex_Timer;

    static int _prevSelect_Alarm = -1,
               _prevSelect_Timer = -1;

    static bool _is_SelAlarm = false,
                _is_SelTimer = false;

    _TopIndex_Alarm = SendMessage(_hListAlarm, LB_GETTOPINDEX, 0, 0);
    _TopIndex_Timer = SendMessage(_hListTimer, LB_GETTOPINDEX, 0, 0);

    //SendMessage(_hListTimer, WM_SETREDRAW, FALSE, 0);

    if(!_is_SelAlarm && !_is_SelTimer){
        _prevSelect_Alarm = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);
        _prevSelect_Timer = SendMessage(_hListTimer, LB_GETCURSEL, 0, 0);
        _is_SelAlarm = (_prevSelect_Alarm != -1);
        _is_SelTimer = (_prevSelect_Timer != -1);
    }
    else if(!_is_SelAlarm && _is_SelTimer){

        if((HWND)_lParam == _hListAlarm){
            SendMessage(_hListTimer, LB_SETCARETINDEX, _prevSelect_Timer, 0);
            SendMessage(_hListTimer, LB_SETCURSEL, -1, 0);

            _prevSelect_Alarm = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);
            _prevSelect_Timer = -1;
            _is_SelAlarm = true;
            _is_SelTimer = false;
        }
        else if(SendMessage(_hListTimer, LB_GETCURSEL, 0, 0) == _prevSelect_Timer){
            SendMessage(_hListTimer, LB_SETCARETINDEX, _prevSelect_Timer, 0);
            SendMessage(_hListTimer, LB_SETCURSEL, -1, 0);

            _prevSelect_Timer = -1;
            _is_SelTimer = false;
        }
        else if(SendMessage(_hListTimer, LB_GETCURSEL, 0, 0) != _prevSelect_Timer){
            _prevSelect_Timer = SendMessage(_hListTimer, LB_GETCURSEL, 0, 0);
        }
    }
    else if(_is_SelAlarm && !_is_SelTimer){

        if((HWND)_lParam == _hListTimer){
            SendMessage(_hListAlarm, LB_SETCARETINDEX, _prevSelect_Alarm, 0);
            SendMessage(_hListAlarm, LB_SETCURSEL, -1, 0);

            _prevSelect_Timer = SendMessage(_hListTimer, LB_GETCURSEL, 0, 0);
            _prevSelect_Alarm = -1;
            _is_SelTimer = true;
            _is_SelAlarm = false;
        }
        else if(SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0) == _prevSelect_Alarm){
            SendMessage(_hListAlarm, LB_SETCARETINDEX, _prevSelect_Alarm, 0);
            SendMessage(_hListAlarm, LB_SETCURSEL, -1, 0);

            _prevSelect_Alarm = -1;
            _is_SelAlarm = false;
        }
        else if(SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0) != _prevSelect_Alarm){
            _prevSelect_Alarm = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);
        }
    }
    //SendMessage(_hListTimer, WM_SETREDRAW, TRUE, 0);  // Okonomide...

    SendMessage(_hListTimer, LB_SETTOPINDEX, _TopIndex_Timer, 0);
    SendMessage(_hListAlarm, LB_SETTOPINDEX, _TopIndex_Alarm, 0);
}



/* ------------------------------ Alarm ---------------------------------------------- */

/* Set Alarm in Struct */
VOID SetAlarm_inStock(AlarmSt* _stock, int _time_to_set)
{
    _stock->second   = (char)(_time_to_set >>  0);
    _stock->minute   = (char)(_time_to_set >>  8);
    _stock->hour     = (char)(_time_to_set >> 16);
    _stock->state    = true;
}


/* Set Time to String */
VOID SetAlarm_toString(AlarmSt* _stock, LPTSTR _szDestiny)
{
    wsprintf(_szDestiny, TEXT("%02d:%02d:%02d     %s"),
             _stock->hour, _stock->minute, _stock->second,
             (_stock->state) ? TEXT("ON") : TEXT("OFF"));
}


/* Verify if Past Set_Ed Alarm */
VOID NotifyAlarm(HWND hwnd, HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm)
{
    static SYSTEMTIME _Time_Compare;
    GetLocalTime(&_Time_Compare);

    bool flag_second, flag_minute, flag_hour;
    for(int i = 0; i < _nStockAlarm; ++i){
        if(!_totalStock[i].state)
            continue;

        flag_second = (_totalStock[i].second == _Time_Compare.wSecond);
        flag_minute = (_totalStock[i].minute == _Time_Compare.wMinute);
        flag_hour   = (_totalStock[i].hour   == _Time_Compare.wHour);

        if(!(flag_second & flag_minute & flag_hour))
            continue;

        TCHAR _strtmp[32];
        wsprintf(_strtmp, TEXT("Alarm: %02dh%02dm%02ds !!!    "),
            _totalStock[i].hour, _totalStock[i].minute, _totalStock[i].second);

        MessageBox(hwnd, _strtmp, TEXT("Alarm"),
            MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL);

        _totalStock[i].state = false;
    }

    int _CurrentSelect = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);
    int _TopIndex = SendMessage(_hListAlarm, LB_GETTOPINDEX, 0, 0);

    RefreshAlarmList(_hListAlarm, _totalStock, _nStockAlarm, _CurrentSelect, _TopIndex);
}


/* Swap Alarm Struct */
VOID SwapStockAlarm(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm, bool _is_up)
{
    int _CurrentSelect = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);

    if(_is_up && _CurrentSelect < 1)
        return;
    if(!_is_up && _CurrentSelect + 1 > _nStockAlarm - 1)
        return;

    TimerSt _tmpswap;
    memcpy(&_tmpswap,  &_totalStock[_CurrentSelect], sizeof(AlarmSt));

    if(_is_up){
        memcpy(&_totalStock[_CurrentSelect], &_totalStock[_CurrentSelect - 1], sizeof(AlarmSt));
        memcpy(&_totalStock[_CurrentSelect - 1], &_tmpswap, sizeof(AlarmSt));
    }
    else{
        memcpy(&_totalStock[_CurrentSelect], &_totalStock[_CurrentSelect + 1], sizeof(AlarmSt));
        memcpy(&_totalStock[_CurrentSelect + 1], &_tmpswap, sizeof(AlarmSt));
    }

    int _TopIndex = SendMessage(_hListAlarm, LB_GETTOPINDEX, 0, 0);

    if(_is_up)
        _TopIndex -= (_TopIndex == _CurrentSelect);
    else
        _TopIndex += (_CurrentSelect - _TopIndex == 12);

    _CurrentSelect += ((_is_up) ? -1 : 1);

    RefreshAlarmList(_hListAlarm, _totalStock, _nStockAlarm, _CurrentSelect, _TopIndex);
}


/* Delete Alarm */
VOID DeleteAlarm(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm)
{
    int _CurrentSelect = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);

    for(int i = _CurrentSelect; i < _nStockAlarm - 1; ++i)
        memcpy(&_totalStock[i], &_totalStock[i + 1], sizeof(AlarmSt));

    memset(&_totalStock[_nStockAlarm - 1], 0, sizeof(AlarmSt));

    int _TopIndex = SendMessage(_hListAlarm, LB_GETTOPINDEX, 0, 0);

    _CurrentSelect -= (_CurrentSelect >= _nStockAlarm - 1);

    RefreshAlarmList(_hListAlarm, _totalStock, _nStockAlarm - 1, _CurrentSelect, _TopIndex);
}


/* Modify State(On/Off) */
VOID ModifyAlarmState(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm)
{
    int _CurrentSelect = SendMessage(_hListAlarm, LB_GETCURSEL, 0, 0);
    int _TopIndex = SendMessage(_hListAlarm, LB_GETTOPINDEX, 0, 0);

    _totalStock[_CurrentSelect].state = !_totalStock[_CurrentSelect].state;

    RefreshAlarmList(_hListAlarm, _totalStock, _nStockAlarm, _CurrentSelect, _TopIndex);
}


/* Refresh Alarm List */
VOID RefreshAlarmList(HWND _hListAlarm, AlarmSt* _totalStock, int _nStockAlarm, int _CurrentSelect, int _TopIndex)
{
    SendMessage(_hListAlarm, WM_SETREDRAW, FALSE, 0);

    SendMessage(_hListAlarm, LB_RESETCONTENT , 0, 0);

    TCHAR _strtmp[32];
    for(int i = 0; i < _nStockAlarm; ++i){
        SetAlarm_toString(&_totalStock[i], _strtmp);
        SendMessage(_hListAlarm, LB_INSERTSTRING, i, (LPARAM)_strtmp);
    }

    SendMessage(_hListAlarm, LB_SETCARETINDEX, _CurrentSelect, 0);
    SendMessage(_hListAlarm, LB_SETCURSEL, _CurrentSelect, 0);
    SendMessage(_hListAlarm, LB_SETTOPINDEX, _TopIndex, 0);

    SendMessage(_hListAlarm, WM_SETREDRAW, TRUE, 0);
}



/* ------------------------------ Timer ---------------------------------------------- */

/* Set Time in Struct */
VOID SetTimer_inStock(TimerSt* _stock, int _time_to_set)
{
    _stock->second   = (char)(_time_to_set >>  0);
    _stock->minute   = (char)(_time_to_set >>  8);
    _stock->hour     = (char)(_time_to_set >> 16);
    _stock->is_zero  = false;
}


/* Set Time to String */
VOID SetTimer_toString(TimerSt* _stock, LPTSTR _szDestiny)
{
     wsprintf(_szDestiny, TimerPreview,
              _stock->hour, _stock->minute, _stock->second);
}


/* Update Timer Remainder */
VOID UpdateTimerRemainder(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer)
{
    static TCHAR _strTmp[32];

    for(int i = 0; i < _nStockTimer; ++i){
        if(!_totalStock[i].second && !_totalStock[i].minute && !_totalStock[i].hour)
            continue;

        if(_totalStock[i].second > 0){
            _totalStock[i].second--;
        }
        else if(_totalStock[i].minute > 0){
            _totalStock[i].minute--;
            _totalStock[i].second = 59;
        }
        else if(_totalStock[i].hour > 0){
            _totalStock[i].hour--;
            _totalStock[i].minute = 59;
            _totalStock[i].second = 59;
        }

        if(!_totalStock[i].second && !_totalStock[i].minute && !_totalStock[i].hour){
            _totalStock[i].is_zero = true;
        }
    }

    int _CurrentSelect = SendMessage(_hListTimer, LB_GETCURSEL, 0, 0);
    int _TopIndex = SendMessage(_hListTimer, LB_GETTOPINDEX, 0, 0);

    RefreshTimerList(_hListTimer, _totalStock, _nStockTimer, _CurrentSelect, _TopIndex);
}


/* Notify with MessageBox */
VOID NotifyTimer(HWND hwnd, TimerSt* _totalStock, int _nStockTimer)
{
    for(int i = 0; i < _nStockTimer; ++i){
        if(_totalStock[i].is_zero == false){
            continue;
        }
        _totalStock[i].is_zero = false;
        MessageBox(hwnd, TEXT("TIMER!!!                 "),
            TEXT("Timer"), MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
    }
}


/* Swap Timer Struct */
VOID SwapStockTimer(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer, bool _is_up)
{
    int _CurrentSelect = SendMessage(_hListTimer, LB_GETCURSEL, 0, 0);

    if(_is_up && _CurrentSelect < 1)
        return;
    if(!_is_up && _CurrentSelect + 1 > _nStockTimer - 1)
        return;

    TimerSt _tmpSwap;
    memcpy(&_tmpSwap,  &_totalStock[_CurrentSelect], sizeof(TimerSt));

    if(_is_up){
        memcpy(&_totalStock[_CurrentSelect], &_totalStock[_CurrentSelect - 1], sizeof(TimerSt));
        memcpy(&_totalStock[_CurrentSelect - 1], &_tmpSwap, sizeof(TimerSt));
    }
    else{
        memcpy(&_totalStock[_CurrentSelect], &_totalStock[_CurrentSelect + 1], sizeof(TimerSt));
        memcpy(&_totalStock[_CurrentSelect + 1], &_tmpSwap, sizeof(TimerSt));
    }

    int _TopIndex = SendMessage(_hListTimer, LB_GETTOPINDEX, 0, 0);

    if(_is_up)
        _TopIndex -= (_TopIndex == _CurrentSelect);
    else
        _TopIndex += (_CurrentSelect - _TopIndex == 12);

    _CurrentSelect += ((_is_up) ? -1 : 1);

    RefreshTimerList(_hListTimer, _totalStock, _nStockTimer, _CurrentSelect, _TopIndex);
}


/* Delete Timer */
VOID DeleteTimer(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer)
{
    int _CurrentSelect = SendMessage(_hListTimer, LB_GETCURSEL, 0, 0);

    for(int i = _CurrentSelect; i < _nStockTimer - 1; ++i)
        memcpy(&_totalStock[i], &_totalStock[i + 1], sizeof(TimerSt));

    memset(&_totalStock[_nStockTimer - 1], 0, sizeof(TimerSt));

    int _TopIndex = SendMessage(_hListTimer, LB_GETTOPINDEX, 0, 0);

    _CurrentSelect -= (_CurrentSelect >= _nStockTimer - 1);

    RefreshTimerList(_hListTimer, _totalStock, _nStockTimer - 1, _CurrentSelect, _TopIndex);
}


/* Refresh Timer List */
VOID RefreshTimerList(HWND _hListTimer, TimerSt* _totalStock, int _nStockTimer, int _CurrentSelect, int _TopIndex)
{
    SendMessage(_hListTimer, WM_SETREDRAW, FALSE, 0);

    SendMessage(_hListTimer, LB_RESETCONTENT , 0, 0);

    TCHAR _strtmp[32];
    for(int i = 0; i < _nStockTimer; ++i){
        SetTimer_toString(&_totalStock[i], _strtmp);
        SendMessage(_hListTimer, LB_INSERTSTRING, i, (LPARAM)_strtmp);
    }

    SendMessage(_hListTimer, LB_SETCARETINDEX, _CurrentSelect, 0);
    SendMessage(_hListTimer, LB_SETCURSEL, _CurrentSelect, 0);
    SendMessage(_hListTimer, LB_SETTOPINDEX, _TopIndex, 0);

    SendMessage(_hListTimer, WM_SETREDRAW, TRUE, 0);
}



/* ----------------------- In Dialog Procedure --------------------------------------- */

/* Get Handle ComboBox(h, m, s) */
VOID GetComboBoxHandle(HWND hwnd, HWND* _hHour, HWND* _hMinute, HWND* _hSecond)
{
    *_hHour   = GetDlgItem(hwnd, CB_HOUR);
    *_hMinute = GetDlgItem(hwnd, CB_MINUTE);
    *_hSecond = GetDlgItem(hwnd, CB_SECOND);
}


/* Send 0~59 to ComboBox(h, m, s) */
VOID InitNewDlgComboBox(HWND hwnd, HWND _hHour, HWND _hMinute, HWND _hSecond)
{
    static TCHAR _strtemp[4];
    for(int i = 0; i < 24; ++i){
        wsprintf(_strtemp, "%dh", i);
        SendMessage(_hHour, CB_ADDSTRING, 0, (LPARAM)_strtemp);
    }
    for(int i = 0; i < 60; ++i){
        wsprintf(_strtemp, "%dm", i);
        SendMessage(_hMinute, CB_ADDSTRING, 0, (LPARAM)_strtemp);
    }
    for(int i = 0; i < 60; ++i){
        wsprintf(_strtemp, "%ds", i);
        SendMessage(_hSecond, CB_ADDSTRING, 0, (LPARAM)_strtemp);
    }

    SendMessage(  _hHour, CB_SETCURSEL, 0, 0);
    SendMessage(_hMinute, CB_SETCURSEL, 0, 0);
    SendMessage(_hSecond, CB_SETCURSEL, 0, 0);
}


/* Get Time Set_ed */
INT GetTimeSet_Ed(HWND hwnd, HWND _hHour, HWND _hMinute, HWND _hSecond, bool _is_Alarm)
{
    int _nHour, _nMinute, _nSecond;
    _nHour   = SendMessage(_hHour,   CB_GETCURSEL, 0, 0);
    _nMinute = SendMessage(_hMinute, CB_GETCURSEL, 0, 0);
    _nSecond = SendMessage(_hSecond, CB_GETCURSEL, 0, 0);

    if(_nHour == -1 || _nMinute == -1 || _nSecond == -1)
        return -1;

    if(!_nHour && !_nMinute && !_nSecond && !_is_Alarm)
        return -1;

    int _nReturnValue;
    _nReturnValue  = _nSecond;
    _nReturnValue += _nMinute  <<  8;
    _nReturnValue += _nHour    << 16;
    _nReturnValue += _is_Alarm << 24;

    return _nReturnValue;
}

#include "variable.h"

/* ---------------------  Const Variable ----------------------------- */

/* Control_Position */
// Edit
const int pos_edit[4] = {22, 30, 220, 20};
const int pos_staticEdit[4] = {22, 10, 100, 20};

// Alarm
const int pos_parentAlarm[4]   = { 20,  60, 145, 260};
const int pos_listAlarm[4]     = { 30, 100, 125, 220};
const int pos_staticAlarm[2][4] = {{30, 80,  68,  20}, {98, 80, 57, 20}};

// Timer
const int pos_parentTimer[4] = {170,  60, 105, 260};
const int pos_listTimer[4]   = {180, 100,  85, 220};
const int pos_staticTimer[4] = {180,  80,  85,  20};

// Button
const int pos_parentButton[4] = {280, 10, 110, 310};
const int pos_button[5][4] = {
    {290,  30 +  2, 90, 56 -  2},
    {290, 142 + 12, 90, 56 - 12},
    {290,  86 + 12, 90, 56 - 12},
    {290, 198 + 12, 90, 56 - 12},
    {290, 254 + 12, 90, 56 - 12},
};


/* Button Label */
const TCHAR buttonLabel[5][10] = {TEXT("NEW"), TEXT("ON/OFF"), TEXT("DELETE"), TEXT("UP"), TEXT("DOWN")};


/* Timer Preview */
const LPCTSTR TimerPreview = TEXT("%02d:%02d:%02d");

/* ---------------- End Const Variable ------------------------------- */

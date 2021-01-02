#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED
#include <windows.h>

/* ---------------------  Const Variable ----------------------------- */

/* Control_Position */
// Edit
extern const int pos_edit[4];
extern const int pos_staticEdit[4];

// Alarm
extern const int pos_parentAlarm[4];
extern const int pos_listAlarm[4];
extern const int pos_staticAlarm[2][4];

// Timer
extern const int pos_parentTimer[4];
extern const int pos_listTimer[4];
extern const int pos_staticTimer[4];

// Button
extern const int pos_parentButton[4];
extern const int pos_button[5][4];


/* Button Label */
extern const TCHAR buttonLabel[5][10];


/* Timer Preview */
extern const LPCTSTR TimerPreview;

/* ---------------- End Const Variable ------------------------------- */

#endif // VARIABLE_H_INCLUDED

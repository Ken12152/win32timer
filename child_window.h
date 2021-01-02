#ifndef CHILD_WINDOW_H_INCLUDED
#define CHILD_WINDOW_H_INCLUDED
#include <windows.h>

HWND CreateEditChild(HWND hwnd, const int _pos[4], int _nID);
HWND CreateButtonChild(HWND hwnd, const int _pos[4], int _nID, LPCTSTR _lpszText);
HWND CreateStaticChild(HWND hwnd, const int _pos[4], int _nID, LPCTSTR _lpszText, BOOL _is_border);

HWND CreateListBoxChild(HWND hwnd, const int _pos[4], int _nID, BOOL _is_Sort);
HWND CreateComboBoxChild(HWND hwnd, const int _pos[4], int _nID, LONG _style);

HWND CreateGroupBox(HWND hwnd, const int _pos[4], LPCTSTR _lpGroupType, LPCTSTR _lpTitle);

#endif // CHILD_WINDOW_H_INCLUDED

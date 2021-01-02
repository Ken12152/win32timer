#include "child_window.h"

/* Edit */
HWND CreateEditChild(HWND hwnd, const int _pos[4], int _nID)
{
	HWND _hEdit;
	HMENU _id = (HMENU)_nID;
	_hEdit = CreateWindow(TEXT("EDIT"), NULL,
						 WS_CHILD | WS_VISIBLE |
						 //WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE |
						 ES_LEFT | WS_BORDER, // | ES_READONLY,
						 _pos[0], _pos[1], _pos[2], _pos[3],
						 hwnd, _id, GetModuleHandle(NULL), NULL);

	return _hEdit;
}


/* Button */
HWND CreateButtonChild(HWND hwnd, const int _pos[4], int _nID, LPCTSTR _lpszText)
{
	HWND _hButton;
	HMENU _id = (HMENU)_nID;
	_hButton = CreateWindow(TEXT("BUTTON"), _lpszText,
						   WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
						   _pos[0], _pos[1], _pos[2], _pos[3],
						   hwnd, _id, GetModuleHandle(NULL), NULL);

	return _hButton;
}


/* Static */
HWND CreateStaticChild(HWND hwnd, const int _pos[4], int _nID, LPCTSTR _lpszText, BOOL _is_border)
{
	HWND _hStatic;
	HMENU _id = (HMENU)_nID;
	_hStatic = CreateWindow(TEXT("STATIC"), _lpszText,
						   WS_CHILD | WS_VISIBLE | ((_is_border) ? WS_BORDER : WS_VISIBLE),
						   _pos[0], _pos[1], _pos[2], _pos[3],
						   hwnd, _id, GetModuleHandle(NULL), NULL);
	return _hStatic;
}


/* ListBox */
HWND CreateListBoxChild(HWND hwnd, const int _pos[4], int _nID, BOOL _is_Sort)
{
	HWND _hListBox;
	HMENU _id = (HMENU)_nID;
	_hListBox = CreateWindow(TEXT("LISTBOX"), NULL,
						   WS_CHILD | WS_VISIBLE | ((_is_Sort) ? LBS_STANDARD :
						   (WS_BORDER | WS_VSCROLL | LBS_NOTIFY)) | LBS_DISABLENOSCROLL,
						   _pos[0], _pos[1], _pos[2], _pos[3],
						   hwnd, _id, GetModuleHandle(NULL), NULL);
	return _hListBox;
}


/* ComboBox */
HWND CreateComboBoxChild(HWND hwnd, const int _pos[4], int _nID, LONG _style)
{
	HWND _hComboBox;
	HMENU _id = (HMENU)_nID;
	_hComboBox = CreateWindow(TEXT("COMBOBOX"), NULL,
						   WS_CHILD | WS_VISIBLE | CBS_SORT | _style,
						   _pos[0], _pos[1], _pos[2], _pos[3],
						   hwnd, _id, GetModuleHandle(NULL), NULL);
	return _hComboBox;
}


/* Create GroupBox */
HWND CreateGroupBox(HWND hwnd, const int _pos[4], LPCTSTR _lpGroupType, LPCTSTR _lpTitle)
{
    HWND _hGroup;
	_hGroup = CreateWindow(_lpGroupType, _lpTitle,
						 WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
						 _pos[0], _pos[1], _pos[2], _pos[3],
						 hwnd, NULL, GetModuleHandle(NULL), NULL);

	return _hGroup;
}

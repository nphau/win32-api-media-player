#pragma once

#include "resource.h"
#include "MediaLib.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

/*
 * The process which is using for playing songs
 */
BOOL OnPlay()
{
	if (hMCIWnd != NULL)
	{
		MCIWndPlay(hMCIWnd);
		return TRUE;
	}
	return FALSE;
}

/*
* The process which is using for opening songs
*/
BOOL OnOpen(HWND hDlg)
{
	// Check if It can be open
	if (IsOpenNewSong(hDlg) == TRUE)
	{
		if (hMCIWnd != NULL)
			MCIWndOpenDialog(hMCIWnd);
		
		return TRUE;
	}
	return FALSE;
}

/*
* The process which is using for stopping songs
*/
BOOL OnStop()
{
	if (hMCIWnd != NULL)
	{
		MCIWndStop(hMCIWnd);
		return TRUE;
	}
	return FALSE;
}


void OnPrev()
{

}



void OnNext()
{

}


BOOL OnVolUp()
{
	LONG volNum = MCIWndGetVolume(hMCIWnd) + deltaVol;
	if (volNum < 1000)
	{
		MCIWndSetVolume(hMCIWnd, volNum);
		return TRUE;
	}
	return FALSE;
}

BOOL OnVolDown()
{
	LONG volNum = MCIWndGetVolume(hMCIWnd) - deltaVol;
	if (volNum > 0)
	{
		MCIWndSetVolume(hMCIWnd, volNum);
		return TRUE;
	}
	return FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK MediaPlayer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		// To create a new MCIWndClass
		return OnInitCreateWnd(hDlg, hInst);
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			SendMessage(GetParent(hDlg), WM_DESTROY, 0, 0);
			break;
		case IDC_BTN_OPEN:
			// Open new file
			if (OnOpen(hDlg) == TRUE)
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_PLAY), TRUE);
			break;
		case IDC_BTN_STOP:
			if (OnStop() == TRUE)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_STOP), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_PREV), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_NEXT), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLUP), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLDOWN), FALSE);
			}
			break;
		case IDC_BTN_PREV:
		
			break;
		case IDC_BTN_PLAY:
			if (OnPlay() == TRUE)
			{
				SetDlgItemText(GetDlgItem(hDlg, IDC_BTN_PLAY), IDC_BTN_PLAY, L"Pause");
				
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_STOP), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_PREV), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_NEXT), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLUP), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLDOWN), TRUE);
			}
			break;
		case IDC_BTN_NEXT:
			OnNext();
			break;
		case IDC_BTN_VOLUP:
			OnVolUp();
			break;
		case IDC_BTN_VOLDOWN:
			OnVolDown();
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

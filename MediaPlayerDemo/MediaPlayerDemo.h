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
 * Identify the mode of Devides
 */
BOOL IsPlayOrPause()
{
	TCHAR * szMode = new TCHAR[100];
	LONG  IsMode = MCIWndGetMode(hMCIWnd, szMode, 100);
	switch (IsMode)
	{
	case MCI_MODE_OPEN: case MCI_MODE_PAUSE: case MCI_MODE_STOP:
		MCIWndPlay(hMCIWnd);
		return TRUE;
	case MCI_MODE_PLAY:
		MCIWndPause(hMCIWnd);
		return FALSE;
	}
}

/*
* The process which is using for opening songs
*/
BOOL OnOpen(HWND hDlg)
{
	// Check if It can be open
	if (IsOpenNewSong(hDlg) == TRUE)
	{
		SendMessage(hDlg, WM_COMMAND, IDC_BTN_STOP, 0);

		if (hMCIWnd != NULL)
		{
			if (GetImageUrl(hDlg, szFileName) == FALSE)
				return FALSE;

			if (szFileName == NULL)
			{
				szFileName = new WCHAR[MAX_PATH];
				wcscpy(szFileName, szTitle);
			}
			else
				MCIWndOpen(hMCIWnd, szFileName, MAX_PATH);

			//if (MCIWndOpenDialog(hMCIWnd) == 0)
			//{
			//	// Get file name
			//	MCIWndGetFileName(hMCIWnd, szFileName, MAX_PATH);
			//}
			//else
			//{
			//	wcscpy(szFileName, szTitle);
			//}
		}
		return TRUE;
	}
	return FALSE;
}

/*
* The process which is using for stopping songs
*/
BOOL OnStop()
{
	MCIWndStop(hMCIWnd);
	MCIWndSeek(hMCIWnd, MCIWND_START);
	return TRUE;
}

void OnPrev()
{
	long seek = MCIWndGetPosition(hMCIWnd) - deltaForward;
	if (seek > 0)
	{
		MCIWndPause(hMCIWnd);
		MCIWndSeek(hMCIWnd, seek);
		MCIWndPlay(hMCIWnd);
	}
}

void OnNext()
{
	long seek = MCIWndGetPosition(hMCIWnd) + deltaForward;
	if (seek < MCIWndGetEnd(hMCIWnd))
	{
		MCIWndPause(hMCIWnd);
		MCIWndSeek(hMCIWnd, seek);
		MCIWndPlay(hMCIWnd);
	}
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

	HANDLE hIcon = NULL;
	switch (message)
	{
	case WM_INITDIALOG:

		// Set Icon for Dialog
		hIcon = LoadImage(hInst,
			MAKEINTRESOURCE(IDI_SMALL),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			0);

		if (hIcon) SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

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
			{
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_PLAY), TRUE);
				SetWindowTextW(hDlg, szFileName);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLUP), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLDOWN), TRUE);
				// To force press Play Button
				SendMessage(hDlg, WM_COMMAND, IDC_BTN_PLAY, 0);
			}
			break;
		case IDC_BTN_STOP:
			if (OnStop() == TRUE)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_STOP), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_PREV), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_NEXT), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLUP), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLDOWN), FALSE);
				SetDlgItemTextW(hDlg, IDC_BTN_PLAY, L"Play");
			}
			break;
		case IDC_BTN_PREV:
			OnPrev();
			break;
		case IDC_BTN_PLAY:
			// Play
			if (IsPlayOrPause() == TRUE)
				SetDlgItemTextW(hDlg, IDC_BTN_PLAY, L"Pause");
			else
				SetDlgItemTextW(hDlg, IDC_BTN_PLAY, L"Play");

			EnableWindow(GetDlgItem(hDlg, IDC_BTN_STOP), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_PREV), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_NEXT), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLUP), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_VOLDOWN), TRUE);
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

void OnDestroy()
{
	// Closes an MCI device or file associated
	if (hMCIWnd != NULL)
		MCIWndDestroy(hMCIWnd);
	if (szFileName != NULL)
		delete szFileName;

	PostQuitMessage(0);
}
#pragma once

#include "resource.h"
#include <commdlg.h>
#include <Vfw.h>

/*
 * MCIWndEnd: macro moves the current position to the end of the content.
 * You can use this macro or explicitly send the MCI_SEEK message
 * MCIWndGetStart: macro retrieves the location of the beginning of the content of an MCI device or file.
 * You can use this macro or explicitly send the MCIWNDM_GETSTART message.
 * MCIWndGetLength: macro retrieves the length of the content or file currently used by an MCI device.
 * You can use this macro or explicitly send the MCIWNDM_GETLENGTH message.
 * MCIWndOpen: macro opens an MCI device and associates it with an MCIWnd window.
 * For MCI devices that use data files, this macro can also open a specified data file,
 * name a new file to be created, or display a dialog box to let the user select a file to open.
 * You can use this macro or explicitly send the MCIWNDM_OPEN message.
 * MCIWndOpenDialog: macro opens a user-specified data file and corresponding type of MCI device
 * and associates them with an MCIWnd window. This macro displays the Open dialog box for
 * the user to select the data file to associate with an MCI window.
 * You can use this macro or explicitly send the MCIWNDM_OPEN message.
 * MCIWndGetVolume: macro retrieves the current volume setting of an MCI device.
 * You can use this macro or explicitly send the MCIWNDM_GETVOLUME message.
 * MCIWndGetFileName: macro retrieves the filename used by an MCI device.
 * You can use this macro or explicitly send the MCIWNDM_GETFILENAME message.
 * MCIWndPlayTo:
 * macro plays the content of an MCI device from the current position to the specified ending
 * location or until another command stops playback.
 * If the specified ending location is beyond the end of the content,
 * playback stops at the end of the content.
 * MCIWndPlayTo macro plays the content of an MCI device from the current position to the specified ending location or until another command stops playback.
 * If the specified ending location is beyond the end of the content, playback stops at the end of the content. You can use this macro or explicitly send the MCIWNDM_PLAYTO message.
 * You can use this macro or explicitly send the MCIWNDM_PLAYTO message.
 * MCIWndSeek: macro moves the playback position to the specified location in the content.
 * You can use this macro or explicitly use the MCI_SEEK command.
 * MCIWndStep: macro moves the current position in the content forward or backward by a specified increment.
 * You can use this macro or explicitly send the MCI_STEP command.
 */


static HWND hMCIWnd = NULL;
static long deltaVol = 200;
static long deltaForward = 50;
static WCHAR * szFileName = NULL;

// Whether it can be open new song or not
BOOL IsOpenNewSong(HWND hDlg)
{
	TCHAR * szMode = new TCHAR[100];
	LONG  IsMode = MCIWndGetMode(hMCIWnd, szMode, 100);

	// Playing ?
	if (IsMode == MCI_MODE_PLAY)
	{
		// Ask user to open new song
		// Yes: stopping current song and opening a new song
		// No: Go on
		int msgBoxId = MessageBox(
			hDlg,
			L"Music/video is playing. Do you want to play a new music/Video?",
			L"Play", MB_YESNO | MB_ICONQUESTION);
		switch (msgBoxId)
		{
		case IDYES:
			return TRUE;
		case IDNO:
			return FALSE;
		}
	}
	return TRUE;
}

BOOL OnInitCreateWnd(HWND hDlg, HINSTANCE hInst)
{
	hMCIWnd = MCIWndCreate(
		hDlg,					// Handle to the parent window.
		hInst,					// Handle to the module instance to associate with the MCIWnd window.
		// Flags defining the window style.
		// In addition to specifying the window styles used with the CreateWindowEx function,
		// you can specify the following styles to use with MCIWnd windows
		WS_CHILD | WS_VISIBLE |    // standard styles
		MCIWNDF_NOPLAYBAR |        // hides toolbar 
		MCIWNDF_NOTIFYMODE | MCIWNDF_NOTIFYPOS,
		NULL	// Null-terminated string indicating the name of an MCI device or data file to open.
		);

	// Khoi tao thanh cong
	if (hMCIWnd != NULL) return TRUE;

	return FALSE;
}

BOOL GetImageUrl(HWND hWnd, WCHAR* &path)
{
	OPENFILENAMEW ofn;

	WCHAR szFile[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Mp3 files (*.mp3)\0*.mp3\0Avi files (*.avi)\0*.avi\0Mp4 files (*.mp4)\0*.mp4\0All Files (*.*)\0*.*\0";

	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrTitle = L"Chọn file media";
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L"txt";

	path = NULL;

	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		path = new WCHAR[MAX_PATH];
		wcscpy(path, szFile);
		return TRUE;
	}
	return FALSE;
}

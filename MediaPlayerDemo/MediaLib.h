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
*/


static HWND hMCIWnd = NULL;
static long deltaVol = 200;

// Check if this song is playing or not
BOOL IsStillPlaying()
{
	return FALSE;
}

// Whether it can be open new song or not
BOOL IsOpenNewSong(HWND hDlg)
{
	// If the song is playing
	// Ask user
	if (IsStillPlaying() == TRUE)
	{
		// Ask user to open new song
		// Yes: stopping current song and opening a new song
		// No: Go on
		int msgBoxId = MessageBox(
			hDlg,
			L"Bài hát đang được phát. Bạn có muốn play bài mới không?",
			L"Play", MB_YESNO);
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

/* 
 * To create a new MCIWndClass
 * Yes: success
 * No: fail
 */
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
		MCIWNDF_NOTIFYMODE,
		NULL	// Null-terminated string indicating the name of an MCI device or data file to open.
		);

	// Khoi tao thanh cong
	if (hMCIWnd != NULL) return TRUE;

	return FALSE;
}
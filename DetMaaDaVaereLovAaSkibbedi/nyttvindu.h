#pragma once

// Globale variabler.
RECT DesktopRect;
WNDCLASSEX wc;
HINSTANCE hInst;
bool GlassBroken = false;

// Mulighet for flere bilder.
struct bilde {
	const wchar_t* navn;
	int w;
	int h;
};
bilde bilder[] = {
	{L"TOILETBILDE_1", 462, 500}
};
size_t antall_bilder = sizeof(bilder)/sizeof(bilder[0]);

// Message handler for alle vindu, trenger bare en felles basic.
LRESULT __stdcall WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}

void nyttvindu()
{
	bilde valgt_bilde = bilder[rand() % antall_bilder];
	int bilde_x = valgt_bilde.w;
	int bilde_y = valgt_bilde.h;

	HWND nyHWND = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, 
		L"MainClass", 
		L"Skibbedi", 
		WS_POPUP, 
		0, 
		0, 
		bilde_x, bilde_y, 
		NULL, 
		NULL, 
		hInst, 
		NULL
	);

	if (!nyHWND) {
		GetError(L"Error during creating window.");
		exit(EXIT_FAILURE);
	}

	ShowWindow(nyHWND, SW_SHOW);
	UpdateWindow(nyHWND);
	SetLayeredWindowAttributes(nyHWND, RGB(255, 255, 255), NULL, LWA_COLORKEY);

	// Tilfeldig synlig sted på skjermen.
	GetClientRect(GetDesktopWindow(), &DesktopRect);
	int skjerm_max_x = DesktopRect.right;
	int skjerm_max_y = DesktopRect.bottom;
	int x = rand() % skjerm_max_x;
	int y = rand() % skjerm_max_y;
	if ((skjerm_max_x - x) < bilde_x)
		x -= bilde_x;
	if ((skjerm_max_y - y) < bilde_y)
		y -= bilde_y;

	// Pass på at vindu alltid er øverst, og oppdater lokasjon.
	SetWindowPos(
		nyHWND,
		HWND_TOPMOST,
		x,
		y,
		bilde_x,
		bilde_y,
		SWP_SHOWWINDOW
	);

	HDC nyHDC = GetDC(nyHWND);
	HDC BufferDC = CreateCompatibleDC(nyHDC);

	HBITMAP BufferBitmap = (HBITMAP)LoadImage(
		GetModuleHandle(NULL),
		valgt_bilde.navn,
		IMAGE_BITMAP,
		0,
		0,
		LR_DEFAULTCOLOR
	);
	SelectObject(BufferDC, BufferBitmap);

	BitBlt(nyHDC, 0, 0, bilde_x, bilde_y, BufferDC, 0, 0, SRCCOPY);
	DeleteObject(BufferBitmap);
	DeleteDC(BufferDC);
	ReleaseDC(nyHWND, nyHDC);

	PlaySound(
		MAKEINTRESOURCE(SKIBBEDI_LYD1),
		GetModuleHandle(NULL),
		SND_RESOURCE | SND_ASYNC | SND_NODEFAULT
	);
}
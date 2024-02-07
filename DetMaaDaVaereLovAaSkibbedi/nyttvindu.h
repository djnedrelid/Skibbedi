#pragma once

// Globale variabler.
RECT DesktopRect;
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

// Message handler for hovedvindu.
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

// Message handler for skibbedi vinduer.
LRESULT __stdcall SkibbediWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}

void nyttvindu()
{
	srand((unsigned int)time(0));
	bilde valgt_bilde = bilder[rand() % antall_bilder];
	int bilde_x = valgt_bilde.w;
	int bilde_y = valgt_bilde.h;

	HWND nyHWND = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, 
		L"SkibbediClass", 
		L"Skibbedi", 
		WS_POPUP, 
		0, 
		0, 
		bilde_x, bilde_y, 
		0, 
		0, 
		hInst, 
		0
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

	// Skibbedi sang.
	PlaySound(
		MAKEINTRESOURCE(SKIBBEDI_LYD1),
		GetModuleHandle(NULL),
		SND_RESOURCE | SND_ASYNC | SND_NODEFAULT
	);

	Sleep(8000);
	SendMessage(nyHWND, WM_CLOSE, 0, 0);
}

void RegistrerVinduKlasser()
{
	WNDCLASSEX wc;

	// Hovedvindu.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IKON));
	wc.hIconSm = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"MainClass";

	if (!RegisterClassEx(&wc)) {
		GetError(L"Feil under registrering av vinduklasse 1.");
		exit(1);
	}

	// Skibbedivindu.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)SkibbediWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IKON));
	wc.hIconSm = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"SkibbediClass";

	if (!RegisterClassEx(&wc)) {
		GetError(L"Feil under registrering av vinduklasse 2.");
		exit(1);
	}
}
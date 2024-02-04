#include <Windows.h>
#include <string>
#include <time.h>
#include "geterror.h"
#include "resource.h"
#include "nyttvindu.h"

void Avslutt()
{
	PlaySound(
		MAKEINTRESOURCE(DML),
		GetModuleHandle(NULL),
		SND_RESOURCE | SND_NODEFAULT
	);
	exit(0);
}

int __stdcall wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR lpCmdLine,
	int nCmdShow
) {
	MSG Msg;
	hInst = hInstance;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IKON));
	wc.hIconSm = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"MainClass";

	if (!RegisterClassEx(&wc)) {
		GetError(L"Feil under registrering av vinduklasse.");
		exit(1);
	}

	// Hovedloop.
	do {
		Sleep(10); // CPU. 

		// Håndter systemmeldinger til vinduet, diskré.
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// Knusekombo.
		if (
			(GetAsyncKeyState(0x41) & 0x8000) && // A
			(GetAsyncKeyState(0x44) & 0x8000) && // D
			(GetAsyncKeyState(0x57) & 0x8000) && // W
			!GlassBroken
		) {
			srand((unsigned int)time(0));
			try { nyttvindu(); }
			catch (...) {
				GetError(L"Feil under oppdatering av projisering.");
				exit(EXIT_FAILURE);
			}
			GlassBroken = true;
		}

		if (
			(GetAsyncKeyState(0x41) & 0x8000) == 0 && // A
			(GetAsyncKeyState(0x44) & 0x8000) == 0 && // D
			(GetAsyncKeyState(0x57) & 0x8000) == 0 // W
		) {
			GlassBroken = false;
		}

		// FIX combo for DML lyd og avslutning.
		if (
			(GetAsyncKeyState(0x4C) & 0x8000) && // L
			(GetAsyncKeyState(0x4F) & 0x8000) && // O
			(GetAsyncKeyState(0x56) & 0x8000)    // V
		) {
			Avslutt();
			break;
		}

	} while (Msg.message != WM_QUIT);

	return (int)Msg.wParam;
}

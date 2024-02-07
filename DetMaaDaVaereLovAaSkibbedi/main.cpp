#include <Windows.h>
#include <string>
#include <time.h>
#include <thread>
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
	RegistrerVinduKlasser();

	// Hovedloop.
	while (1) { 

		// Håndter systemmeldinger til vinduet, diskré.
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// Avslutt hvis WM_QUIT har blitt postet.
		if (Msg.message == WM_QUIT)
			break;

		// Knusekombo.
		if (
			(GetAsyncKeyState(0x41) & 0x8000) && // A
			(GetAsyncKeyState(0x44) & 0x8000) && // D
			(GetAsyncKeyState(0x57) & 0x8000) && // W
			!GlassBroken
		) {
			std::thread nv (nyttvindu);
			nv.detach();
			GlassBroken = true;
		}

		if (
			(GetAsyncKeyState(0x41) & 0x8000) == 0 && // A
			(GetAsyncKeyState(0x44) & 0x8000) == 0 && // D
			(GetAsyncKeyState(0x57) & 0x8000) == 0 // W
		) {
			GlassBroken = false;
		}

		// Kombo for DML lyd og avslutning.
		if (
			(GetAsyncKeyState(0x4C) & 0x8000) && // L
			(GetAsyncKeyState(0x4F) & 0x8000) && // O
			(GetAsyncKeyState(0x56) & 0x8000)    // V
		) {
			Avslutt();
			break;
		}

		Sleep(10); // CPU.
	}

	return (int)Msg.wParam;
}

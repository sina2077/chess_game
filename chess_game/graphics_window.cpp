// graphics_window.cpp - see graphics_window.h for why this exists.
#include "graphics_window.h"
#include <process.h>
#include <string.h>
#include <stdlib.h>

namespace {
	const COLORREF BACKGROUND = RGB(12, 12, 12);     // same black as the old console
	const UINT_PTR REFRESH_TIMER = 1;
	const UINT REFRESH_MS = 15;
	const char WINDOW_CLASS[] = "ChessGameCanvasWindow";

	// Canvas (written by the game thread through g_canvas_dc).
	HDC g_canvas_dc = NULL;
	HBITMAP g_canvas_bitmap = NULL;
	DWORD* g_canvas_bits = NULL;
	BITMAPINFO g_bmi;

	// Snapshot of the canvas the window actually paints (UI thread only).
	DWORD* g_shown_bits = NULL;
	const size_t CANVAS_BYTES = size_t(gfx::CANVAS_WIDTH) * gfx::CANVAS_HEIGHT * sizeof(DWORD);

	HWND g_hwnd = NULL;
	HANDLE g_window_ready = NULL;
	DWORD g_game_thread_id = 0;
	bool g_window_visible = false;

	void make_dpi_aware() {
		// Per-monitor DPI awareness gives a sharp picture at any scaling and
		// makes cursor/window coordinates consistent physical pixels.
		typedef BOOL(WINAPI* SetCtxFn)(HANDLE);
		HMODULE user32 = GetModuleHandleA("user32.dll");
		SetCtxFn set_ctx = user32 ? (SetCtxFn)GetProcAddress(user32, "SetProcessDpiAwarenessContext") : NULL;
		if (!set_ctx || !set_ctx((HANDLE)-4 /* PER_MONITOR_AWARE_V2 */))
			SetProcessDPIAware();
	}

	// Where the canvas goes inside the client area: scaled to fit, centered.
	RECT canvas_rect(HWND hwnd) {
		RECT client;
		GetClientRect(hwnd, &client);
		int cw = client.right, ch = client.bottom;
		int w = cw, h = MulDiv(cw, gfx::CANVAS_HEIGHT, gfx::CANVAS_WIDTH);
		if (h > ch) {
			h = ch;
			w = MulDiv(ch, gfx::CANVAS_WIDTH, gfx::CANVAS_HEIGHT);
		}
		RECT r;
		r.left = (cw - w) / 2;
		r.top = (ch - h) / 2;
		r.right = r.left + w;
		r.bottom = r.top + h;
		return r;
	}

	void paint(HWND hwnd) {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hwnd, &ps);
		RECT client;
		GetClientRect(hwnd, &client);
		RECT r = canvas_rect(hwnd);

		// Draw into a back buffer so resizing/repainting never flickers.
		HDC back = CreateCompatibleDC(dc);
		HBITMAP back_bmp = CreateCompatibleBitmap(dc, client.right, client.bottom);
		HGDIOBJ old_bmp = SelectObject(back, back_bmp);

		HBRUSH bg = CreateSolidBrush(BACKGROUND);
		FillRect(back, &client, bg);
		DeleteObject(bg);

		SetStretchBltMode(back, HALFTONE);
		SetBrushOrgEx(back, 0, 0, NULL);
		StretchDIBits(back, r.left, r.top, r.right - r.left, r.bottom - r.top,
			0, 0, gfx::CANVAS_WIDTH, gfx::CANVAS_HEIGHT,
			g_shown_bits, &g_bmi, DIB_RGB_COLORS, SRCCOPY);

		BitBlt(dc, 0, 0, client.right, client.bottom, back, 0, 0, SRCCOPY);
		SelectObject(back, old_bmp);
		DeleteObject(back_bmp);
		DeleteDC(back);
		EndPaint(hwnd, &ps);
	}

	// Copies the canvas to the on-screen snapshot if the game drew something.
	void refresh(HWND hwnd) {
		if (memcmp(g_shown_bits, g_canvas_bits, CANVAS_BYTES) == 0)
			return;
		memcpy(g_shown_bits, g_canvas_bits, CANVAS_BYTES);
		InvalidateRect(hwnd, NULL, FALSE);
		if (!g_window_visible) {
			// First drawing (after the console menu): show the board.
			g_window_visible = true;
			ShowWindow(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
		}
	}

	LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {
		case WM_TIMER:
			if (wp == REFRESH_TIMER)
				refresh(hwnd);
			return 0;
		case WM_ERASEBKGND:
			return 1;   // everything is painted in WM_PAINT
		case WM_PAINT:
			paint(hwnd);
			return 0;
		case WM_SIZE:
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		case WM_DPICHANGED: {
			const RECT* s = (const RECT*)lp;
			SetWindowPos(hwnd, NULL, s->left, s->top, s->right - s->left, s->bottom - s->top,
				SWP_NOZORDER | SWP_NOACTIVATE);
			return 0;
		}
		case WM_CLOSE:
			// The game loop never ends by itself; closing the board quits.
			ExitProcess(0);
		}
		return DefWindowProcA(hwnd, msg, wp, lp);
	}

	unsigned __stdcall window_thread(void*) {
		HINSTANCE inst = GetModuleHandleA(NULL);
		WNDCLASSEXA wc = { sizeof(wc) };
		wc.lpfnWndProc = window_proc;
		wc.hInstance = inst;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.lpszClassName = WINDOW_CLASS;
		RegisterClassExA(&wc);

		// Size the window to fit ~90% of the work area, keeping the aspect ratio.
		RECT work;
		SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
		int max_w = (work.right - work.left) * 9 / 10;
		int max_h = (work.bottom - work.top) * 9 / 10;
		DWORD style = WS_OVERLAPPEDWINDOW;
		RECT frame = { 0, 0, 0, 0 };
		AdjustWindowRectEx(&frame, style, FALSE, 0);
		int client_w = max_w - (frame.right - frame.left);
		int client_h = MulDiv(client_w, gfx::CANVAS_HEIGHT, gfx::CANVAS_WIDTH);
		if (client_h > max_h - (frame.bottom - frame.top)) {
			client_h = max_h - (frame.bottom - frame.top);
			client_w = MulDiv(client_h, gfx::CANVAS_WIDTH, gfx::CANVAS_HEIGHT);
		}
		int win_w = client_w + (frame.right - frame.left);
		int win_h = client_h + (frame.bottom - frame.top);
		int x = work.left + ((work.right - work.left) - win_w) / 2;
		int y = work.top + ((work.bottom - work.top) - win_h) / 2;

		g_hwnd = CreateWindowExA(0, WINDOW_CLASS, "Chess", style, x, y, win_w, win_h,
			NULL, NULL, inst, NULL);

		// The game reads the keyboard with GetKeyState() from its own thread;
		// sharing input state lets it see keys pressed in this window.
		AttachThreadInput(GetCurrentThreadId(), g_game_thread_id, TRUE);

		SetTimer(g_hwnd, REFRESH_TIMER, REFRESH_MS, NULL);
		SetEvent(g_window_ready);

		MSG msg;
		while (GetMessageA(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return 0;
	}
}

namespace gfx {
	HDC create_surface() {
		if (g_canvas_dc)
			return g_canvas_dc;

		make_dpi_aware();

		// 32-bit top-down DIB the game draws into.
		ZeroMemory(&g_bmi, sizeof(g_bmi));
		g_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		g_bmi.bmiHeader.biWidth = CANVAS_WIDTH;
		g_bmi.bmiHeader.biHeight = -CANVAS_HEIGHT;
		g_bmi.bmiHeader.biPlanes = 1;
		g_bmi.bmiHeader.biBitCount = 32;
		g_bmi.bmiHeader.biCompression = BI_RGB;

		HDC screen = GetDC(NULL);
		g_canvas_dc = CreateCompatibleDC(screen);
		g_canvas_bitmap = CreateDIBSection(screen, &g_bmi, DIB_RGB_COLORS, (void**)&g_canvas_bits, NULL, 0);
		ReleaseDC(NULL, screen);
		SelectObject(g_canvas_dc, g_canvas_bitmap);

		RECT all = { 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT };
		HBRUSH bg = CreateSolidBrush(BACKGROUND);
		FillRect(g_canvas_dc, &all, bg);
		DeleteObject(bg);

		// Flush every GDI call immediately so the window always sees
		// up-to-date pixels (GDI batches calls per thread otherwise).
		GdiSetBatchLimit(1);

		g_shown_bits = (DWORD*)malloc(CANVAS_BYTES);
		memcpy(g_shown_bits, g_canvas_bits, CANVAS_BYTES);

		// Give this (game) thread a message queue so input can be attached.
		MSG msg;
		PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE);
		g_game_thread_id = GetCurrentThreadId();

		g_window_ready = CreateEventA(NULL, TRUE, FALSE, NULL);
		HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, window_thread, NULL, 0, NULL);
		if (thread) {
			WaitForSingleObject(g_window_ready, 5000);
			CloseHandle(thread);
		}
		return g_canvas_dc;
	}

	void get_cursor(POINT* p) {
		GetCursorPos(p);
		if (!g_hwnd)
			return;
		ScreenToClient(g_hwnd, p);
		RECT r = canvas_rect(g_hwnd);
		int w = r.right - r.left, h = r.bottom - r.top;
		if (w <= 0 || h <= 0)
			return;
		p->x = MulDiv(p->x - r.left, CANVAS_WIDTH, w);
		p->y = MulDiv(p->y - r.top, CANVAS_HEIGHT, h);
	}
}

// graphics_window.h
//
// Drawing surface for the chess game.
//
// The game used to paint with GDI straight onto the console window
// (GetDC(GetConsoleWindow())). That no longer works on current Windows:
//   * Windows Terminal (the default console host on Windows 11) gives
//     GetConsoleWindow() a hidden pseudo-window, so nothing shows up at all;
//   * in the classic console, every console repaint (text output, "cls",
//     scrolling, resizing, another window passing over it) wipes the board;
//   * mouse coordinates were hard-coded for 125% display scaling.
//
// This module gives the game an off-screen GDI canvas instead. All existing
// drawing code keeps using the same global HDC; a dedicated game window shows
// that canvas, scaled to fit, and repaints it whenever it changes.
#pragma once
#include <windows.h>

namespace gfx {
	// Logical canvas size. This is the coordinate space the game draws in
	// (board squares go up to x = 1450, y = 1030).
	const int CANVAS_WIDTH = 1480;
	const int CANVAS_HEIGHT = 1060;

	// Creates the off-screen canvas plus the window that displays it, and
	// returns the canvas DC. The window appears as soon as something is drawn.
	HDC create_surface();

	// Mouse position in canvas coordinates (replaces GetCursorPos + the old
	// fixed 1.25 DPI factor and title-bar offset).
	void get_cursor(POINT* p);
}

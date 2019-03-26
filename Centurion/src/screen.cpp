#include "screen.h"
#include <SDL.h>
#include <stdexcept>
#include <windows.h>

using centurion::Screen;

int Screen::GetWidth()
{
	DPI_AWARENESS_CONTEXT context = GetThreadDpiAwarenessContext();
	if (context != DPI_AWARENESS_CONTEXT_SYSTEM_AWARE) {
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	}
	return GetSystemMetrics(SM_CXSCREEN);

}

int Screen::GetHeight()
{
	DPI_AWARENESS_CONTEXT context = GetThreadDpiAwarenessContext();
	if (context != DPI_AWARENESS_CONTEXT_SYSTEM_AWARE) {
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	}
	return GetSystemMetrics(SM_CYSCREEN);
}
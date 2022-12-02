module window

#include "@VROOT/src/win32/window.c"
#flag -luser32 -lgdi32 -lkernel32

fn C.win32_createWindow(width int, height int, title &char)
fn C.win32_run()
fn C.win32_quit()

// A struct representing a cross platform window
pub struct Window {
	width u32
	height u32
	title string
}

// Creates a new window with the specified size and title
pub fn create(width u32, height u32, title string) Window {
	$if windows {
		C.win32_createWindow(int(width), int(height), title.str)
	}

	return Window { width, height, title }
}

// Runs the window
pub fn run() {
	$if windows {
		C.win32_run()
	}
}

// Quits the application and closes the window
pub fn quit() {
	$if windows {
		C.win32_quit()
	}
}

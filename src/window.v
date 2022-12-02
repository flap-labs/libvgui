module window

$if windows {
	#include "@VROOT/src/win32/window.c"
	#include "@VROOT/src/win32/opengl.c"
	#include "@VROOT/external/glad/glad.c"

	#flag -luser32 -lgdi32 -lkernel32 -lopengl32
	#flag -Iexternal
}

$if linux {
	#include "@VROOT/src/linux/window.c"
	#include "@VROOT/src/linux/opengl.c"
	#include "@VROOT/external/glad/glad.c"
	
	#flag -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
	#flag -pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 
	#flag -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0 -I/usr/include/cairo -I/usr/include/pango-1.0 
	#flag -I/usr/include/fribidi -I/usr/include/harfbuzz -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid 
	#flag -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 
	#flag -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -Iexternal -lX11 -lGL -ldl
}

fn C.win32_createWindow(width int, height int, title &char)
fn C.win32_run()

fn C.linux_createWindow(width int, height int, title &char)
fn C.linux_run()

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

	$if linux {
		C.linux_createWindow(int(width), int(height), title.str)
	}

	return Window { width, height, title }
}

// Runs the window
pub fn run() {
	$if windows {
		C.win32_run()
	}

	$if linux {
		C.linux_run()
	}
}

module window

import frame
import image

#include "@VROOT/src/components/frame.c"
#include "@VROOT/src/components/image.c"

$if windows {
	#include "@VROOT/src/win32/window.c"
	#include "@VROOT/src/win32/opengl.c"
	#include "@VROOT/external/glad/glad.c"

	#flag -luser32 -lgdi32 -lkernel32 -lopengl32
	#flag -Iexternal
}

$if macos {
	#include "@VROOT/src/macos/window.m"
	#include "@VROOT/external/glad/glad.c"
	#flag -framework Foundation -framework Cocoa
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
fn C.win32_changeWindowTitle(newTitle &char)
fn C.win32_changeWindowColor(r int, g int, b int)
fn C.win32_addUpdateFunction(update voidptr)
fn C.win32_run()

fn C.macos_createWindow(width int, height int, title &char)
fn C.macos_run()

fn C.linux_createWindow(width int, height int, title &char, frameVisible int)
fn C.linux_changeWindowTitle(newTitle &char)
fn C.linux_changeWindowColor(r int, g int, b int)
fn C.linux_run()
fn C.linux_addUpdateFunction(update voidptr)

fn C.frame_render()
fn C.frame_finalize()

fn C.image_render()
fn C.image_finalize()

pub enum WindowAttributes {
	window_frame_visible
	window_frame_hidden
}

// Local update function
fn update(window Window) {
	C.frame_render()
	for f in window.frames {
		frame.render(f)
	}

	C.image_render()
	for i in window.images {
		image.image_render(i)
	}
}

// A struct representing a cross platform window
pub struct Window {
	width u32
	height u32
mut:
	title string
	frames []frame.Frame
	images []image.Image
}

// Creates a new window with the specified size and title
pub fn create(width u32, height u32, title string, attribs WindowAttributes) &Window {
	mut win := Window { width, height, title, []frame.Frame{}, []image.Image{} }
	mut ref := &win

	$if windows {
		C.win32_createWindow(int(width), int(height), title.str)

		adjusted_update := fn [ref] () {
			update(*ref)
		}

		C.win32_addUpdateFunction(adjusted_update)
	}

	$if macos {
		C.macos_createWindow(int(width), int(height), title.str)
	}

	$if linux {
		if attribs == WindowAttributes.window_frame_visible {
			C.linux_createWindow(int(width), int(height), title.str, int(true))
		}

		if attribs == WindowAttributes.window_frame_hidden {
			C.linux_createWindow(int(width), int(height), title.str, int(false))
		}

		adjusted_update := fn [ref] () {
			update(*ref)
		}

		C.linux_addUpdateFunction(adjusted_update)
	}

	return ref
}

// Changes the window's titlebar text
pub fn change_title(mut window &Window, newTitle string) {
	(*window).title = newTitle
	
	$if windows {
		C.win32_changeWindowTitle(newTitle.str)
	}

	$if linux {
		C.linux_changeWindowTitle(newTitle.str)
	}
}

// Changes the window's titlebar text
pub fn change_background_color(r byte, g byte, b byte) {
	$if windows {
		C.win32_changeWindowColor(int(r), int(g), int(b))
	}
	
	$if linux {
		C.linux_changeWindowColor(int(r), int(g), int(b))
	}
}

// Adds a frame to the window's renderer
pub fn add_frame(mut window &Window, frame frame.Frame) {
	window.frames.insert((*window).frames.len, frame)
}

// Adds an image to the window's renderer
pub fn add_image(mut window &Window, image image.Image) {
	window.images.insert((*window).images.len, image)
}

// Runs the window
pub fn run(window &Window) {
	// C.frame_finalize()
	// C.image_finalize()

	$if windows {
		C.win32_run()
	}

	$if macos {
		C.macos_run()
	}

	$if linux {
		C.linux_run()
	}
}

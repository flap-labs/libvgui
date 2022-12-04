module main

import window
import frame
import image

fn main() {
	mut win := window.create(1280, 720, "Testing VGUI", window.WindowAttributes.window_frame_visible)
	window.run(win)
}

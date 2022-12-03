module main

import window
import frame

fn main() {
	mut win := window.create(1280, 720, "Testing VGUI", window.WindowAttributes.window_frame_visible)

	mut f := frame.new(10, 10, 100, 100)
	window.add_frame(mut win, f)

	mut f2 := frame.new(10, 120, 100, 100)
	window.add_frame(mut win, f2)

	mut f3 := frame.new(10, 230, 100, 100)
	window.add_frame(mut win, f3)

	window.run()
}

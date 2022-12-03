module main

import window
import frame

fn main() {
	mut win := window.create(1280, 720, "Testing VGUI", window.WindowAttributes.window_frame_hidden)

	window.change_background_color(30, 30, 30)
	window.change_title(mut win, "VGUI")

	mut f := frame.new(10, 10, 100, 100)
	frame.change_color(mut f, 255, 0, 0)
	window.add_frame(mut win, f)

	mut f2 := frame.new(10, 120, 100, 100)
	frame.change_color(mut f2, 0, 255, 0)
	window.add_frame(mut win, f2)

	mut f3 := frame.new(10, 230, 100, 100)
	frame.change_color(mut f3, 0, 0, 255)
	window.add_frame(mut win, f3)

	window.run()
}

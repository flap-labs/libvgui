module main

import window
import flex
import frame

fn main() {
	mut win := window.create(1280, 720, "Testing VGUI", window.WindowAttributes.window_frame_visible)

	mut flexbox := flex.new(10, 10)

	test_frame := frame.new(0, 0, 100, 100)
	flex.add_frame(mut flexbox, test_frame)

	test_frame2 := frame.new(0, 0, 100, 100)
	flex.add_frame(mut flexbox, test_frame2)

	test_frame3 := frame.new(0, 0, 100, 100)
	flex.add_frame(mut flexbox, test_frame3)

	flex.set_padding(mut flexbox, 10, 10)
	flex.update(mut flexbox)
	
	window.add_flexbox(mut win, flexbox)

	window.run(win)
}

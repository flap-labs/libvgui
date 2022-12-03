module main

import window
import frame

fn main() {
	mut win := window.create(1280, 720, "Testing VGUI")
	
	f := frame.new(10, 10, 100, 100)
	window.add_frame(mut win, f)

	frame2 := frame.new(10, 120, 100, 100)
	window.add_frame(mut win, frame2)

	window.run()
}

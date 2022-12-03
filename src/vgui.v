module main

import window
import frame

fn main() {
	mut win := window.create(1280, 720, "Testing VGUI")
	
	frame := frame.new(10, 10, 500, 500)
	window.add_frame(mut win, frame)

	window.run()
}

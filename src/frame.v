module frame

fn C.frame_new(x int, y int, width int, height int) C.Frame
fn C.frame_draw(frame C.Frame)

// The frame widget
pub struct Frame {
	cframe C.Frame
pub:
	x int
	y int
	width int
	height int
}

// Creates a new frame widget
pub fn new(x int, y int, width int, height int) Frame {
	cframe := C.frame_new(x, y, width, height)
	frame := Frame { cframe, x, y, width, height }
	return frame
}

// Renders a frame object
pub fn render(frame Frame) {
	C.frame_draw(&frame.cframe)
}

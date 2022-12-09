module frame

fn C.frame_new(x int, y int, width int, height int) C.Frame
fn C.frame_draw(frame C.Frame)
fn C.frame_changeColor(frame C.Frame, r int, g int, b int)

// The frame widget
pub struct Frame {
mut:
	cframe C.Frame
pub mut:
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

// Changes the position of the frame
pub fn change_pos(mut frame Frame, x int, y int) {
	frame.x = x
	frame.y = y
	frame.cframe.x = voidptr(x)
	frame.cframe.y = voidptr(y)
}

// Changes the size of the frame
pub fn change_size(mut frame Frame, width int, height int) {
	frame.width = width
	frame.height = height
	frame.cframe.width = voidptr(width)
	frame.cframe.height = voidptr(height)
}

// Changes the color of the frame
pub fn change_color(mut frame Frame, r byte, g byte, b byte) {
	C.frame_changeColor(&frame.cframe, int(r), int(g), int(b))
}

// Renders a frame object
pub fn render(frame Frame) {
	C.frame_draw(&frame.cframe)
}

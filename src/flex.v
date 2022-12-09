module flex
import frame

fn C.win32_getWidth() int
fn C.win32_getHeight() int

// A flexbox component
pub struct Flexbox {
	x int
	y int
mut:
	padx int
	pady int
	frames []frame.Frame
	items int
}

// Creates a new flexbox component
pub fn new(x int, y int) Flexbox {
	flex := Flexbox { x, y, 0, 0, []frame.Frame{}, 0 }
	return flex
}

// Sets the padding of each item inside the flexbox
pub fn set_padding(mut flexbox Flexbox, padx int, pady int) {
	flexbox.padx = padx
	flexbox.pady = pady
}

// Adds a new frame component to the flexbox
pub fn add_frame(mut flexbox Flexbox, f frame.Frame) {
	flexbox.items++
	flexbox.frames.insert(flexbox.frames.len, f)
}

// Updates the flexbox
pub fn update(mut flexbox Flexbox) {
	mut component_width := C.win32_getWidth() / flexbox.items - flexbox.x - flexbox.padx + 5 * (flexbox.items - 3)

	mut current_x := flexbox.x
	mut count := 0

	for mut i in flexbox.frames {
		if count == flexbox.items - 1 {
			component_width -= ((flexbox.items - 2) * 5) * (flexbox.items - 3)
		}

		frame.change_size(mut i, component_width, C.win32_getHeight() - flexbox.y - flexbox.pady)
		frame.change_pos(mut i, current_x, flexbox.y)

		current_x += component_width + flexbox.padx
		count++
	}
}

// Renders the flexbox
pub fn render(flexbox Flexbox) {
	C.frame_render()

	for f in flexbox.frames {
		frame.render(f)
	}
}

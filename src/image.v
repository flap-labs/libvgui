module image

fn C.image_new(x int, y int, width int, height int, filepath &char) C.Image
fn C.image_draw(image C.Image)

// An image widget
pub struct Image {
	cimage C.Image
pub:
	x int
	y int
	width int
	height int
}

// Creates a new image with the specified position, size, and filepath
pub fn new(x int, y int, width int, height int, filepath string) Image {
	cimage := C.image_new(x, y, width, height, filepath.str)
	image := Image { cimage, x, y, width, height }
	return image
}

// Renders the image widget
pub fn image_render(image Image) {
	C.image_draw(&image.cimage)
}

from citrus import *

import sf2d
import sftd

sf2d.init()
sftd.init()

sf2d.set_clear_color(sf2d.rgba8(0, 0, 0, 255))
sf2d.swap_buffers()

console.init(gfx.SCREEN_BOTTOM)

font = sftd.Font('/Custard.ttf', 40, color=sf2d.rgba8(255, 255, 255, 255), line_width=100)
box = font.calc_bounding_box('Hello, world!')

print(box)

# center the text on the top screen
text_x = int((sf2d.screen['top']['width'] / 2.0) - (box[0] / 2.0))
text_y = int((sf2d.screen['top']['height'] / 2.0) - (box[1] / 2.0))

print('Text position: (%d, %d)' % (text_x, text_y))
print("""Font:
  size: %d
  color: %d
  line_width: %d""" % (font.size, font.color, font.line_width))

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_LEFT)

    font.draw_text(text_x, text_y, 'Hello, world!')

    sf2d.end_frame()

    sf2d.swap_buffers()

sftd.fini()
sf2d.fini()

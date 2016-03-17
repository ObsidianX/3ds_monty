from citrus import *

import sf2d
import math

sf2d.init()
sf2d.set_clear_color(sf2d.rgba8(0x40, 0x40, 0x40, 0xFF))

console.init(gfx.SCREEN_BOTTOM)

rads = 0.0

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_LEFT)

    sf2d.draw_fill_circle(60, 100, 35, sf2d.rgba8(0, 255, 0, 255))
    sf2d.draw_fill_circle(180, 120, 55, sf2d.rgba8(255, 255, 0, 255))

    sf2d.draw_rectangle_rotate(260, 20, 40, 40, sf2d.rgba8(255, 255, 0, 255), 2.0 * rads)
    sf2d.draw_rectangle(20, 60, 40, 40, sf2d.rgba8(255, 0, 0, 255))
    sf2d.draw_rectangle(5, 5, 30, 30, sf2d.rgba8(0, 255, 255, 255))

    sf2d.end_frame()

    rads += 0.01

    print('\x1b[1;1H%6.0f fps' % sf2d.get_fps())

    sf2d.swapbuffers()

sf2d.fini()

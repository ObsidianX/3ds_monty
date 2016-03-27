from citrus import *
import sys

gfx.init_default()
console.init(gfx.SCREEN_TOP)

print(sys.path)

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break
    elif hid.keys_down() & hid.KEY_X:
        try:
            import _img
            print(_img)
            print(_img._PngLoader)
            print(_img._JpegLoader)
        except ImportError as e:
            print(e)

    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

gfx.exit()

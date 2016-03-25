from citrus import *
import sf2d

gfx.init_default()
top = console.PrintConsole(gfx.SCREEN_TOP, window=(2, 2, 46, 26))

metrics = {
    "top_width": sf2d.screen['top']['width'],
    "top_height": sf2d.screen['top']['height'],
    "bot_width": sf2d.screen['bottom']['width'],
    "bot_height": sf2d.screen['bottom']['height']
}

print("""Screen metrics:

 Top screen:
   width = %(top_width)s
   height = %(top_height)s

 Bottom screen:
   width = %(bot_width)s
   height = %(bot_height)s""" % metrics)

top.set_position(13, 25)
print('Press Start to exit')

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

gfx.exit()
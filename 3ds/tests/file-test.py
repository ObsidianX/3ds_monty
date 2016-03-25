from citrus import *

gfx.init_default()
console.init(gfx.SCREEN_TOP)

d = open('/tmp.py', 'r').read()
print(d)

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

gfx.exit()
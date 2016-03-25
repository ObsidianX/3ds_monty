from citrus import *

gfx.init_default()
console.init(gfx.GFX_SCREEN_TOP, None)

print('\x1b[15;19HHello, world!')
print('\x1b[29;15HPress Start to exit.')

while apt.main_loop():
    hid.scan_input()

    held = hid.keys_held()
    if held & hid.KEY_START:
        break
    if held & hid.KEY_X:
        raise Exception('Hello, exception handling!')

    gfx.flush_buffers()
    gfx.swap_buffers()
    
    gsp.wait_for_vblank()

gfx.exit()

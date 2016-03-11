from citrus import *

raise Exception('Hello, Exception handling!')

gfx.init_default()

console.init(gfx.GFX_SCREEN_TOP, None)

def pos_print(msg, x, y):
    print('\x1b[%d;%dH%s' % (y, x, msg))

def move_cursor(x, y):
    print('\x1b[%d;%dH' % (y, x))

pos_print('Hello, world!', 19, 15)

move_cursor(0, 0)

print('Roses are \x1b[31mred\x1b[0m')
print('Violets are \x1b[34mblue\x1b[0m')
print('Piracy is bad')
print('While homebrews are good\n')

print('\x1b[47;30mBlack text on white background\x1b[0m')

pos_print('Press Start to exit.', 15, 29)

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

gfx.exit()



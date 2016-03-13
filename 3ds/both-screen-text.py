from citrus import *

gfx.init_default()

top_screen = console.PrintConsole(gfx.SCREEN_TOP)
bot_screen = console.PrintConsole(gfx.SCREEN_BOTTOM)

top_screen.select()
print('This is the top screen.')
print('We have a lot of space here!')
print('\nwindow width: %d\nwindow height: %d' % (top_screen.window_width, top_screen.window_height))

bot_screen.select()
print('This is the bottom screen.')
print('We don\'t have much space...')
print('Still better than DS screens :D')

top_screen.select()
print('\x1b[29;15HPress Start to exit.')

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

gfx.exit()

from citrus import *

gfx.init_default()
console.init(gfx.SCREEN_TOP)
print('init sound\n')
ndsp.init()

ndsp.set_output_mode(ndsp.OUTPUT_MONO)
ndsp.set_output_count(2)

bobomb = ndsp.Sound(open('/hello.wav', 'rb'))
goal = ndsp.Sound(open('/goal.wav', 'rb'))
channel1 = ndsp.Channel(0)
channel1.set_interpolation(ndsp.INTERP_POLYPHASE)
channel2 = ndsp.Channel(1)
channel2.set_interpolation(ndsp.INTERP_POLYPHASE)

print('X: goal')
print('Y: bobomb')
print('Start: exit')

while apt.main_loop():
    hid.scan_input()

    down = hid.keys_down()

    if down & hid.KEY_START:
        break

    if down & hid.KEY_Y:
        print('\x1b[20;1HPlay: bobomb.wav')
        channel1.play(bobomb)
    if down & hid.KEY_X:
        print('\x1b[20;1HPlay: goal.wav  ')
        channel2.play(goal)

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

ndsp.exit()
gfx.exit()
from citrus import *

gfx.init_default()
console.init(gfx.SCREEN_TOP)
print('init sound\n')
csnd.init()

wav1 = open('/coin.wave', 'rb').read()
wav2 = open('/1up.wave', 'rb').read()

flags = csnd.FLAG_ONE_SHOT | csnd.FLAG_FORMAT_16BIT
rate = 22050

coin = csnd.Sound(channel=8, flags=flags, sample_rate=rate, data=wav1)
oneup = csnd.Sound(channel=9, flags=flags, sample_rate=rate, data=wav2)

del wav1
del wav2

print('X: 1-up')
print('Y: coin')
print('Start: exit')

while apt.main_loop():
    hid.scan_input()

    down = hid.keys_down()

    if down & hid.KEY_START:
        break

    if down & hid.KEY_Y:
        print('\x1b[20;1HPlay: coin.wave')
        coin.play()
    if down & hid.KEY_X:
        print('\x1b[20;1HPlay: 1up.wave ')
        oneup.play()

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

csnd.exit()
gfx.exit()
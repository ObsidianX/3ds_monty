from citrus import *

RATE_22050 = 22050

gfx.init_default()
console.init(gfx.SCREEN_TOP)
csnd.init()

flags = csnd.FLAG_ONE_SHOT | csnd.FLAG_FORMAT_PCM16

wav = open('/coin.wave', 'rb').read()
coin = csnd.Sound(channel=0, flags=flags, sample_rate=RATE_22050, data=wav)
del wav

wav = open('/1up.wave', 'rb').read()
oneup = csnd.Sound(channel=1, flags=flags, sample_rate=RATE_22050, data=wav)
del wav

while apt.main_loop():
    hid.scan_input()

    down = hid.keys_down()

    if down & hid.KEY_START:
        break

    if down & hid.KEY_Y:
        coin.play()
    if down & hid.KEY_X:
        oneup.play()

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

csnd.exit()
gfx.exit()
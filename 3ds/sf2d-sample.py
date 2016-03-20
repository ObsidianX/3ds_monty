from citrus import *
import sf2d
import math
import img

sf2d.init()
csnd.init()
console.init(gfx.SCREEN_BOTTOM)

print('Loading tex_yay3ds.png')
png = img.load_png(open('/tex_yay3ds.png', 'rb'))
print('Success! (%dx%d)' % (png[0], png[1]))
print('\nCreating Texture (tex_3ds)')
tex_yay3ds = sf2d.Texture(png[0], png[1], data=png[3])
print('Success! %s' % tex_yay3ds)
del png

print('\nLoading tex_shit.png')
png = img.load_png(open('/tex_shit.png', 'rb'))
print('Success! (%dx%d)' % (png[0], png[1]))
print('\nCreating Texture (tex_shit)')
tex_shit = sf2d.Texture(png[0], png[1], data=png[3])
print('Success! %s' % tex_shit)
del png

wav1 = open('/coin.wave', 'rb').read()
wav2 = open('/1up.wave', 'rb').read()

flags = csnd.FLAG_ONE_SHOT | csnd.FLAG_FORMAT_16BIT
rate = 22050

coin = csnd.Sound(channel=8, flags=flags, sample_rate=rate, data=wav1)
oneup = csnd.Sound(channel=9, flags=flags, sample_rate=rate, data=wav2)

del wav1
del wav2

def render_frame():
    sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_LEFT)

    tex_shit.draw(1, 1)
    tex_shit.draw(1, 99, center=(0, tex_shit.height - 1))
    tex_shit.draw(99, 99, center=(tex_shit.width - 1, tex_shit.height - 1))
    tex_shit.draw(99, 1, center=(tex_shit.width - 1, 0))

    tex_yay3ds.draw(50, 50, center=(tex_yay3ds.width / 2, tex_yay3ds.height / 2), angle=angle)

    sf2d.end_frame()

sf2d.set_clear_color(sf2d.rgba8(200, 200, 200, 255))

angle = 0.0

while apt.main_loop():
    hid.scan_input()

    down = hid.keys_down()
    if down & hid.KEY_START:
        break
    if down & hid.KEY_Y:
        print('\x1b[27;10HPlay: coin.wave')
        coin.play()
    if down & hid.KEY_X:
        print('\x1b[27;10HPlay: 1up.wave ')
        oneup.play()

    render_frame()

    angle += 0.1

    sf2d.swap_buffers()
    print('\x1b[28;1H%6.0f fps' % sf2d.get_fps())

sf2d.fini()
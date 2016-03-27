from citrus import *
import sf2d
import img

sf2d.init()
ndsp.init()
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

ndsp.set_output_mode(ndsp.OUTPUT_MONO)
ndsp.set_output_count(2)

hello = ndsp.Sound(open('/hello.wav', 'rb'))
goal = ndsp.Sound(open('/goal.wav', 'rb'))
channel1 = ndsp.Channel(0)
channel1.set_interpolation(ndsp.INTERP_POLYPHASE)
channel2 = ndsp.Channel(1)
channel2.set_interpolation(ndsp.INTERP_POLYPHASE)

def draw_shit():
    tex_shit.draw(1, 1)
    tex_shit.draw(1, 239, center=(0, tex_shit.height - 1), angle=0)
    tex_shit.draw(399, 239, center=(tex_shit.width - 1, tex_shit.height - 1), angle=0)
    tex_shit.draw(399, 1, center=(tex_shit.width - 1, 0), angle=0)

def render_frame():
    offset_3d = os.get_3d_slider_state() * 30.0

    sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_LEFT)

    draw_shit()
    tex_yay3ds.draw(200 - int(offset_3d / 2.0), 120, center=(tex_yay3ds.width / 2, tex_yay3ds.height / 2), angle=angle)

    sf2d.end_frame()

    if offset_3d == 0:
        return

    sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_RIGHT)

    draw_shit()
    tex_yay3ds.draw(200 + int(offset_3d / 2.0), 120, center=(tex_yay3ds.width / 2, tex_yay3ds.height / 2), angle=angle)

    sf2d.end_frame()

sf2d.set_clear_color(sf2d.rgba8(200, 200, 200, 255))
sf2d.set_3d(True)

angle = 0.0

while apt.main_loop():
    hid.scan_input()

    down = hid.keys_down()
    if down & hid.KEY_START:
        break
    if down & hid.KEY_Y:
        print('\x1b[27;10HPlay: hello.wav')
        channel1.play(hello )
    if down & hid.KEY_X:
        print('\x1b[27;10HPlay: goal.wav ')
        channel2.play(goal)

    render_frame()

    angle += 0.1

    sf2d.swap_buffers()
    print('\x1b[28;1H%6.0f fps' % sf2d.get_fps())

sf2d.fini()
ndsp.exit()

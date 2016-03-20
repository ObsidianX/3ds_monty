from citrus import *
import sf2d
import img

def frame():
    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

sf2d.init()
sf2d.set_clear_color(sf2d.rgba8(255, 0, 0, 255))

console.init(gfx.SCREEN_BOTTOM)

png = img.load_png(open('/test.png', 'rb'))
tex = sf2d.Texture(png[0], png[1], sf2d.TEXFMT_RGBA8, sf2d.PLACE_RAM, png[3])

x = 0
y = 0

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    held = hid.keys_held()
    if held & hid.KEY_DPAD_RIGHT:
        x += 1
    elif held & hid.KEY_DPAD_LEFT:
        x -= 1
    if held & hid.KEY_DPAD_DOWN:
        y += 1
    elif held & hid.KEY_DPAD_UP:
        y -= 1

    sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_LEFT)

    tex.draw(x, y)

    sf2d.end_frame()

    sf2d.swap_buffers()
    print('\x1b[1;1H%6.0f fps' % sf2d.get_fps())

sf2d.fini()
from citrus import *
import sf2d
import img

TOP_HEIGHT = sf2d.screen['top']['height']
TOP_WIDTH = sf2d.screen['top']['width']

CENTER_X = int(TOP_WIDTH / 2)
CENTER_Y = int(TOP_HEIGHT / 2)


class App:
    def __init__(self):
        sf2d.init()
        console.init(gfx.SCREEN_BOTTOM)
        print('App::__init__')

        sf2d.set_clear_color(sf2d.rgba8(0xDD, 0xDD, 0xDD, 255))

        self.shit = self.load_texture('shit')
        self.yay = self.load_texture('yay3ds')

        self.yay_angle = 0

    def load_texture(self, name):
        print('App::load_texture("romfs:/%s.png")' % name)
        png = img.load_png(open('romfs:/%s.png' % name, 'rb'))
        print('Loaded: %s' % repr(png[0:3]))
        tex = sf2d.Texture(png[0], png[1], data=png[3])
        print('Texture created')
        del png
        print('png data deleted')

        return tex

    def cleanup(self):
        print('App::cleanup')
        del self.shit
        del self.yay

        sf2d.fini()

    def run(self):
        print('App::run')
        while apt.main_loop():
            hid.scan_input()

            if hid.keys_down() & hid.KEY_START:
                break

            sf2d.start_frame(gfx.SCREEN_TOP)

            self.shit.draw(10, 10)
            self.shit.draw(10, TOP_HEIGHT - 10, center=(0, 15))
            self.shit.draw(TOP_WIDTH - 10, 10, center=(15, 0))
            self.shit.draw(TOP_WIDTH - 10, TOP_HEIGHT - 10, center=(15, 15))

            self.yay.draw(CENTER_X, CENTER_Y, center=(20, 13), angle=self.yay_angle)
            self.yay_angle += 0.1

            sf2d.end_frame()

            sf2d.swap_buffers()

        self.cleanup()


app = App()
app.run()

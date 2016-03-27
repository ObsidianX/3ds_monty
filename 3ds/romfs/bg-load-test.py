from citrus import *
import img
import sf2d

ST_LOAD = 0
ST_DISP = 1
ST_END = 2


class App:
    state = ST_LOAD
    bg = None
    load_pat = ['-', '\\', '|', '/']
    load_pat_idx = 0

    def __init__(self):
        sf2d.init()

        console.init(gfx.SCREEN_BOTTOM)

        self.bg_loader = img.load_png(open('romfs:/bgtest.png', 'rb'), background=True)

    def run(self):
        while apt.main_loop():
            hid.scan_input()

            if self.state == ST_LOAD:
                self.load_state()
            elif self.state == ST_DISP:
                self.display_state()
            elif self.state == ST_END:
                sf2d.fini()
                break

    def load_state(self):
        tex = self.bg_loader.get_image()
        if tex is not None:
            del self.bg_loader

            self.bg = sf2d.Texture(tex[0], tex[1], sf2d.TEXFMT_RGBA8, sf2d.PLACE_RAM, tex[3])

            self.state = ST_DISP
            print('\x1b[1;1H' + (' ' * 50))
            return

        print('\x1b[1;1HLoading %s' % self.load_pat[self.load_pat_idx])
        self.load_pat_idx += 1
        if self.load_pat_idx >= len(self.load_pat):
            self.load_pat_idx = 0

        sf2d.swap_buffers()

    def display_state(self):
        if hid.keys_down() & hid.KEY_START:
            self.state = ST_END

        sf2d.start_frame(gfx.SCREEN_TOP, gfx.SIDE_LEFT)

        self.bg.draw(0, 0)

        sf2d.end_frame()

        sf2d.swap_buffers()


App().run()

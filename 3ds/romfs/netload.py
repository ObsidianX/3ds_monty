from citrus import *
import sf2d
import sftd
import img
import math

TOP_WIDTH = sf2d.screen['top']['width']
TOP_HEIGHT = sf2d.screen['top']['height']
BOT_WIDTH = sf2d.screen['bottom']['width']
BOT_HEIGHT = sf2d.screen['bottom']['height']


class NetLoad:
    def __init__(self):
        sf2d.init()
        sftd.init()

        sf2d.set_clear_color(sf2d.rgba8(0, 0, 0, 255))

        bg_tile = img.load_png(open('romfs:/netload_assets/bg_tile.png', 'rb'))

        self.bg_tile = sf2d.Texture(bg_tile[0], bg_tile[1], data=bg_tile[3])
        self.font = sftd.Font('romfs:/netload_assets/DroidSans-Bold.ttf', 20, color=sf2d.rgba8(0, 0, 0, 255))

        self.a = 0.0

        del bg_tile

    def _cleanup(self):
        del self.bg_tile
        del self.font

        sftd.fini()
        sf2d.fini()

    def run(self):
        while apt.main_loop():
            hid.scan_input()

            self.draw_top()
            self.draw_bottom()

            sf2d.swap_buffers()

        self._cleanup()

    def draw_top(self):
        sf2d.start_frame(gfx.SCREEN_TOP)

        params = (2 << 12) | (2 << 8)
        self.bg_tile.draw_quad_uv(0, 0, TOP_WIDTH, TOP_HEIGHT, 0, 0, TOP_WIDTH / 8, TOP_HEIGHT / 8, params)

        self.font.draw_text(10, 20, "Python NetLoader")

        sf2d.end_frame()

    def draw_bottom(self):
        sf2d.start_frame(gfx.SCREEN_BOTTOM)

        params = (2 << 12) | (2 << 8)
        self.bg_tile.draw_quad_uv(0, 0, BOT_WIDTH, BOT_HEIGHT, 0, 0, BOT_WIDTH / 8, BOT_HEIGHT / 8, params)

        sf2d.end_frame()


app = NetLoad()
app.run()

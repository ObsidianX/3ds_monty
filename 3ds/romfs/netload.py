from citrus import *
import sf2d
import sftd
import img

TOP_WIDTH = sf2d.screen['top']['width']
TOP_HEIGHT = sf2d.screen['top']['height']
BOT_WIDTH = sf2d.screen['bottom']['width']
BOT_HEIGHT = sf2d.screen['bottom']['height']

REPEAT_TEX = (2 << 12) | (2 << 8)

STATE_LOADING = 0
STATE_WAITING = 1
STATE_RECEIVING = 2
STATE_DONE = 3

CENTER_X = int(TOP_WIDTH / 2.0)
CENTER_Y = int(TOP_HEIGHT / 2.0)

SPINNER_CENTER_X = 16
SPINNER_CENTER_Y = 16


class NetLoad:
    spinner = []
    spinner_index = 0
    spinner_count = 0

    def __init__(self):
        sf2d.init()
        sftd.init()

        console.init(gfx.SCREEN_BOTTOM)

        sf2d.set_clear_color(sf2d.rgba8(0, 0, 0, 255))

        bg_tile = img.load_png(open('romfs:/netload_assets/bg_tile.png', 'rb'))
        self.bg_tile = sf2d.Texture(bg_tile[0], bg_tile[1], data=bg_tile[3])
        del bg_tile

        self.font = sftd.Font('romfs:/netload_assets/DroidSans-Bold.ttf', 20, color=sf2d.rgba8(0, 0, 0, 255))

        self.state = STATE_LOADING

        #for i in range(1, 9):
        #    png = img.load_png(open('romfs:/netload_assets/spinner%d.png' % i, 'rb'))
        #    self.spinner.append(sf2d.Texture(png[0], png[1], data=png[3]))
        #    del png

    def _cleanup(self):
        del self.bg_tile
        del self.font

        for i in self.spinner:
            del self.spinner[i]

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

        self.bg_tile.draw_quad_uv(0, 0, TOP_WIDTH, TOP_HEIGHT, 0, 0, TOP_WIDTH / 8, TOP_HEIGHT / 8, REPEAT_TEX)

        self.font.draw_text(0, 0, "Python NetLoader")

        #if self.state == STATE_LOADING:
        #    self.draw_spinner()

        sf2d.end_frame()

    def draw_bottom(self):
        sf2d.start_frame(gfx.SCREEN_BOTTOM)

        self.bg_tile.draw_quad_uv(0, 0, BOT_WIDTH, BOT_HEIGHT, 0, 0, BOT_WIDTH / 8, BOT_HEIGHT / 8, REPEAT_TEX)

        sf2d.end_frame()

    def draw_spinner(self):
        self.spinner[self.spinner_index].draw(CENTER_X, CENTER_Y, center=(SPINNER_CENTER_X, SPINNER_CENTER_Y), angle=0)
        self.spinner_count += 1
        if self.spinner_count >= 15:
            self.spinner_index += 1
            self.spinner_count = 0
            if self.spinner_index > len(self.spinner):
                self.spinner_index = 0


app = NetLoad()
app.run()

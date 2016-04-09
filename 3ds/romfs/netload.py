from citrus import *
import sf2d
import sftd
import img
import socket
import struct

TOP_WIDTH = sf2d.screen['top']['width']
TOP_HEIGHT = sf2d.screen['top']['height']
BOT_WIDTH = sf2d.screen['bottom']['width']
BOT_HEIGHT = sf2d.screen['bottom']['height']

REPEAT_TEX = (2 << 12) | (2 << 8)

STATE_WAITING = 1
STATE_RECEIVING = 2
STATE_DONE = 3

CENTER_X = int(TOP_WIDTH / 2.0)
CENTER_Y = int(TOP_HEIGHT / 2.0)

SPINNER_CENTER_X = 12
SPINNER_CENTER_Y = 12


class NetLoad:
    spinner = []
    spinner_index = 0
    spinner_count = 0
    server = None
    client = None
    quit = False
    complete = False
    downloaded = 0
    done_count = 0

    def __init__(self):
        sf2d.init()
        sftd.init()
        soc.init()

        sf2d.set_clear_color(sf2d.rgba8(0, 0, 0, 255))

        # clear screens
        sf2d.start_frame(gfx.SCREEN_TOP)
        sf2d.end_frame()
        sf2d.start_frame(gfx.SCREEN_BOTTOM)
        sf2d.end_frame()

        self.bg_tile = self.load_texture('bg_tile')
        self.pc_icon = self.load_texture('computer')
        self.arrow = self.load_texture('arrow')
        self.python = self.load_texture('script')
        self._3ds = self.load_texture('3ds')
        self.progress = self.load_texture('progress')
        self.progress_bg = self.load_texture('progress_bg')
        self.success = self.load_texture('success')

        self.font = sftd.Font('romfs:/netload_assets/DroidSans-Bold.ttf', 20, color=sf2d.rgba8(0, 0, 0, 255))

        self.state = STATE_WAITING

        for i in range(1, 9):
            self.spinner.append(self.load_texture('spinner%d' % i))

        self.setup_socket()

    def _cleanup(self):
        del self.bg_tile
        del self.pc_icon
        del self.arrow
        del self.python
        del self._3ds

        del self.font

        for i in self.spinner:
            del i

        if self.client != None:
            self.client.close()
        if self.server != None:
            self.server.close()

        soc.exit()
        sftd.fini()
        sf2d.fini()

    def load_texture(self, name):
        png = img.load_png(open('romfs:/netload_assets/%s.png' % name, 'rb'))
        tex = sf2d.Texture(png[0], png[1], data=png[3])
        del png

        return tex

    def run(self):
        while apt.main_loop():
            hid.scan_input()

            self.draw_top()
            self.draw_bottom()

            if self.quit or self.complete:
                break

            sf2d.swap_buffers()

        self._cleanup()

    def draw_top(self):
        sf2d.start_frame(gfx.SCREEN_TOP)

        self.bg_tile.draw_quad_uv(0, 0, TOP_WIDTH, TOP_HEIGHT, 0, 0, TOP_WIDTH / 8, TOP_HEIGHT / 8, REPEAT_TEX)

        self.font.draw_text(0, 0, '')
        self.font.size = 30
        self.font.draw_text(45, 10, 'Monty3DS NetLoader')
        self.font.size = 20

        {STATE_WAITING: self.state_waiting,
         STATE_RECEIVING: self.state_receiving,
         STATE_DONE: self.state_done
         }[self.state]()

        sf2d.end_frame()

    def draw_bottom(self):
        sf2d.start_frame(gfx.SCREEN_BOTTOM)

        self.bg_tile.draw_quad_uv(0, 0, BOT_WIDTH, BOT_HEIGHT, 0, 0, BOT_WIDTH / 8, BOT_HEIGHT / 8, REPEAT_TEX)

        if self.state == STATE_WAITING:
            self.font.draw_text(65, BOT_HEIGHT - 30, 'Press Start to Cancel')

        sf2d.end_frame()

    def draw_spinner(self):
        self.spinner[self.spinner_index].draw(TOP_WIDTH - 30, TOP_HEIGHT - 30,
                                              center=(SPINNER_CENTER_X, SPINNER_CENTER_Y))
        self.spinner_count += 1
        if self.spinner_count >= 3:
            self.spinner_index += 1
            self.spinner_count = 0
            if self.spinner_index >= len(self.spinner):
                self.spinner_index = 0

    def setup_socket(self):
        self.server = socket.socket()
        self.server.bind(('', 5000))
        self.server.setblocking(False)
        self.server.listen(5)

    def state_waiting(self):
        self.draw_spinner()

        self.pc_icon.draw(100, CENTER_Y - 10, center=(32, 32))
        self.arrow.draw(150, CENTER_Y - 10, center=(12, 12))
        self.python.draw(CENTER_X, CENTER_Y - 10, center=(24, 24))
        self.arrow.draw(250, CENTER_Y - 10, center=(12, 12))
        self._3ds.draw(300, CENTER_Y - 10, center=(32, 32))

        self.font.draw_text(70, TOP_HEIGHT - 60, 'Waiting for Python script...')
        self.font.size = 15
        self.font.draw_text(140, TOP_HEIGHT - 30, '%s:5000' % socket.gethostid())
        self.font.size = 20

        if hid.keys_down() & hid.KEY_START:
            self.quit = True

        try:
            self.client, self.client_addr = self.server.accept()

            self.filesize = struct.unpack('!I', self.client.recv(4))[0]
            self.output = open('/monty_netload.py', 'w')

            self.state = STATE_RECEIVING
        except BaseException:
            pass

    def state_receiving(self):
        progress = float(self.downloaded) / float(self.filesize)

        self.font.draw_text(125, TOP_HEIGHT - 60, 'Receiving...')

        self.progress_bg.draw(CENTER_X, CENTER_Y, center=(128, 16))
        self.progress.draw_quad_uv(72, 104, 72 + (256 * progress), 136, 0, 0, progress, 1, 0)

        old_color = self.font.color
        self.font.color = sf2d.rgba8(255, 255, 255, 255)
        self.font.draw_text(CENTER_X - 20, CENTER_Y - 13, str(int(progress * 100)) + '%')
        self.font.color = old_color

        try:
            chunk = self.client.recv(self.filesize)
            self.output.write(chunk)
            self.downloaded += len(chunk)
            if self.downloaded == self.filesize:
                self.output.close()
                self.client.close()
                self.client = None
                self.state = STATE_DONE
        except BaseException:
            pass

    def state_done(self):
        self.success.draw(CENTER_X, CENTER_Y, center=(32, 32))

        if self.done_count >= 30:
            self.complete = True
        else:
            self.done_count += 1


app = NetLoad()
app.run()

if app.quit:
    raise Exception(0xDEAD0000)

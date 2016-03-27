from citrus import *
from uthread import *

gfx.init_default()
console.init(gfx.SCREEN_TOP)

running = True
print('Creating threads')


class HidThread(MicroThread):
    def step(self):
        while True:
            if hid.keys_down() & hid.KEY_START:
                running = False
            yield None


class SpinnerThread(MicroThread):
    pattern = ['|', '/', '-', '\\']

    def create(self):
        self.index = 0

    def step(self):
        while True:
            print('\x1b[28;3H%s' % self.pattern[self.index])
            self.index += 1
            yield None


print('Creating scheduler')
sched = Scheduler()
sched.add_thread(HidThread())
sched.add_thread(SpinnerThread())
sched_g = sched.main()

while apt.main_loop():
    hid.scan_input()

    if running:
        next(sched_g)

    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

gfx.exit()

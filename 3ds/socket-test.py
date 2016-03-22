from citrus import *
import socket


gfx.init_default()
top = console.PrintConsole(gfx.SCREEN_TOP)
bottom = console.PrintConsole(gfx.SCREEN_BOTTOM)
print('')

soc.init()
s = socket.socket()

s.setblocking(False)
ai = socket.getaddrinfo('', 4000)
print(ai)
s.bind(ai[0][4])
s.listen(1)

bottom.select()

print('Socket created')
print('Listening on port 4000')
print('\x1b[29;10HPress Start to exit')

client = None

top.select()
while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    if client is None:
        try:
            for i in range(10):
                client, addr = s.accept()
            client.settimeout(0.001)
            client.setblocking(False)
        except OSError as e:
            print(e)

    else:
        try:
            for i in range(10):
                d = client.recv(1024)
            print('IN: %s' % d)
        except OSError as e:
            print(e)

    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

soc.exit()
gfx.exit()
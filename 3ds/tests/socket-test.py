from citrus import *
import socket


gfx.init_default()
top = console.PrintConsole(gfx.SCREEN_TOP)
bottom = console.PrintConsole(gfx.SCREEN_BOTTOM)
print('')

soc.init()
s = socket.socket()
s.setblocking(False)
s.bind(('', 4000))
s.listen(5)

bottom.select()

print('Socket created')
print('Listening on %s:4000' % socket.gethostid())
print('\x1b[29;10HPress Start to exit')

client = None
last_error = None

top.select()
while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    if client is None:
        try:
            client, addr = s.accept()
            client.setblocking(False)

            print('\x1b[2;2H                          ')
            print('\x1b[4;2Hnew connection:', addr)
        except socket.error as e:
            if e.args != last_error:
                last_error = e.args
                print(e.args)
    else:
        try:
            data = client.recv(1024)
            client.send('pong')
            print('\x1b[6;2HIN: %s' % data)
        except socket.error as e:
            if e.args[0] == 104:
                print('\x1b[10;2HClient disconnected')
                client = None
            else:
                print('\x1b[2;3H%s' % e)

    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

soc.exit()
gfx.exit()
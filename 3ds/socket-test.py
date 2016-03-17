from citrus import *

from socket import socket

gfx.init_default()
console.init(gfx.SCREEN_TOP)

soc.init()
s = socket()

print('Socket created')
print('Pres Start to exit')

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    gsp.wait_for_vblank()

soc.exit()

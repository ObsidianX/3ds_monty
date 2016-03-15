from citrus import *

def prnt(msg):
    print(msg)
    gfx.flush_buffers()

def do_download():
    top = console.PrintConsole(gfx.SCREEN_TOP)
    bottom = console.PrintConsole(gfx.SCREEN_BOTTOM)

    console.clear()
    bottom.select()
    prnt('creating request')
    req = httpc.Request('http://scooterlabs.com/echo')

    prnt('beginning request')
    req.begin_request()

    status = req.response_status_code
    prnt('status code: %d' % status)
    if status != 200:
        prnt('! result not 200. aborting.')
        del req
        return
    
    size = req.download_size
    prnt('content size: %d' % size)

    content = req.download_data(size)
    if content is None:
        print('! download error: %d' % req.last_result)
        del req
        return
    
    top.select()
    prnt(content)
    del req


gfx.init_default()

console.init(gfx.SCREEN_TOP)

http_init = False
print('Press Y to initialize HTTPC')

while apt.main_loop():
    hid.scan_input()

    down = hid.keys_down()
    if down & hid.KEY_Y and not http_init:
        http_init = True
        httpc.init()
        console.clear()
        print('httpc init!')
        print('Press A to make a GET request')
    elif down & hid.KEY_A and http_init:
        do_download()
    elif down & hid.KEY_START:
        break

    gfx.flush_buffers()
    gfx.swap_buffers()

    gsp.wait_for_vblank()

if http_init:
    httpc.exit()
gfx.exit()

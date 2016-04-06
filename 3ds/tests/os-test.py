from citrus import *

gfx.init_default()
console.init(gfx.SCREEN_TOP)

format = """\x1b[1;1H
    Firm version: %d
    Kernel version: %d
    Time: %d
    Wifi Strength: %d
    3D Slider: %1.2f
    System version tuple: %s
    System version string: %s
"""

TIME_POS = '4;10'
WIFI_POS = '5;19'
_3D_POS = '6;15'


def gen_info():
    firm = os.get_firm_version()
    kern = os.get_kernel_version()
    time = os.get_time()
    wifi = os.get_wifi_strength()
    slider = os.get_3d_slider_state()
    sys_tup = os.get_system_version_data()
    sys_str = os.get_system_version_data_string()

    return (firm, kern, time, wifi, slider, sys_tup, sys_str)


print(format % gen_info())

while apt.main_loop():
    hid.scan_input()

    if hid.keys_down() & hid.KEY_START:
        break

    print('\x1b[%sH%d' % (TIME_POS, os.get_time()))
    print('\x1b[%sH%d' % (WIFI_POS, os.get_wifi_strength()))
    print('\x1b[%sH%1.2f' % (_3D_POS, os.get_3d_slider_state()))

    gfx.flush_buffers()
    gfx.swap_buffers()
    gsp.wait_for_vblank()

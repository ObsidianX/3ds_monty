#!/usr/bin/python

import os
import socket
import sys
import struct

MONTY_PORT = 5000
READ_SIZE = 16 * 1024

TYPE_PY  = 1
TYPE_ZIP = 2

ip = sys.argv[1]
filename = sys.argv[2]

base_filename = os.path.basename(filename)

stat = os.stat(filename)
filetype = 0
if filename.lower().endswith('zip'):
    filetype = TYPE_ZIP
elif filename.lower().endswith('py'):
    filetype = TYPE_PY
else:
    print('Must upload a ZIP or PY file')
    sys.exit(1)

info_packet = struct.pack('!IBH', stat.st_size, filetype, len(base_filename))

f = open(filename, 'rb')

s = socket.socket()
s.connect((ip, MONTY_PORT))

s.send(info_packet)
s.send(base_filename.encode())

server_filesize = struct.unpack('!I', s.recv(4))[0]
if server_filesize != stat.st_size:
    print('Server sent back the wrong size response: %d (expected %d)' % (server_filesize, stat.st_size))
    sys.exit(1)
print('Sending "%s" (%d bytes)...' % (base_filename, stat.st_size))

while True:
    chunk = f.read(READ_SIZE)
    if not chunk:
        break
    s.sendall(chunk)

s.close()



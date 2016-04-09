#!/usr/bin/python
import os, socket, sys, struct

statinfo = os.stat(sys.argv[2])
fbiinfo = struct.pack('!I', statinfo.st_size)
p = sys.argv[2]
dsip = sys.argv[1]

file = open(p, "rb")
sock = socket.socket()
sock.connect((dsip, 5000))

sock.send(fbiinfo)
print('File size: %d (%s)' % (statinfo.st_size, fbiinfo.encode('hex')))

while True:
    chunk = file.read(16384)
    if not chunk:
        break  # EOF
    sock.sendall(chunk)

sock.close()
sys.exit()

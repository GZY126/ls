#!/usr/bin/env  python2
# -*- coding: utf-8 -*-  


from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

# 0F BD D7 8E 0C 01 02 05 06 09 09 06 05 04 08 05 02 0F 38 D4 21 0D 25 15 01 36 54 20 B2 5F 15 16 D4
# 33 15 06 08 01  02 41 12 13 DE 0B 6F 21 4D 37 00 05 0B 6F 21 
# 36 49 EB 20 AB C6 6A A8 62 3D D9 B3 51 7F 5C 11 50 12 0D 5F 71 35 5F 03 A6 7E D5 34 02 FF AD FC C3

backend = default_backend()
key = '\x0F\xBD\xD7\x8E\x0C\x01\x02\x05\x06\x09\x09\x06\x05\x04\x08\x05\x02\x0F\x38\xD4\x21\x0D\x25\x15\x01\x36\x54\x20\xB2\x5F\x15\x16'
iv = '\x33\x15\x06\x08\x01\x02\x41\x12\x13\xDE\x0B\x6F\x21\x4D\x37\x00\x05\x0B\x6F\x21\x4D\x37\x00\x05\x00'
cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend)
decryptor = cipher.decryptor()
ct = decryptor.update(b"\x36\x49\xEB\x20\xAB\xC6\x6A\xA8\x62\x3D\xD9\xB3\x51\x7F\x5C\x11\x50\x12\x0D\x5F\x71\x35\x5F\x03\xA6\x7E\xD5\x34\x02\xFF\xAD\xFC") + decryptor.finalize()
print ct



cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend)
encryptor = cipher.encryptor()
ct = encryptor.update(b"gotodomains.xyz" + "\x00" * (32-15)) + encryptor.finalize()
EncData = ''
for i in ct:
    EncData += '%02X ' % ord(i)
print EncData

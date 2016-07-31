# Python bytecode 2.7 (disassembled from Python 2.7)

# Embedded file name: 002.py
# Compiled at: 2016-07-31 09:08:13
import base64
import os
import random
from Crypto import Random
from Crypto.Cipher import AES
from datetime import datetime

class Ransom(object):
    def __init__(self, key, encFile):
        self.data = encFile
        if len(key) >= len(str(encFile)):
            self.key = key[:encFile]
        else:
            self.key = self.padding(key)

    def foo(self, data):
        data = self.padding(data)
        iv = Random.new().read(16)
        return base64.b64encode(iv + AES.new(self.key, AES.MODE_CBC, iv).encrypt(data))

    def padding(self, key):
        return key + (self.data - len(key) % self.data) * chr(self.data - len(key) % self.data)


def keyExpansion(k):
    orange = range(256)
    i = 0
    for j in xrange(256):
        i = (i + orange[j] + ord(k[j % len(k)])) % 256
        orange[j], orange[i] = orange[i], orange[j]
    return orange


def rc4(state):
    i, j = (0, 0)
    while True:
        i = (i + 1) % 256
        j = (j + state[i]) % 256
        state[i], state[j] = state[j], state[i]
        stream = state[(state[i] + state[j]) % 256]
        yield stream

def rc4_enc(input, k):
    keyStream = rc4( keyExpansion(k) )
    input = bytearray(input)
    output = bytearray((i ^ il for i, il in zip(input, keyStream)))
    return str(output)


inputData = open('.\\file', 'rb').read()
encFile = open('.\\file.enc', 'wb')

logan = 'TrEndMicRo'
k = ''
for i in range(5):
    k = k + random.choice(logan)
k = '!' + k + k + k + k + k + k + '!'

encryptor = Ransom(k, 32)

peach = ''
watermelon = datetime.now()
peach = peach + watermelon.strftime('%Y')
peach = peach + watermelon.strftime('%d')
peach = peach + watermelon.strftime('%m')

for i in xrange(10):
    grape = encryptor.foo(inputData)
    plum = rc4_enc(grape, peach)
    inputData = plum

encFile.write(plum)
encFile.close()

import hashlib

w = hashlib.md5()
w.update(open("flag.txt").readline())
open("key.txt", "wb").write( w.digest() )
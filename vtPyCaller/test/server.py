import sys
import socket

def main():
    server = server_create('localhost', 9090)
    while 1:
        #accept connections from outside
        (client, address) = server.accept()

        msg = message_get(client)
        if (msg):
            print "recieved(%s)" % (msg)
            message_send(client, sys.argv[1])
            print "sent(%s)" % (sys.argv[1])
        else:
            break

def server_create(host, port):
    #create an INET, STREAMing socket
    server = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)
    #bind the socket to a public host, 
    # and a well-known port
    server.bind((host, port))
    #become a server socket
    server.listen(5)

    return server

### Common ####
MSGLEN = 1024

def message_get(sock):
    msg = ''
    while len(msg) < MSGLEN:
        chunk = sock.recv(MSGLEN-len(msg))
        if chunk == '':
            raise RuntimeError, "socket connection broken"
        msg = msg + chunk
        return msg
    return 0

def message_send(sock, message):
    msgfmt = "%-" + "%ds" % (MSGLEN)
    msg = msgfmt % (message)

    totalsent = 0
    while totalsent < MSGLEN:
        sent = sock.send(msg[totalsent:])
        if sent == 0:
            raise RuntimeError, "socket connection broken"
        totalsent = totalsent + sent
    return 1

main()
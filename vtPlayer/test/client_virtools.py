import vt
import sys
import socket

### Common ####
MSGLEN = 10

def vt_client():
    client = server_connect('localhost', 9090)
    vt.ActivateOut(bid,0,1)
    if vt.IsInputActive(bid,0):
	my_message = vt.GetInVal(bid,3)
	vt.SetOutVal(bid,0,my_message)
	if message_send(client, my_message):
	   vt.ActivateOut(bid,2,1)
	   msg = message_get(client)
	   if(msg):
		vt.ActivateOut(bid,3,1)
		vt.SetOutVal(bid,1,len(msg))
		vt.SetOutVal(bid,2, msg)


def server_connect(host, port):
    #create an INET, STREAMing socket
    client = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)
    #now connect to the web server on port 80 
    # - the normal http port
    client.connect((host, port))

    return client

def message_get(sock):
    msg = ''
    chunk = ''

    while len(msg) < MSGLEN:
	chunk = sock.recv(MSGLEN-len(msg))
        if chunk == '':
	     vt.ActivateOut(bid,1,1)
	     vt.SetOutVal(bid,3, "socket connection broken")
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
	    vt.ActivateOut(bid,1,1)
            vt.SetOutVal(bid,3, "socket connection broken")
        totalsent = totalsent + sent
    return 1

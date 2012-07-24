import socket

HOST = ''
PORT = 3002          # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((HOST, PORT))
#s.listen(1)
#conn, addr = s.accept()
#print "Connected to IRC5\n"
while 1:
	data = s.recv(1024)
	if not data:
		break
	else:
		s.sendto(" ",('192.168.0.150',PORT))
		print data+"\n"
conn.close()

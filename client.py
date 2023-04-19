# Import socket module
import socket			

# Create a socket object
s = socket.socket()		

# Define the port on which you want to connect
port = 12345			

# connect to the server on local computer

while True:
    # receive data from the server and decoding to get the string.
    x=input()
    s.connect(('127.0.0.1', port))
    print (s.recv(1024).decode())

    # close the connection
s.close()	
	

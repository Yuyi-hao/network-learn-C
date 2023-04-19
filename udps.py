import socket

host='127.0.0.1'

port=62000
buffer=1024

s = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)		
print ("Socket successfully created by aditya")

s.bind((host,port))		
print ("socket binded to with host :",host," port :",port)

while True:

    # Establish connection with client.
    c, addr= s.recvfrom(buffer)	
    print('connected with client address : ', addr)
    print("message: ",c)

    # send a thank you message to the client. encoding to send byte type.
    x=str(input("enter messege : "))
    s.sendto(bytes(x,'utf-8'),addr)

    if c.decode()=='END':
        print("server is going to close")
        break
    # Close the connection with the client
    # c.close()

    # Breaking once connection closed
    # break


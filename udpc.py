import socket

host='127.0.0.1'

port=62000
buffer=1024

s = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)		
print ("Socket successfully created by aditya")

s.connect((host,port))		
# print ("socket binded to with host :",host," port :",port)

while True:

    # Establish connection with client.
    x=str(input("enter messege : "))

    s.send(bytes(x,'utf-8'))

    t=s.recv(buffer).decode()
    print(t)

    if x=='END':
        break
    # Close the connection with the client
    # c.close()

    # Breaking once connection closed
    # break


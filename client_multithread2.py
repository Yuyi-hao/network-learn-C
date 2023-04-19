import socket 

client_socket=socket.socket(family=socket.AF_INET,type=socket.SOCK_STREAM)
host='127.0.0.1'
port=8000

# create socket object 
client_socket.connect((host,port))

data=client_socket.recv(1024)
print(data.decode())
while True:
    # accept a client connection 
    message=input("enter message : ")
    client_socket.send(message.encode())
    data=client_socket.recv(1024)
    print("received from server : ",data.decode())
    if data.decode()=="END":
        break

client_socket.close()


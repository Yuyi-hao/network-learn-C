import socket 
import threading

# function handling client request
def handle_client(conn,addr):
    print("connect to :",addr)

    # send welcome message to client
    conn.send(b"welocome to server!")

    while True:
        # receive data frm client 
        data=conn.recv(1024)

        # check if client has disconnected 

        if not data:
            print("disconnected from : ",addr)
            break
        # data sent by client 
        print("client sent data : ",data)

        # send data back to client 
        conn.send(data)

    conn.close()

def main():
    host="127.0.0.1"
    port=8000

    # create socket object 
    server_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)

    server_socket.bind((host,port))

    server_socket.listen()

    print('server listening on ',host," ",port)

    while True:
        # accept a client connection 

        conn,addr=server_socket.accept()

        # start a new thread to handle client 
        client_thread=threading.Thread(target=handle_client,args=(conn,addr))
        client_thread.start()

if __name__=='__main__':
    main()

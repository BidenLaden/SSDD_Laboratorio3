import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_address = ('localhost', 4000)
sock.bind(server_address)

sock.listen(5)

while True:
    print('waiting for connection ')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)
        message = ''
        while True:
            data = connection.recv(1)
            if (data == b'\0'):
                break;
            message += data.decode()
        

        print('Recibido = ' + message)
        message = message + "\0"
        connection.sendall(message.encode())

    finally:
        connection.close()
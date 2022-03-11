import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = (sys.argv[1], int(sys.argv[2]))
print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

try:
    print('>', end="")
    message = input()
    message = message + "\0"
    sock.sendall(message.encode())

    #while True:
    while True:
        msg = sock.recv(1)
        if(msg == b'\0'):
            break;
        
        
    message += msg.decode()
    
    
    
    print('>>> Recibido ' + message)
finally:
    print('Closing socket ')
    sock.close()
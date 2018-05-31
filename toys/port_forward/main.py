#! /usr/bin/python3

import socket
import threading
import sys


def main():
    listen_addr = (sys.argv[1], int(sys.argv[2]))
    to_addr = (sys.argv[3], int(sys.argv[4]))

    print("监听地址：" + str(listen_addr))
    print("转发到：" + str(to_addr))

    try:
        forwader = Forwarder(listen_addr, to_addr)
        forwader.bind()
    except KeyboardInterrupt:
        pass


class Forwarder:
    def __init__(self, listen_addr, to_addr):
        self.listen_addr = listen_addr
        self.to_addr = to_addr
        self.timeout = 30

    def bind(self):
        server = None

        try:
            server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server.settimeout(self.timeout)
            server.bind(self.listen_addr)
            server.listen(5)

            while True:
                sock, addr = server.accept()
                t = threading.Thread(target=self._forward_wrapper,
                                     args=(server, sock, addr))
                t.start()
        finally:
            if server:
                server.close()

    def _forward_wrapper(self, server, sock, addr):
        try:
            self._forward(sock, addr)
        except Exception as e:
            raise e
            if server:
                server.close()

    def _forward(self, sock, addr):
        def forward_io(from_socket, to_socket):
            try:
                while True:
                    to_socket.send(from_socket.recv(1024))
            except Exception as e:
                print(type(e))
                print(e)
                from_socket.close()
                to_socket.close()

        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.settimeout(self.timeout)

        try:
            client.connect(self.to_addr)
            t = threading.Thread(target=forward_io, args=(sock, client))
            t.start()
            forward_io(client, sock)
            t.join()
        except ConnectionRefusedError:
            print("连接sock失败!")
            sock.close()


if __name__ == '__main__':
    main()

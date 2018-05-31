#! /usr/bin/env python3

import os
import sys
import socket
import multiprocessing
import signal
"""
关于socket编程的更好的例子，见socket库的文档，里面列出了一个小巧简单的例子：
https://docs.python.org/3/library/socket.html

很好的中文API：
http://www.runoob.com/python3/python3-socket.html

socket的一个小知识：
https://segmentfault.com/q/1010000003101541
"""


def main():
    ip = "127.0.0.1"
    port = 1234

    server_status_code = multiprocessing.Value('i', 0)
    server_process = multiprocessing.Process(
        target=lambda: Server().run(ip, port, server_status_code))

    print("马上启动服务器进程")
    server_process.start()

    while server_status_code.value == 0:
        pass

    client(ip, port)

    while True:
        if input("是否关闭服务器？") == "y":
            ctrl_c_process(server_process)
            break


def client(ip, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as tcp:
        try:
            tcp.connect((ip, port))
            data = input("请输入英文单词，客户端将其提交给服务器进行大小写转换：")

            print("客户端马上发送数据")
            tcp.send(data.encode())

            buffer = b''
            while True:
                s = tcp.recv(1024)
                if s:
                    buffer += s
                else:
                    break

            print("客户端已接收数据，为：")
            print(buffer)

        except socket.error as e:
            print("连接失败！失败原因：")
            print(e)


class Server:
    def __init__(self):
        pass

    def run(self, ip, port, server_status_code):
        print("服务器进程启动，本服务器为您温馨提供大小写转换功能，好用又快速! pid：%d" % os.getpid())
        print("服务器监听端口: %s:%d" % (ip, port))

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as tcp:
            try:
                tcp.bind((ip, port))  # 绑定ip:port
                tcp.listen(1)  # 开始监听
                server_status_code.value = 1
                self._loop(tcp)  # 循环接收连接并处理
            except OSError as e:
                server_status_code.value = 2
                print("绑定失败，失败原因：")
                print(e)
            except KeyboardInterrupt as e:
                print("进程被ctrl-c终止")

    def _loop(self, tcp_socket):
        while True:
            conn, addr = tcp_socket.accept()
            with conn:
                data = conn.recv(1024).decode()  # 读取客户段发送过来的数据
                data = "".join([(c.upper() if c.islower() else c.lower())
                                for c in data.split()])
                conn.send(data.encode())  # 将处理好的数据发送回去


def ctrl_c_process(process=None):
    if not process:
        pid = os.getpid()
    else:
        pid = process.pid

    if sys.platform == 'win32':
        os.kill(pid, signal.CTRL_C_EVENT)
    else:
        os.kill(pid, signal.SIGINT)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

def countdown(n):
    while n > 0:
        yield n
        n -= 1

def test():
    while True:
        print((yield))


模板 = '''\
你好，我的名字叫{名字}
'''
学霸溥 = 666
溥神 = 666

print(模板.format(名字 = 学霸溥))


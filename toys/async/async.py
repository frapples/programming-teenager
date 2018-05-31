#!/usr/bin/env python3

import asyncio

# http://blog.theerrorlog.com/yield-from-in-python-3.html
# http://www.infoq.com/cn/news/2015/05/python-async-await


def main1():
    # asyncio库的异步调用
    @asyncio.coroutine
    def hello():
        print("hello world")
        # 这里发生的事情就好像线程阻塞了一样，实际上并没有
        r = yield from asyncio.sleep(2)
        print(r)
        print("hello again")

    loop = asyncio.get_event_loop()
    tasks = [hello(), hello()]
    loop.run_until_complete(asyncio.wait(tasks))
    loop.close()


def main2():
    # 简单的协程调度
    def process1():
        for i in range(0, 100):
            print(i)
            yield

    def process2():
        for i in range(100, 200):
            print(i)
            yield

    tasks = [process1(), process2()]

    print("主循环开始")
    while len(tasks) > 0:
        for task in tasks:
            try:
                task.__next__()
            except StopIteration:
                tasks.remove(task)


def main3():
    # 在2的基础上，加了yield from。能够在协程内部调用协程
    def process1():
            yield from process2()

    def process2():
        for i in range(10, 20):
            print(i)
            yield

    # 运行的时候，就好像这两个协程并行执行一样
    tasks = [process1(), process1()]

    print("主循环开始")
    while len(tasks) > 0:
        for task in tasks:
            try:
                task.__next__()
            except StopIteration:
                tasks.remove(task)


def main4():
    # 实现一个的异步sleep的demo

    import time

    def process():
        for i in range(0, 5):
            print(i)
            real_delay = yield from my_sleep(1000)
            print("已sleep %g毫秒" % real_delay)

    def my_sleep(delay):
        start_time = time.time() * 1000
        while True:
            diff = time.time() * 1000 - start_time
            if diff >= delay:
                return diff
            else:
                yield

    tasks = [process(), process(), process(), process()]

    print("主循环开始")
    while len(tasks) > 0:
        for task in tasks:
            try:
                task.__next__()
            except StopIteration:
                tasks.remove(task)


def main5():
    main_event_loop = []
    # callback 风格

    import time

    def process():
        # callback的写法，这代码是给人看的？
        nums = list(range(0, 5))

        i = 0
        print(nums[i])

        def lambda_(real_delay):
            print("已sleep %g毫秒" % real_delay)

            nonlocal i
            i += 1

            if i < len(nums):
                print(nums[i])
                my_sleep(1000, lambda_)

        my_sleep(1000, lambda_)

    def my_sleep(delay, callback):
        # my_sleep实际上会创建一个协程放入事件循环中
        # 假设我们的python不支持yield这些花里胡梢的东西

        class Xxx:
            def __init__(self):
                self.start_time = time.time() * 1000

            def __next__(self):
                diff = time.time() * 1000 - self.start_time
                if diff >= delay:
                    callback(diff)
                    raise StopIteration()

        main_event_loop.append(Xxx())

    process()
    process()

    while len(main_event_loop) > 0:
        for task in main_event_loop:
            try:
                task.__next__()
            except StopIteration:
                main_event_loop.remove(task)


if __name__ == '__main__':
    n = input("示例1-5: ")
    globals()["main" + n]()

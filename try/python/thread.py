import threading
import time

# http://python.jobbole.com/82105/?utm_source=blog.jobbole.com&utm_medium=relatedPosts


class Money:
    def __init__(self, card1, card2):
        self.card1 = card1
        self.card2 = card2

    def change(self, n):
        self.card1 += n
        time.sleep(0.1)
        self.card2 -= n

    def __str__(self):
        return str(self.card1) + " " + str(self.card2)


def start(target1, target2=None):
    if not target2:
        target2 = target1
    th1 = threading.Thread(target=target1)
    th2 = threading.Thread(target=target2)
    th1.start()
    th2.start()
    th1.join()
    th2.join()


def demo1():
    money = Money(10, 10)

    def thread():
        for i in range(5):
            money.change(1)
            print(money)

    start(thread)


def demo2():
    # 加锁保证原子性，实际上应该放入change内部加锁更好
    money = Money(10, 10)
    lock = threading.Lock()

    def thread_safe():
        for i in range(5):
            with lock:
                money.change(1)
                print(money)

    # with lock相当于
    # lock.acquire()
    # try:
    #     pass
    # finally:
    #     lock.release()

    start(thread_safe)


def demo3():
    # 消费者-生产者：缓冲区大小为100
    # 当缓冲区空时，消费者阻塞
    # 当缓冲区满时，生产者阻塞
    q = list()
    lock = threading.Lock()
    cond = threading.Condition(lock)

    def producer():
        for i in range(200):
            with lock:
                if len(q) >= 100:
                    cond.wait()

                q.append(i)
                cond.notify()

    def consumer():
        while True:
            with lock:
                if len(q) == 0:
                    cond.wait()

                data = q.pop(0)
                print(data, end=" ")

                # 注：这里有个问题，如果消费者线程不停止的话python是不输出数据的，会误以为产生了死锁
                if data == 99:
                    return
                cond.notify()

    start(producer, consumer)


def main():
    demo1()
    print()
    demo2()
    print()
    demo3()


if __name__ == '__main__':
    main()

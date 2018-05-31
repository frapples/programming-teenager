#! /usr/bin/env python3
"""
在透明终端中，使用start_color会使背景变成了黑色，不透明。解决方法：
http://stackoverflow.com/questions/3201106/ncurses-and-white-on-black

支持256色终端，以及设定背景色时如何指定默认。（设成-1即可）
http://stackoverflow.com/questions/18551558/how-to-use-terminal-color-palette-with-curses

介绍了C语言的curses用法，由此可以了解curses的一些用法
http://read.pudn.com/downloads169/sourcecode/unix_linux/778380/curses/%E7%AC%AC%E4%BA%8C%E7%AB%A0%EF%BC%9Acurses%E5%BA%93IO%E5%A4%84%E7%90%86.pdf

另外，docs.python.org的HOWTO及关于该库的文档也提供了一些信息。
https://docs.python.org/3.4/library/curses.html

256色表
http://www.calmar.ws/vim/256-xterm-24bit-rgb-color-chart.html
"""

# 注：文件名不要命名成curses，否则下面这句话会把本文件当成curses包导入
import curses

try:
    stdscr = curses.initscr()
    curses.start_color()
    curses.noecho() # 关闭回显
    curses.curs_set(True) # 是否隐藏光标
    curses.use_default_colors()

    height = 20; width = 50
    win = curses.newwin(height, width, 0, curses.COLS // 2 - width // 2)

    win.box()
    win.addstr(1, 1, "颜色种类：" + str(curses.COLORS))
    curses.init_pair(1, curses.COLOR_RED, -1)
    win.addstr(2, 1, "test1", curses.A_UNDERLINE)
    win.addstr(3, 1, "test2", curses.color_pair(1))
    win.refresh() # 在endwin()退出之后，可以用这句话刷新再进入

    while win.getch() != ord("q"):
        pass
    curses.endwin()

except BaseException as e:
    curses.endwin()
    raise e


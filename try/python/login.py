#! /usr/bin/env python3

import os.path as op
_DIR_ = op.dirname(op.realpath(__file__))

import sys
sys.path.append(op.join(_DIR_, "..", "mylib"))

import urlrequest
import urllib.parse
import re


def parse_js_location_url(content):
    m = re.match(r".*href='(.*?)'.*", content)
    if m:
        return m.group(1)
    else:
        return ''


class Main:
    __slots__ = ("_login_url", "_browser")

    def __init__(self):
        self._login_url = "http://172.18.6.30"
        self._browser = urlrequest.Browser()

    def main(self):
        username = "2014217252"
        password = "535807"
        try:
            cmd = sys.argv[1]
        except IndexError:
            cmd = ""

        is_login = self.is_login()
        print("您现在的状态: %s" % ("登录" if is_login else "离线"))

        if cmd == "login" and not is_login:
            ok, useflux, balance = self.login(username, password)
            if ok:
                print("登录成功")
                print("剩余流量: %s" % useflux)
                print("剩余余额: %s" % balance)
            else:
                print("登录失败")
        elif cmd == "logoff" and is_login:
            ok = self.logoff()
            print("注销成功" if ok else "注销失败")

    def is_login(self):
        return self._browser.open(self._login_url).decode().find("下线操作页面") >= 0

    def login(self, username, password):
        result = self._browser.open(self._login_url)
        # 跟随js跳转到的网址是真正填表单的网址，需要的是后面带的一大挂参数
        location_url = parse_js_location_url(result.decode('utf8'))
        _, query = urllib.parse.splitquery(location_url)
        url = self._login_url + "/eportal/userV2.do?" + query

        login_get_data = {
            "method": "login",
            "param": 'true',
            'username': username,
            'pwd': password
        }

        login_post_data = {
            'usernameHidden': username,
            'username': username,
            'authorMode': '',
            'pwd': password
        }

        html = self._browser.open(url,
                                  get_data=login_get_data,
                                  post_data=login_post_data).decode()

        useflux, balance = self._parse_useflux_balance(html)
        ok = html.find("登录成功窗") >= 0
        return ok, useflux, balance

    def logoff(self):
        result = self._browser.open(
            "http://172.18.6.30/eportal/userV2.do?method=offline")
        return result.decode().find("注销成功") >= 0

    def _parse_useflux_balance(self, html):
        useflux = re.findall(r"可用流量.+?([0-9.]+MB)", html)[0]
        balance = re.findall(r"本账号余额.+?([0-9.]+元)", html)[0]
        return useflux, balance


Main().main()

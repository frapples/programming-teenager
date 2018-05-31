#! /bin/env python3

"""
此demo通过代码及其注释讲解了python3脚本是如何与网站交互的。
此代码参考了资料： cuiqingcai.com/968.html。该资料虽然是针对python2的，但是却解决了笔者遇到的一些困难。该资料还提供了将cookie保存到外部文件和从外部文件加载的相关信息。
注：python内置的文档说明也提供了大量资料。用help函数。
另，该代码未进行测试。
"""
import urllib.request, urllib.parse, http.cookiejar


def demo_1:
    url = "http://www.google.com"

    # postData可以是dict可以是拥有两个元素的list。储存的是POST数据。
    postData = {
        'data1': 'jfkdkdl',
        'data2': 'jfdjcd',
        'data3':'djfkdl'
        }

    # 创建请求对象req，请求的URL是url
    req = urllib.request.Request(url)
    # 请求对象的add_header方法设置请求头，下面设置了UA，语言等。
    req.add_header("User-Agent", "Windows / IE 9: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)")
    req.add_header("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3")
    req.add_header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8")

    # urlopen从请求头，编码后的post数据创建回应对象，这是个类文件对象，
    with urllib.request.urlopen(req, data=urllib.parse.urlencode(postData).encode('utf8')) as f:
        print('status:', f.status, f.reason)
        for k, v in f.getheaders():
            print('%s: %s' % (k, v))
        print('Data:', f.read().decode('gbk'))

def demo_2:
    ### 注：测试时偶然发现php内置的server(php -S参数开启)的$_COOKIES数组无法得到cookies，已确保请求头包含cookie信息，无论是脚本还是浏览器测试都有此问题。

    # 该对象可以不设置，若设置了那么会把cookie信息保存到这里
    cookies = http.cookiejar.CookieJar()
    # cookies处理器，参数可以省略
    cookieHandler = urllib.request.HTTPCookieProcessor(cookies)
    # 从cookies处理器创建打开器对象，之后通过此打开器与网站交互，cookies会被自动处理。
    opener = urllib.request.build_opener(cookieHandler)

    url = "http://www.google.com"
    req = urllib.request.Request(url)
    req.add_header("User-Agent", "Windows / IE 9: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)")
    with opener.open(req, timeout=500) as f:
        print('status:', f.status, f.reason)
        for k, v in f.getheaders():
            print('%s: %s' % (k, v))
        print('Data:', f.read().decode('utf8'))

def demo_3:
    # 注意，此处的代理参数是指，若请求的是http协议则使用http键指定的代理，若请求的是https协议则使用https键指定的代理。
    # 如果你在参数中写的是sock5代理，试图想让urllib帮你把http请求通过该sock5代理发送，经过测试这是不行的！你得改用其它的http代理。
    proxy_handler = urllib.request.ProxyHandler({"http" : "127.0.0.1:8888", "https": "127.0.0.1:8888"})
    # 此用法很灵活，比如从proxy处理器和cookie处理器一起创建一个opener，完全可以！
    self.opener = urllib.request.build_opener(proxy_handler)
    url = "http://www.google.com"
    req = urllib.request.Request(url)
    req.add_header("User-Agent", "Windows / IE 9: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)")
    with opener.open(req, timeout=500) as f:
        print('status:', f.status, f.reason)
        for k, v in f.getheaders():
            print('%s: %s' % (k, v))
        print('Data:', f.read().decode('utf8'))

        # 查阅资料，得知通过代理访问还有两种方法：
        # 1.urlopen的proxies参数
        # 2.Request对象的set_proxy方法\u82e5\u8bbe\u7f6e\u4e86\u90a3\u4e48\u4f1a\u628acookie\u4fe1\u606f\u4fdd\u5b58\u5230\u8fd9\u91cc

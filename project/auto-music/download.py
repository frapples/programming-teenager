import subprocess

class CommandNotFoundError(Exception):
    def __init__(self, command):
        self.command = command

    def __str__(self):
        return "未找到%s命令，你可能没有安装这个软件。" % self.command

def download(link, out_path, seg=2, useragent=None):
    if not useragent:
        useragent = "Windows / IE 10: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)"

    try:
        code = subprocess.call(["aria2c", "-s", str(seg), link, "-o", out_path, "-U", "useragent"])
    except FileNotFoundError:
        raise CommandNotFoundError("aria2c")

    return code == 0

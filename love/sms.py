#! /usr/bin/env python3

import xml.dom.minidom
import os.path as op
import sys

BOY = 2
GIRL = 1

def main():
    xmlName = sys.argv[1]
    doc = xml.dom.minidom.parse(xmlName)
    smses = [(int(sms.getAttribute('type')), sms.getAttribute('body'), sms.getAttribute('time'))
            for sms in doc.childNodes[0].childNodes if sms.nodeName == 'sms']

    boyCount = len(['' for (who, _, __) in smses if who == BOY])
    girlCount = len(['' for (who, _, __) in smses if who == GIRL])

    str = '总数:%d 男孩:%d 女孩%d\r\n' % (boyCount + girlCount, boyCount, girlCount)
    for who, content, time in smses:
        str += '--------------------\r\n'
        str += '%s (%s)\r\n' % (('美美哒小芳#^_^#' if who == GIRL else '帅帅哒小果^_^') , time)
        str += '%s\r\n' % (content, )

    with open(op.splitext(xmlName)[0] + '.txt', 'wb') as f: 
        f.write(str.encode())



if __name__ == "__main__":
    main()

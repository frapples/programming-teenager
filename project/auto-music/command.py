from utils import *
import logging
import json
import os, os.path

"""
用途：该库提供了一种易用的方式来解析命令行参数。
示例：
# 将命令行参数传入
cmd = Command(sys.argv[1:])
cmd.bind("subcommand <sub_args>", callback, "命令示例")
第一个参数称之为usage，它使用一种描述性的语法来定义命令的格式。
如果命令行参数的格式与usage匹配，就从命令行参数里将各种数据提取出来，传入callbalck函数执行。
数据的名称在usage指定，数据的值从命令行参数中提取。

开始解析命令行参数，并运行对应的逻辑
cmd.run()


usage的格式：
subcommand1 subcommand2 ... <sub_arg1> <sub_arg2> ... [-b|--baba] <arg1> <arg2> -a <arg1> <arg2> [-v|--verbose]

如果某个sign不需要接数据，且是可选的，那么会将这个sign描述作为名字，将这个sign是否存在作为bool的值。
目前的实现中，除了bool型sign，不支持可选参数的语法。所有的sign，全都是可选的。

"""


class Command:
    def __init__(self, args):
        self._args = args
        self._binds = []
        self._logger = logging.getLogger("class Command")

    def bind(self, usage, func, description=''):
        self._binds.append((usage, func, description))

    def help(self):
        for usage, func, description in self._binds:
            print("%s    %s" % (usage, description))


    def debug_enable(self):
        self._logger.setLevel(logging.DEBUG)

    @property
    def args(self):
        return self._args

    def run(self):
        for usage, func, _ in self._binds:
            vars_ = self.parse_args_var(self.args, usage)
            if vars_ != self.MATCH_FAIL:
                func(**vars_)
                return
        print("命令格式错误。请阅读以下帮助：")
        self.help()

    # --------------------------

    MATCH_FAIL = -1


    def parse_args_var(self, args, usage):
        # TODO : 目前的实现中:
        # 所有的signs都是可选参数
        # -v 和 [-v]没有区别
        # bool型的-v没有实现

        def is_sign(str_):
            return (len(str_) >= 2 and
                    (str_[0] == "-" or (str_[0] == "[" and str_[-1] == "]")))

        def is_optional_sign(str_):
            return (len(str_) >= 2 and
                    (str_[0] == "[" and str_[-1] == "]"))

        def is_long_sign(str_):
            return (len(str_) >= 3 and str_[0:2] == "--")

        def is_var(str_):
            return len(str_) >= 1 and str_[0] == "<" and str_[len(str_) - 1] == ">"

        def get_varname(str_):
            if is_var(str_):
                return str_[1:-1]
            else:
                return ""

        def get_sign_names(str_):
            if is_optional_sign(str_):
                str_ = str_[1:-1]
            return str_.split("|")

        def split(args_or_usage_list):
            "分成subcommand和sign两部分"
            for i, item in enumerate(args_or_usage_list):
                if is_sign(item):
                    return args_or_usage_list[0:i], \
                           args_or_usage_list[i:]

            return args_or_usage_list, list()


        def parse_args_signs(args):
            signs = dict()

            current = ""
            for item in args:
                if is_sign(item):
                    current = item
                    signs[item] = list()
                else:
                    if current:
                        for sign in get_sign_names(current):
                            signs[sign].append(item)
                    else:
                        assert False
            return signs


        def parse_usage_signs(usage):
            signs = dict()
            optianal_signs = dict()
            equal_signs = list()

            current = ""
            for item in usage:
                if is_sign(item):
                    current = item
                    sign_names = get_sign_names(item)
                    equal_signs.append(sign_names)
                    for sign in sign_names:
                        signs[sign] = list()
                        optianal_signs[sign] = is_optional_sign(item)
                else:
                    if current:
                        for sign in get_sign_names(current):
                            signs[sign].append(item)
                    else:
                        assert False
            return signs, optianal_signs, equal_signs


        def get_bool_signs(usage_signs, equal_signs):
            "找出所有bool型sign，返回sign到varname的字典"
            bool_signs = dict()
            for sign in usage_signs.keys():
                if not usage_signs[sign] and optianal_signs[sign]:
                    equal_signs_ = [signs for signs in equal_signs if sign in signs][0]
                    long_signs = [sign_ for sign_ in equal_signs_ if is_long_sign(sign_)]
                    if long_signs:
                        var_name = long_signs[0][2:]
                        bool_signs[sign] = var_name
            return bool_signs


        vars = dict()

        args_sub_part, args_sign_part = split(args)
        usage_sub_part, usage_sign_part = split(usage.split())

        self._logger.debug("----------------- ");
        self._logger.debug("args_sub_part: %s", str(args_sub_part))
        self._logger.debug("usage_sub_part: %s", str(usage_sub_part))

        # 对比subcommand部分
        if len(args_sub_part) != len(usage_sub_part):
            return self.MATCH_FAIL


        for args_item, usage_item in zip(args_sub_part, usage_sub_part):
            if not is_var(usage_item):
                if args_item != usage_item:
                    return self.MATCH_FAIL
            else:
                vars[get_varname(usage_item)] = args_item


        # 对比sign部分
        args_signs = parse_args_signs(args_sign_part)
        usage_signs, optianal_signs, equal_signs = parse_usage_signs(usage_sign_part)

        self._logger.debug("args_signs: %s", str(args_signs))
        self._logger.debug("usage_signs: %s", str(usage_signs))

        # 非可选sign检查
        # for sign in optianal_signs.keys():
        #     if not optianal_signs[sign] and sign not in args_signs:
        #         return self.MATCH_FAIL

        for sign in args_signs.keys():
            if sign in usage_signs and \
               len(args_signs[sign]) == len(usage_signs[sign]):
                for var, value in zip(usage_signs[sign], args_signs[sign]):
                    vars[get_varname(var)] = value
            else:
                return self.MATCH_FAIL

        # 处理bool型的sign

        bool_signs = get_bool_signs(usage_signs, equal_signs)

        for bool_sign_varname in bool_signs.values():
            vars[bool_sign_varname] = False

        for sign in bool_signs.keys():
            if sign in args_signs:
                varname = bool_signs[sign]
                vars[varname] = True


        self._logger.debug("vars: %s", str(vars))
        return vars



#  命令行使用缓存
class CommandBuffer:
    def __init__(self, name):
        self._name = name
        self._base_no = 1
        self._max_no = 1000
        self._circle = Circle(self.file_path(self.name), self.max_length)


    @property
    def name(self):
        return self._name

    @property
    def base_no(self):
        return self._base_no

    @property
    def max_no(self):
        return self._max_no

    @property
    def max_length(self):
        return self._max_no - self._base_no + 1

    @base_no.setter
    def base_no(self, no):
        self._base_no = no


    @max_no.setter
    def max_no(self, no):
        self._max_no = no

    def add_items(self, items):
        start = self._circle.add_multi(items)
        self._circle.sync()
        return self.no_range(start, len(items))

    def no_range(self, start, count):
        i = start
        while (i < start + count):
            yield self._index2no(i)
            i = self._circle.next(i)

    def get(self, *kargs):
        if (len(kargs) == 1):
            index = self._no2index(kargs[0])
            return self._circle.get(index)

        if (len(kargs) == 2):
            start, end = kargs
            start, end = self._no2index(start), self._no2index(end)

            ret = []
            i = start
            while (i <= end):
                ret.append(self._circle.get(i))
                i = self._circle.next(i)
            return ret


    def _no2index(self, no):
        return no - self.base_no

    def _index2no(self, index):
        return index + self.base_no

    default_store_dir = ""
    @classmethod
    def set_default_store_dir(cls, dir_):
        cls.default_store_dir = dir_

    @classmethod
    def file_path(cls, file_name):
        "filename 没有扩展名部分"
        return os.path.join(cls.default_store_dir, file_name + ".commandbuffer.json")


class Circle:
    def __init__ (self, file_path, length):
        self._file = file_path
        self._length = length

        self._buffer, self._top = self._load_from_file(self.file)

    @property
    def file(self):
        return self._file

    @property
    def length(self):
        return self._length

    def next(self, index):
        return (index + 1) % self._length

    def add(self, item):
        index = self._top

        if index < len(self._buffer):
            self._buffer[index] = item
        else:
            assert index == len(self._buffer)

            self._buffer.append(item)

        self._top = self.next(self._top)
        return index


    def add_multi(self, items):
        index = self.add(items[0])
        for item in items[1:]:
            self.add(item)
        return index


    def get(self, index):
        return self._buffer[index]


    def sync(self):
        self._dump_to_file(self.file, (self._buffer, self._top))


    def _load_from_file(self, file):
        if not os.path.exists(file):
            return ([], 0)
        with open(file, "r") as f:
            return json.load(f)

    def _dump_to_file(self, file, data):
        with open(file, "w") as f:
            json.dump(data, f)

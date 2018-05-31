import time
import os.path
import json


def cache_decorator(func):
    def wrap(*args, **kwargs):
        key = json.dumps((func.__name__, args, kwargs))
        if cache_is_valid(key):
            res = get_cache_data(key)
        else:
            res = func(*args, **kwargs)
            cache_update(key, res)
        return res

    return wrap


# cache_file = ""
# cache_duration = 0
config = dict()
config["cache_file"] = ""
config["cache_duration"] = 0

def set_cache_file(file_):
    # cache_file = file_
    config["cache_file"] = file_

def set_cache_duration(duration):
    "cache有效时间，单位秒"
    config["cache_duration"] = duration

def cache_update(key, data):
    caches = load_caches()
    caches[key] = (int(time.time()), data)
    write_caches(caches)

def cache_is_valid(key):
    "该key是否有效"
    return cache_is_exists(key) and get_cache_time(key) + config["cache_duration"] > time.time()

def cache_is_exists(key):
    "该key是否存在于cache中"
    caches = load_caches()
    return key in caches

def get_cache_time(key):
    "该key对应的cache缓存时的时间"
    caches = load_caches()
    cache_time, data = caches[key]
    return cache_time

def get_cache_data(key):
    "该key对应的数据"
    caches = load_caches()
    cache_time, data = caches[key]
    return data


def remove_invalid_cache(caches):
    keys = list(caches.keys())
    for key in keys:
        cache_time, _ = caches[key]
        if cache_time + config["cache_duration"] <= time.time():
            caches.pop(key)
    return caches

def load_caches():
    if not os.path.exists(config["cache_file"]):
        return dict()
    with open(config["cache_file"], "r") as f:
        return json.load(f)


def write_caches(caches):
    with open(config["cache_file"], "w") as f:
        caches = remove_invalid_cache(caches)
        json.dump(caches, f)

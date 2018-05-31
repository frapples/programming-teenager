import re, json, base64
import hashlib
import urllib.parse

import requests

from . import cache

def open_url(url, get_data={}, post_data={}, referer=''):
    headers = {}
    headers['user-agent'] = "Windows / IE 10: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)"

    if referer:
        headers['referer'] = referer

    if not post_data:
        r = requests.get(url, params=get_data, headers=headers)
    else:
        r = requests.post(url, params=get_data, data=post_data, headers=headers)
    return r

def open_url_as_text(url, get_data={}, post_data={}, referer=''):
    return open_url(url, get_data, post_data, referer).text

def open_url_as_json(url, get_data={}, post_data={}, referer=''):
    return open_url(url, get_data, post_data, referer).json()


# --------------------------

def kugou_home_url():
    return "http://www2.kugou.kugou.com/yueku/v8/html/default.html?ver=7545"


def kugou_singer_home_url(singer_id):
    assert(singer_id != 0)
    return "http://www2.kugou.kugou.com/yueku/v8/singer/home/%s-0-0-s.html" % singer_id

# --------------------------


@cache.cache_decorator
def query_singers(name):
    url =  "http://www2.kugou.kugou.com/yueku/v8/singer/name/getData.js"
    get_data = { "cdn": "cdn",
                 "w": name,
                 "p": 1,
                 "s": 10
    }

    infos = open_url_as_json(url, get_data=get_data, referer=kugou_home_url())

    # 整理下API传过来的信息
    singers = list()
    if infos:
        for info in infos["data"]:
            # 传回来的歌手名后面带有地区名，去掉
            singer = dict()
            singer["name"]        = info["singername"].replace("(%s)" % info["areaname"], "")
            singer["song_count"]  = info["song_count"]
            singer["album_count"] = info["album_count"]
            singer["id"]          = info["singerid"]
            singers.append(singer)
    return  singers

# --------------------------


@cache.cache_decorator
def query_albums(singer_id):
    # 注意：这里一次最多只能拉100个专辑下来
    url = "http://www2.kugou.kugou.com/yueku/v8/singer/album/getData.js"
    get_data = {
        "cdn": "cdn",
        "sid": singer_id,
        "p": "1",
        "s": "100"
    }

    infos = open_url_as_json(url, get_data=get_data, referer=kugou_singer_home_url(singer_id))

    # 重新把专辑信息整理一下
    albums = list()
    if infos:
        for info in infos["data"]:
            album = dict()
            album["name"] = info["albumname"]
            album["publish_time"] = info["publish_time"]
            album["song_count"] = info["song_count"]
            album["introduction"] = info["intro"]
            album["id"] = info["albumid"]
            albums.append(album)
    return albums


# --------------------------

def adapt_song_info(origin_info):
    # 重新把歌曲信息整理一下
    # 注意：从album和singer查询到的数据不太一样，如歌曲id。
    song = dict()
    song["name"] = origin_info["songname"]
    song["id"] = origin_info["id"] if "id" in origin_info else origin_info["songid"]
    song["publish_time"] = origin_info["publish_time"]
    song["duration"] = origin_info["duration"] / 1000

    qualities = dict()
    if origin_info["filesize_320"] > 0 :
        qualities["320"] = (origin_info["filesize_320"], origin_info["hash_320"])

    if origin_info["filesize_flac"] > 0 :
        qualities["flac"] = (origin_info["filesize_flac"], origin_info["hash_flac"])

    if origin_info["filesize_ape"] > 0 :
        qualities["ape"] = (origin_info["filesize_ape"], origin_info["hash_ape"])

    if origin_info["filesize"] > 0:
        bitrate = str(origin_info["bitrate"])
        qualities[bitrate] = (origin_info["filesize"], origin_info["hash"])

    song["qualities"] = qualities
    return song


@cache.cache_decorator
def query_songs_from_singer(singer_id):
    url = "http://www2.kugou.kugou.com/yueku/v8/singer/song/getData.js"
    get_data = {
        "cdn": "cdn",
        "sid": singer_id,
        "p" : "1",
        "s": 1000
    }

    infos = open_url_as_json(url, get_data=get_data, referer=kugou_singer_home_url(singer_id))
    if not infos:
        return []
    else:
        return [adapt_song_info(info) for info in infos["data"]]

@cache.cache_decorator
def query_songs_from_album(album_id):
    url = "http://www2.kugou.kugou.com/yueku/v8/album/song/getData.js"

    get_data = {
        "cdn": "cdn",
        "aid": album_id
    }

    infos = open_url_as_json(url, get_data=get_data)
    if not infos:
        return []
    else:
        return [adapt_song_info(info) for info in infos["data"]]


def query_songs_from_keyword(keyword):
    # 用的web版酷狗的API 似乎抓不到无损的hash
    # 所暂时用它来从关键字查询歌名和对应歌手
    url = "http://mobilecdn.kugou.com/api/v3/search/song"
    get_data = {
        "format": "jsonp",
        "keyword": keyword,
        "page": 1,
        "pagesize": 30,
        "showtype": 1,
        "callback": "kgJSONP505299152"
    }
    text = open_url_as_text(url, get_data=get_data)

    text = re.findall(r"%s\((.+)\)" % get_data["callback"], text)[0]

    infos = json.loads(text)["data"]["info"]

    songs = list()
    for info in infos:
        song = dict()
        song["name"] = info["songname"]
        song["singer_name"] = info["singername"]
        songs.append(song)
    return songs



# --------------------------

@cache.cache_decorator
def query_song_download_link(hash_):
    url = "http://trackercdn.kugou.com/i/"

    get_data = {
        "acceptMp3": "1",
        "pid": "6",
        "cmd": "4",
        "key": hashlib.md5((hash_.lower() + "kgcloud").encode()).hexdigest(),
        "hash": hash_.lower()
    }

    info = open_url_as_json(url, get_data=get_data)
    return info["url"]

# --------------------------


def query_lyric(song_name, song_time, hash_, type_="lrc"):
    assert(type_ in ("lrc", "krc"))

    # keyword用歌手 - 歌名格式可提高成功率，但是仍有可能失败。此时可尝试下只用歌名
    url = "http://lyrics.kugou.com/search?ver=1&man=no&client=pc&keyword=%s&duration=%s&hash=%s" % \
            (urllib.parse.quote(song_name), int(song_time * 1000), hash_.lower())
    info = open_url_as_json(url)
    if not info or not info["status"] == 200:
        return ''
    info = info["candidates"][0]

    url = "http://lyrics.kugou.com/download?ver=1&client=pc&id=%s&accesskey=%s&fmt=%s&charset=utf8" % \
            (info["id"], info["accesskey"], type_)
    info = open_url_as_json(url)

    if not (info and info["status"] == 200):
        return ''

    content = base64.b64decode(info["content"].encode())
    return content



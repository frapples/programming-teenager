#! /usr/bin/env python3
import os, os.path
_DIR_ = os.path.dirname(os.path.realpath(__file__))
import sys
# sys.path.append(os.path.join(_DIR_, '..', '..', 'mylib'))
import urllib.parse
import logging
from functools import reduce
import urllib.parse

import requests

import command
from playlist import Playlist
import api, api.cache, api.music
import download



def global_config():
    logging.basicConfig(level=logging.ERROR)
    api.cache.set_cache_file(os.path.join(_DIR_, "data", "tmp", "api-cache.json"))
    api.cache.set_cache_duration(24 * 60 * 60)
    Playlist.set_default_store_dir("data/playlist")
    command.CommandBuffer.set_default_store_dir("data/tmp")

# 顺序不能颠倒
global_config()
g_song_buffer = command.CommandBuffer("song")

def main():
    cmd = command.Command(sys.argv[1:])
    # cmd.debug_enable()
    cmd.bind("help", cmd.help, "显示命令帮助")
    cmd.bind("show-singer <singername>", show_singer, "显示某歌手的统计信息")
    cmd.bind("show-album <singername> <albumname>", show_album, "显示某歌手某专辑的信息")
    cmd.bind("list-album <singername> [-v|--verbose]", list_album, "显示某歌手的所有专辑(-v显示更多信息)")
    cmd.bind("list-song -s <singername> -a <albumname> [-v|--verbose]", list_song, "显示某歌手（或某专辑）的所有歌曲(-v显示更多信息)")
    cmd.bind("search-song  <singername> <songname>", search_song, "按歌手和歌曲名搜索歌曲")
    cmd.bind("search <keyword> ", search_keyword, "按关键字搜索")
    cmd.bind("download-link <singername> <songname>", download_link, "按歌手和歌名搜索歌曲，并列出这首歌的下载地址")
    cmd.bind("show-lyric <singername> <songname> -t <lyric_type>", show_lyric, "按歌手和歌名搜索歌曲，并输出这首歌的歌词")

    # 播放列表
    cmd.bind("playlist show", playlist_show, "列出播放列表里的歌曲")
    cmd.bind("playlist add <song_no>", playlist_add, "添加歌曲到播放列表")
    cmd.bind("playlist clear", playlist_clear, "清空播放列表里的歌曲")
    cmd.bind("playlist set-quality", playlist_set_qualities_order, "设置音质优先级列表")
    cmd.bind("playlist save-link <filepath>", playlist_save_download_link, "把播放列表歌曲的下载链接保存到指定文件")
    cmd.bind("playlist download <dir> [--krc] [--lrc] [--onlylyric]", playlist_download, "把播放列表歌曲下载到指定位置")

    cmd.run()

def playlist_show():
    playlist = Playlist()
    songs = playlist.get_songs()
    for i, song in enumerate(songs):
        print_nonl("%d." % (i + 1))
        print_nonl(dump_song(song, False))

def playlist_addlast():
    pass

def playlist_add(song_no):
    song_no = int(song_no)
    song = g_song_buffer.get(song_no)
    playlist = Playlist()
    playlist.add_song(song)
    print("已添加歌曲：")
    print_nonl(dump_song(song, False, str(song_no) + "."))

def playlist_set_qualities_order():
    def dump_quality_order(order):
        s = ""
        for quality in order:
            s += quality + " "
        return s

    playlist = Playlist("default")
    print("当前优先级：%s" % dump_quality_order(playlist.quality_order))

    choise_list = [["128", "192", "320", "ape", "flac"],
                   ["128", "192", "320", "flac", "ape"],
                   ["ape", "flac", "320", "192", "128"],
                   ["flac", "ape", "320", "192", "128"],
                   ["320", "ape", "flac", "192", "128"],
                   ["320", "192", "flac", "ape", "128"],
                   ["320", "192", "ape", "flac", "128"],
                   ["320", "flac", "ape", "192", "128"]]
    print()
    print("选择序号来设置优先级")
    for i, order in enumerate(choise_list):
        print(str(i + 1) + "." + dump_quality_order(order))

    key = 0
    while (True):
        print("请选择序号:")
        key = int(input()) - 1
        if key < len(choise_list):
            break
        print("输入不在范围内")

    playlist.quality_order = choise_list[key]

def playlist_clear():
    playlist = Playlist("default")
    playlist.clear()

def playlist_save_download_link(filepath):
    playlist = Playlist("default")
    songs = playlist.get_songs()
    with open(filepath, "w") as f:
        for song in songs:
            link = playlist.get_download_link(song, api.query_download_link)
            f.write(link + "#" + urllib.parse.quote(song["singer_name"] + " - " + song["name"]) + "\r\n")

def playlist_download(dir, krc, lrc, onlylyric):
    playlist = Playlist("default")
    songs = playlist.get_songs()

    for song in songs:
        if not onlylyric:
            link = playlist.get_download_link(song, api.query_download_link)
            _, ext = os.path.splitext(link)
            file_name = api.music.song_file_name_noext(song) + ext
            path = os.path.join(dir, file_name)

            # 以后判存的方式需要改进
            if os.path.exists(path):
                print("%s已经存在，无需下载。" % path)
            else:
                print("开始下载%s" % path)
                try:
                    download.download(link, path)
                except download.CommandNotFoundError as e:
                    print("错误: %s" % str(e))

        lyric_types = list()
        if lrc: lyric_types.append("lrc")
        if krc: lyric_types.append("krc")
        for type_ in lyric_types:
            file_name = api.music.song_file_name_noext(song) + "." + type_
            path = os.path.join(dir, file_name)
            print("开始保存歌词：%s" % file_name)
            text = api.query_lyric(song, type_)
            with open(path, "wb") as f:
                f.write(text)



def show_singer(singername):
    singer = api.query_singer(singername)

    if singer:
        print("==============================")
        print("歌手名：%s" % singer["name"])
        print("总歌曲数：%s" % singer["song_count"])
        print("总专辑数：%s" % singer["album_count"])
    else:
        print("抱歉，没有该歌手的信息。。。")

    print()

def show_album(singername, albumname):
    print(singername, albumname)
    pass

def list_album(singername, verbose):
    albums = api.query_albums(singername)

    print("==============================")
    for album in albums:
        print("-------------")
        print("专辑名称：%s" % album["name"])
        if verbose:
            print("专辑介绍：%s" % album["introduction"])
        print("歌曲数量：%s" % album["song_count"])
        print("发行时间：%s" % album["publish_time"])
    print()


def list_song(singername,  verbose, albumname=None):
    print("==============================")
    # TODO: albumname is not None时
    if albumname is None:
        songs = api.query_songs_from_singer(singername)

        range_ = g_song_buffer.add_items(songs)

        for song, no in zip(songs, range_):
            print_nonl(dump_song(song, False, str(no) + "."))


def search_keyword(keyword):
    songs = api.query_songs_from_keyword(keyword)
    for song in songs:
        print("%s \t %s" % (song["singer_name"], song["name"]))


def search_song(singername, songname):
    songs = api.query_songs_from_singer(singername)
    songs = [song for song in songs if song["name"].find(songname) >= 0]

    if not songs:
        print("没有找到匹配的歌曲。。。")
    else:

        range_ = g_song_buffer.add_items(songs)
        for song, no in zip(songs, range_):
            print_nonl(dump_song(song, False, str(no) + "."))

def download_link(singername, songname):
    songs = api.query_songs_from_singer(singername)
    songs = [song for song in songs if song["name"].find(songname) >= 0]

    if songs:
        for song in songs:
            print("-------------")
            print("%s (%d分%d秒)" % (song["name"], song["duration"] // 60, song["duration"] % 60))
            for quality in song["qualities"]:
                print(api.query_download_link(song, quality))

def show_lyric(singername, songname, lyric_type):
    if lyric_type not in ("krc", "lrc"):
        print("歌词类无法识别。")
        return

    songs = api.query_songs_from_singer(singername)
    songs = [song for song in songs if song["name"].find(songname) >= 0]
    if songs:
        for song in songs:
            print("-------------")
            print("%s (%d分%d秒)" % (song["name"], song["duration"] // 60, song["duration"] % 60))
            lyric = api.query_lyric(song, lyric_type)
            print("*******")
            print(lyric)
            print("*******")


def dump_song(song, verbose, pre_str=''):
    s = ""
    if verbose:
        s += "-------------" + "\n"
        s += pre_str
        s += "歌曲名：%s" % song["name"] + "\n"
        s += "歌曲时长：%d分%d秒" % (song["duration"] // 60, song["duration"] % 60) + "\n"
        s += "发行时间：%s" % (song["publish_time"] if song["publish_time"] else "未知") + "\n"
        for quality in song["qualities"].keys():
            quality_to_des_map = {"128": "普通(128Kps)",
                                    "192": "普通(192Kps)",
                                    "320": "高品(320Kps)",
                                    "ape": "无损(ape)",
                                    "flac": "无损(flac)"
            }
            s += "音质：" + quality_to_des_map.get(quality, quality + "Kps") + "\n"
    else:
        s += pre_str + "%s (%d分%d秒)" % (song["name"], song["duration"] // 60, song["duration"] % 60) + "\n"
    return s



def print_nonl(obj):
    sys.stdout.write(str(obj))


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("程序被用户打断。")
    except requests.exceptions.RequestException as e:
        print("在尝试在连接网络时，发生了一个问题。")
        print("可能是网络没连好，也可能是API失效。")
        print("详细信息：")
        print(e)
    except Exception as e:
        print("程序可能出现了bug。请联系开发者告诉它以下信息以帮助修复bug：")
        raise e




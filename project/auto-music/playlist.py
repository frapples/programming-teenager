import os
import os.path
import json


class Playlist:
    """
    对播放列表的抽象。
    播放列表里存放的是歌曲。
    歌曲之间是有顺序的。
    """
    def __init__(self, name="default"):
        "创建或者返回一个已经存在的播放列表"
        self._name = name
        data = self._load()
        self._songs = data["songs"]
        self._quality_order = data["quality_order"]

    @property
    def name(self):
        return self._name

    def add_song(self, song):
        self._songs.append(song)
        self.sync()

    def add_songs(self, songs):
        self._songs += songs
        self.sync()

    def del_song(self, index):
        self._songs.pop(index)
        self.sync()

    def clear(self):
        self._songs.clear()
        self.sync()

    def get_songs(self):
        return self._songs

    @property
    def quality_order(self):
        return self._quality_order

    @quality_order.setter
    def quality_order(self, order):
        self._quality_order = order
        self.sync()


    def get_download_link(self, song, api_func):
        quality = self._find_close_quality([key for key in song["qualities"].keys()], self.quality_order)
        return api_func(song, quality)

    def _find_close_quality(self, qualities, order):
        def str_to_int(str_, default=0):
            try:
                return int(str_)
            except ValueError:
                return default

        for quality in order:
            if quality in ("ape", "flac"):
                if quality in qualities:
                    return quality
            else:
                for q in qualities:
                    if abs(str_to_int(q) - int(quality)) < 50:
                        return quality
        return ""

    def _load(self):
        default = {"songs":[], "quality_order": ["320", "ape", "flac", "192", "128"]}
        ret = self.load_from_file(self.name)
        if not ret:
            ret = dict()

        for key in default.keys():
            if key not in ret:
                ret[key] = default[key]

        return ret

    def sync(self):
        return self.dump_to_file(self.name, {"songs": self._songs, "quality_order": self._quality_order})


    default_store_dir = ""

    @classmethod
    def set_default_store_dir(cls, dir_):
        cls.default_store_dir = dir_

    @classmethod
    def file_path(cls, file_name):
        "filename 没有扩展名部分"
        return os.path.join(cls.default_store_dir, file_name + ".json")

    @classmethod
    def load_from_file(cls, file_name):
        if not os.path.exists(cls.file_path(file_name)):
            return None
        with open(cls.file_path(file_name), "r") as f:
            return json.load(f)

    @classmethod
    def dump_to_file(cls, file_name, data):
        with open(cls.file_path(file_name), "w") as f:
            json.dump(data, f)




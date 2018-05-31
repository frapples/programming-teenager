from . import baseapi

def query_songs_from_singer(singer_name):
    """
    给出歌手名，查询出所有歌曲
    @param  str  singer_name 歌手名
    @return list             歌曲信息。每项是个dict，记录了每一首歌的信息。
    """
    singers = baseapi.query_singers(singer_name)
    if not singers:
        return list()

    songs = baseapi.query_songs_from_singer(singers[0]['id'])

    # 给歌曲添加歌手名
    for song in songs:
        song["singer_name"] = singer_name

    return songs

def query_albums(singer_name):
    "给出歌手名，查询出所有专辑"
    singers = baseapi.query_singers(singer_name)
    if not singers:
        return list()

    albums = baseapi.query_albums(singers[0]["id"])
    return albums

def query_songs_from_singer_album(singer_name, album_name=""):
    "给出歌手名和专辑名，查询出所有歌曲"
    pass

def query_singer(singer_name):
    """
    给出歌手名，查询歌手信息
    """
    singers = baseapi.query_singers(singer_name)
    if not singers:
        return dict()
    else:
        return singers[0]

def query_download_link(song_info, quality):
    qualities = song_info["qualities"]
    if quality not in qualities:
        return ""
    filesize, hash_ = qualities[quality]
    link = baseapi.query_song_download_link(hash_)
    return link



def query_lyric(song_info, lyric_type):
    """
    @param  dict song_info 歌曲信息
    @param  str lyric_type 歌词类型，lrc或krc
    @return bytes          歌词
    """
    first = list(song_info["qualities"].keys())[0]
    _, hash_ = song_info["qualities"][first]
    return baseapi.query_lyric(song_info["name"], song_info["duration"], hash_, lyric_type)

def query_songs_from_keyword(keyword):
    return baseapi.query_songs_from_keyword(keyword)

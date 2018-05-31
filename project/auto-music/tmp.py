import api
import api.cache
from api import baseapi
import json

import os, os.path
_DIR_ = os.path.dirname(os.path.realpath(__file__))
api.cache.set_cache_file(os.path.join(_DIR_, "data", "tmp", "api-cache.json"))
api.cache.set_cache_duration(24 * 60 * 60 * 30)

def main():
    keys = ["志",  "明", "薛", "莫", "陈"]

    singers = []
    for key in keys:
        singers += baseapi.query_singers(key)

    print(len(singers))
    print(singers)

    songs = []
    for singer in singers:
        print("query singers %s" % singer['name'])
        try:
            ss = baseapi.query_songs_from_singer(singer['id'])
        except Exception as e:
            print(e)
            continue

        # 给歌曲添加歌手名
        for s in ss:
            s["singer_name"] = singer['name']

        songs += ss

    print(len(songs))

    with open("data.json", "w") as f:
        json.dump(songs, f)



if __name__ == "__main__":
    main()

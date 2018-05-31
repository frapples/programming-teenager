import json
import random

with open("song.json") as f:
    songs = json.load(f)


sql = ""
for song in songs:
    user_id = random.randint(1, 5)
    t = int(song["duration"])
    h = t // 60 // 60
    m = t // 60 - h * 60
    s = t - m * 60 - h * 60 * 60
    b_time = "%02d:%02d:%02d" % (h, m, s)
    sql += "INSERT INTO music (name, singer, album, b_time, user_id) VALUES" + \
            '("{name}", "{singer_name}", "{album_name}", "{b_time}", {user_id});\n'.format(user_id=user_id, b_time=b_time, **song)

with open("data.sql", "w", newline="\r\n") as f:
    f.write(sql)


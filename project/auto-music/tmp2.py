import json
import random

with open("data.json") as f:
    songs = json.load(f)

with open("album.json") as f:
    albums = json.load(f)


for song in songs:
    names = [a['name'] for a in albums if a['singer_name'] == song['singer_name']]
    if not names:
        names = ["无专辑"]
    song["album_name"] = names[random.randint(0, len(names) - 1)]


#  for song in songs:
    #  print(song)


with open("song.json", "w") as f:
    json.dump(songs, f)

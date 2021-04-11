import os
import csv
from bs4 import BeautifulSoup
from itertools import zip_longest

visited_urls = []
lyrics_list = []

path = "gaana.com"

for filename in os.listdir(path):
    if filename.endswith(".html"):
        fullpath = os.path.join(path, filename)
        
    soup = BeautifulSoup(open(fullpath, "html.parser"))
    try:
        lyrics = soup.find("div",class_="lyrics").get_text()
        print(lyrics)
    except AttributeError:
        pass
        
    lyrics_list.append(lyrics)
    if fullpath not in visited_urls:
        visited_urls.append(fullpath)
        
        
rows = [visited_urls, lyrics_list]
export_data = zip_longest(*rows, fillvalue = '')

with open('songs.csv', mode='w') as csv_file:
    wr = csv.writer(csv_file, delimiter=',')
    wr.writerow("Song Url", "Song Lyrics")
    wr.writerows(export_data)

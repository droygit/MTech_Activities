import spacy
import heapq
import csv
import pandas as pd
import numpy as np
from flask import Flask, render_template, request
from sklearn.metrics.pairwise import cosine_similarity

app = Flask(__name__)

dat = []
liststr = []
songnamelist = []

def data_precessing():
    with open ('song_lyrics_debug.csv') as f:
        reader = csv.reader(f, delimiter=",")
        for row in reader:
            #print("+++++")
            #print(row)
            lyrics = "".join(row[3]).split()
            lyrics = [word.lower() for word in lyrics]
            #print("Lyrics : ", lyrics)
            inputstr = ' '.join(lyrics)
            liststr.append(inputstr)
            songname = ''.join(row[1]).split()
            #print("Song name : ", songname)
            #print(len(songname))
            songnamel = ' '.join(songname)
            songnamelist.append(songnamel)
            #print("Song List : ", songnamelist)
            #print("list str: ", liststr)
            #print("+++++")
            
    print("Data processing complete!")


def search_engine(topmostnum,query):
        nlp = spacy.load('en_core_web_sm')
        spacy_stopwords = spacy.lang.en.stop_words.STOP_WORDS
        
        lyricstr = []

        for w in spacy_stopwords:
            nlp.vocab[w].is_stop = True

        #Tokenization
        for word in liststr:
            doc = nlp(word)
            #print("Doc : ", doc)
            #Stop word filteration
            tokens = [token.lemma_ for token in doc if not token.is_stop and not token.is_punct]
            #print("Token : ", tokens)
            lyricstr.append(tokens)

        #vectors = np.array([nlp(" ".join(lyric)).vector for lyric in lyricstr])
        vector = [nlp(" ".join(lyric)).vector for lyric in lyricstr]
        vectors = np.array(vector)
        
        temp = ' '.join(query)

        tempvec = nlp(temp)
        print(tempvec.vector)

        docvector = np.array((tempvec.vector))
        docvector = docvector.reshape(1,-1)
        print("Dec vector : ", docvector)
        
        arr = []

        cosineVal = (cosine_similarity(docvector, vectors)).flatten()
        print("Cosign")
        print(cosineVal)
        c_query = cosineVal.tolist()
        arraypos = heapq.nlargest(topmostnum, c_query)
        print("c_query : ", c_query)
        print("arraypos : ", arraypos)
        for element in arraypos:
            array = c_query.index(element)
            arr.append(array)
        print(arr)
        for in_ele in arr:
            dat.append([songnamelist[in_ele],liststr[in_ele]])
        
        print (dat)
        return dat

#if __name__ == "__main__":
#    data_precessing()
#    search_engine(2,['hi','hello','bye'])


@app.route('/', methods=['POST', 'GET'])
def home():
    return render_template('search.html')

@app.route('/results', methods=['GET', 'POST'])
def search_request():

    search_term = request.form["input"]
    search_term = search_term.split(',')
    top_most_num = int(request.form["num"])
    print(top_most_num, search_term)
    dat.clear()

    result=search_engine(top_most_num, search_term)
    pd.set_option('display.max_colwidth', -1)
    df = pd.DataFrame(result,columns=['Song Name','Song Lyrics'])
    return render_template('results.html',words=search_term,num=top_most_num,data=df.to_html(col_space=500))

if __name__ == "__main__":
  app.run(host='localhost', port=9000)
  
  
import spacy
import heapq
from spacy import lemmatizer
import numpy as np
import csv
from sklearn.metrics.pairwise import cosine_similarity
import csv
import pandas as pd
from flask import Flask, render_template, request

app = Flask(__name__)



dat = []
def backend(query):
        nlp = spacy.load('en_core_web_lg')
        spacy_stopwords = spacy.lang.en.stop_words.STOP_WORDS

        liststr = []
        songnamelist = []

        with open ('14.csv') as f:
            reader = csv.reader(f, delimiter=",")
            for row in reader:
                text = "".join(row[3]).split()
                text = [word.lower() for word in text]
                #print(text)
                inputstr = ' '.join(text)
                liststr.append(inputstr)
                songname = ''.join(row[1]).split()
                print(songname)
                print(len(songname))
                songnamel = ' '.join(songname)
                songnamelist.append(songnamel)
                print(songnamelist)

        #print("Number of songs in the list: ",len(liststr))
        #print(len(songnamelist))

        lyricstr = []

        customize_stop_words = ['[Verse]', '[Chorus]', '[Pre-Chorus]', ',', '"', '[', ']', 'chorus', '-', '?', '(', ')',':','.','!']

        for w in customize_stop_words:
           nlp.vocab[w].is_stop = True

        for i in liststr:
            doc = nlp(i)
            tokens = [token.lemma_ for token in doc if not token.is_stop]
            #tokens = [token.lemma_ for token in doc if not token.lemma_.is_punct | n.lemma_ != "-PRON-"]
            #tokens = [token.lemma_ for token in token]
            #tokens = [token.lemma_ for token in tokens]
            #print(tokens)
            lyricstr.append(tokens)

        vectors = np.array([nlp(" ".join(lyric)).vector for lyric in lyricstr])

        #print("Vector representation of songs: ", vectors)
        #print(vectors.shape)

        templove = ['heart','lovely', 'family','caring','forever','trust','passion','romance','sweet','kiss','love','hugs','warm','fun','kisses','joy', 'sex','friendship','marriage','husband','wife','forever']
        templove = ' '.join(templove)
        temphappy = ["cheerful", 'happy',"content", "delighted","ecstatic", "elated","glad", "joyful","joyous","joy", "jubilant", "lively", "merry","overjoyed","peaceful","pleasant", "pleased","thrilled", "upbeat","blessed","blest","blissful", "blithe","captivated", "chipper", "chirpy","convivial", "exultant", "gay","gleeful", "gratified","intoxicated", "jolly","laughing", "light", "mirthful", "peppy","perky", "playful","sparkling", "sunny","tickled", "tickled pink", "up", "satisfy"]
        temphappy = ' '.join(temphappy)
        tempmotivation = ["catalyst", "desire", "encouragement", "impetus", "impulse", "incentive", "inclination","interest","motive", "reason", "wish", "action", "actuation", "angle", "disposition", "drive", "fire", "gimmick", "goose", "hunger", "impulsion", "incitation", "incitement", "inducement", "instigation", "kick", "persuasion","predetermination", "predisposition", "provocation", "push", "spur", "stimulus", "suggestion", "get", "right stuff"]
        tempmotivation = ' '.join(tempmotivation)
        tempsad = ["sad", "irritate","lousy","upset","incapable","enraged","disappointment","doubtful","alone","hostile","discourage","uncertain","paralyze","insult","ashame","indecisive","fatigue","sore","powerless","perplex","useless","annoy","diminish","embarrass","inferior","upset","guilty","hesitant","vulnerable","hateful","dissatisfy","shy","empty","unpleasant","miserable","stupefied","forced","offensive","detestable","disillusion","hesitant","bitter","repugnant","unbelieving","despair","aggressive","despicable","skeptical","frustrated","resentful","disgusting","distrustful","distress","inflame","abominable","misgiving","woeful","provoke","terrible","lost","pathetic","incensed","indespair","unsure","tragic","infuriate","sulky","uneasy","cross","bad","dominate","tense","boil","insensitive","fearful","crush","tearful","dull","terrified","torment","sorrowful","nonchalant","suspicious","deprive","neutral","anxious","pain","grief","reserve","alarm","torture","anguish","weary","panic","deject","desolate","bore","nervous","reject","desperate","preoccupied","scare","injure","pessimistic","cold","worry","offend","unhappy","disinterest","frighten","afflict","lonely","lifeless","timid","ache","grieve","shaky","victim","mourn","restless","heartbroken","dismay","doubt","agony","threaten","coward","humiliate","wrong","menace","alienate","wary"]
        tempsad = ' '.join(tempsad)

        tempvech = nlp(temphappy)
        tempvecl = nlp(templove)
        tempvecm = nlp(tempmotivation)
        tempvecs = nlp(tempsad)
        #print(tempvec.vector)


        docvectorh = np.array((tempvech.vector))
        docvectorh = docvectorh.reshape(1,-1)

        docvectorl = np.array((tempvecl.vector))
        docvectorl = docvectorl.reshape(1,-1)

        docvectorm = np.array((tempvecm.vector))
        docvectorm = docvectorm.reshape(1,-1)

        docvectors = np.array((tempvecs.vector))
        docvectors = docvectors.reshape(1,-1)

        arrh = []
        arrl = []
        arrm = []
        arrs = []

        number_of_elements = 10

        #query = input("Enter your mood: ")

        if query in temphappy:
            coshappy = (cosine_similarity(docvectorh, vectors)).flatten()
            #print(coshappy)
            chappy = coshappy.tolist()
            sortedarray1 = heapq.nlargest(number_of_elements, chappy)
            #print(chappy)
            for x1 in sortedarray1:
                array = chappy.index(x1)
                arrh.append(array)
            #print(arrh)
            for in_ele in arrh:
                print("Top 10 Happy songs \n")
                print("Song Name: ", songnamelist[in_ele])
                print("Song Lyrics: \n", liststr[in_ele])
                dat.append([songnamelist[in_ele],liststr[in_ele]])
                print("abc",dat)
            return dat
        elif query in templove:
            coslove = (cosine_similarity(docvectorl, vectors)).flatten()
            # print(coshappy)
            clove = coslove.tolist()
            sortedarray2 = heapq.nlargest(number_of_elements, clove)
            # print(chappy)
            for x2 in sortedarray2:
                array = clove.index(x2)
                arrl.append(array)
            #print(arrl)
            for in_ele in arrl:
                print("Top 10 Romantic songs \n")
                print("Song Name: ", songnamelist[in_ele])
                print("Song Lyrics: \n", liststr[in_ele])
                dat.append([songnamelist[in_ele], liststr[in_ele]])
                print("abc", dat)
            return dat
        elif query in tempmotivation:
            cosmotivation = (cosine_similarity(docvectorm, vectors)).flatten()
            # print(coshappy)
            cmotivation = cosmotivation.tolist()
            sortedarray3 = heapq.nlargest(number_of_elements, cmotivation)
            # print(chappy)
            for x3 in sortedarray3:
                array = cmotivation.index(x3)
                arrm.append(array)
            #print(arrm)
            for in_ele in arrm:
                print("Top 10 Motivational songs \n")
                print("Song Name: ", songnamelist[in_ele])
                print("Song Lyrics: \n", liststr[in_ele])
                dat.append([songnamelist[in_ele], liststr[in_ele]])
                print("abc", dat)
            return dat
        elif query in tempsad:
            cossad = (cosine_similarity(docvectors, vectors)).flatten()
            # print(coshappy)
            csad = cossad.tolist()
            sortedarray4 = heapq.nlargest(number_of_elements, csad)
            # print(chappy)
            for x4 in sortedarray4:
                array = csad.index(x4)
                arrs.append(array)
           # print(arrs)
            for in_ele in arrs:
                print("Top 10 Sad songs \n")
                print("Song Name: ", songnamelist[in_ele])
                print("Song Lyrics: \n", liststr[in_ele])
                dat.append([songnamelist[in_ele], liststr[in_ele]])
                print("abc", dat)
            return dat
        else:
            pass
        

@app.route('/', methods=['POST', 'GET'])
def home():
    return render_template("home.html")

@app.route('/results', methods=['GET', 'POST'])
def search_request():

    search_term = request.form["input"]
    print(search_term)
    dat.clear()
    dum=backend(search_term)
    pd.set_option('display.max_colwidth', -1)
    df = pd.DataFrame(dum,columns=['Song Name','Lyrics'])
    #print(df)
    return render_template("results.html",data=df.to_html(col_space=500))

if __name__ == "__main__":
  with open('14.csv') as f:
      reader = csv.DictReader(f)
      #helpers.bulk(es, reader, index='my-index', doc_type='my-type')
  app.secret_key = 'mysecret'
  app.run(host='localhost', port=5014)

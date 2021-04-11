#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sun Apr 11 14:58:56 2021

@author: ubuntu1
"""
import spacy

sentence2 = "I'm looking for all of the lemmas. Please help me find them!"
sentence = "This is a sample sentence, showing off the stop words filtration."
nlp = spacy.load('en_core_web_sm')
spacy_stopwords = spacy.lang.en.stop_words.STOP_WORDS
doc = nlp(sentence)
print(doc)
doc = [token.lemma_ for token in doc if not token.is_stop and not token.is_punct]
print(doc)

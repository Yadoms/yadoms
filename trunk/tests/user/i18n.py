import yadomsServer
import os.path
import json
import codecs

def get():
   """Return the translations array"""
   
   print ("os.path.join(yadomsServer.wwwLocalesPath(), fr.json)=", os.path.join(yadomsServer.wwwLocalesPath(), "fr.json"))
   translations = json.load(codecs.open(os.path.join(yadomsServer.wwwLocalesPath(), "fr.json"), "r", "utf-8-sig"))
   return translations
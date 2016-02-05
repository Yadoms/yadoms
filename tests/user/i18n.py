import yadomsServer
import os.path
import json
import codecs


def currentLocale():
   """Return the current browser locale"""
   
   return 'fr'
   

def get():
   """Return the translations array"""
   
   translations = json.load(codecs.open(os.path.join(yadomsServer.wwwLocalesPath(), currentLocale() + ".json"), "r", "utf-8-sig"))
   return translations
   
   
def getPlugin(pluginName):
   """Return the translations array for a plugin"""
   
   translations = json.load(codecs.open(os.path.join(yadomsServer.pluginsPath(), pluginName, "locales", currentLocale() + ".json"), "r", "utf-8-sig"))
   return translations
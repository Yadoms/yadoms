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
   
   
def getPlugin(pluginType):
   """Return the translations array for a plugin"""

   try:
      translations = json.load(codecs.open(os.path.join(yadomsServer.pluginsPath(), pluginType, "locales", currentLocale() + ".json"), "r", "utf-8-sig"))
   except:
      print ('getPlugin : {pluginType} not found')
   
   return translations
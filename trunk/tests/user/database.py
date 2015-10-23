import yadomsServer
import os
import os.path

def new():
   """Use a new database (= delete database if exist)"""

   if (os.path.isfile(yadomsServer.databasePath())):
      os.remove(yadomsServer.databasePath())

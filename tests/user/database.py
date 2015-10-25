import yadomsServer
import os
import os.path
import shutil

def new():
   """Use a new database (= delete database if exist)"""

   if (os.path.isfile(yadomsServer.databasePath())):
      os.remove(yadomsServer.databasePath())

         
def withOneStoppedRule():
   """Deploy a database containing one rule in stopped state"""
      
   shutil.copyfile(os.path.join("resources", "databases", "OneStoppedRule.db3"), yadomsServer.databasePath())
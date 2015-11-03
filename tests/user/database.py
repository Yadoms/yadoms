import yadomsServer
import resources
import os
import os.path
import shutil

def new():
   """Use a new database (= delete database if exist)"""

   resources.deleteFile(yadomsServer.databasePath())

         
def withOneStoppedRule():
   """Deploy a database containing one rule in stopped state"""
      
   shutil.copyfile(os.path.join("resources", "databases", "OneStoppedRule.db3"), yadomsServer.databasePath())
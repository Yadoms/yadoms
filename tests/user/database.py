import yadomsServer
import resources
import os
import os.path
import shutil

def new():
   """Use a new database (= delete database if exist)"""

   resources.deleteFile(yadomsServer.databasePath())

         
def deploy(databaseName):
   """Deploy a specific database"""
      
   shutil.copyfile(os.path.join("resources", "databases", databaseName + ".db3"), yadomsServer.databasePath())
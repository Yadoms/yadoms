import yadomsServer
import resources
import os.path

def new():
   """Use a new database (= delete database if exist)"""

   resources.deleteFile(yadomsServer.databasePath())

         
def deploy(databaseName):
   """Deploy a specific database"""

   resources.deployFile(os.path.join("resources", "databases", databaseName + ".db3"), yadomsServer.databasePath())

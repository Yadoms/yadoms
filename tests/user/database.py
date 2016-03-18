import yadomsServer
import resources
import os.path

def new():
   print 'Use new database'
   resources.deleteFile(yadomsServer.databasePath())

         
def deploy(databaseName):
   print 'Deploy database ', databaseName
   resources.deployFile(os.path.join("resources", "databases", databaseName + ".db3"), yadomsServer.databasePath())

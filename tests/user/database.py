import yadomsServer
import resources
import os.path

def new():
   print 'Use new database'
   resources.deleteFile(yadomsServer.databasePath())
   resources.deleteFile(yadomsServer.databasePath() + "-journal")

         
def deploy(databaseName):
   deployFromPath(os.path.join("resources", "databases", databaseName + ".db3"))

         
def deployFromPath(databaseFullPath):
   print 'Deploy database from ', databaseFullPath
   resources.deleteFile(yadomsServer.databasePath() + "-journal")
   resources.deployFile(databaseFullPath, yadomsServer.databasePath())

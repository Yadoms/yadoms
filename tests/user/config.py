import yadomsServer
import resources
import os.path

         
def deploy(configName):
   print ('Deploy configuration {configName}')
   resources.deployFile(os.path.join("resources", "config", configName + ".ini"), os.path.join(yadomsServer.binaryPath(), "yadoms.ini"))

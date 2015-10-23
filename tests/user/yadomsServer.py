import subprocess
import time

def binaryPath():
   """return the server binary path"""
   
   # TODO dépend de l'OS
   return "../../builds/DEBUG"
   
   
def databasePath():
   """return the server database path"""
   
   return binaryPath() + "/yadoms.db3"
   
   
def scriptsPath():
   """return the scripts database path"""
   
   return binaryPath() + "/scripts"
   
   
def start():
   """Start the Yadoms server"""
   # TODO nom de l'exécutable OS-dependant
   return subprocess.Popen(binaryPath() + "/yadoms.exe", 1)
   
   
def stop(yadomsProcess):
   """Stop the Yadoms server"""

   yadomsProcess.terminate()

   for it in range(10):
      if (yadomsProcess.poll() != None):
         return
      time.sleep(1)
      
   yadomsProcess.kill()

   
def restart():
   """Restart the Yadoms server"""
   stop()
   start()

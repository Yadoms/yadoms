import subprocess
import signal
import time
import os
import psutil

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
   return subprocess.Popen(binaryPath() + "/yadoms.exe")
   
   
def stop(yadomsProcess):
   """Kill Yadoms server with its sup-processes"""

   parent = psutil.Process(yadomsProcess.pid)
   for child in parent.children(True):
      os.kill(child.pid, signal.SIGINT)
   os.kill(parent.pid, signal.SIGINT)

           
def restart():
   """Restart the Yadoms server"""
   stop()
   start()

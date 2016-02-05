import subprocess
import signal
import time
import os
import os.path
import psutil
import platform
from selenium.webdriver.support.ui import WebDriverWait

def binaryPath():
   """return the server binary path"""
   
   if (platform.system() == "Windows"):
      return os.path.join("..", "..", "builds", "DEBUG")
   else:
      return os.path.join("..", "..", "builds")
   
   
def databasePath():
   """return the server database path"""
   
   return os.path.join(binaryPath(), "yadoms.db3")
   
   
def scriptsPath():
   """return the scripts path"""
   
   return os.path.join(binaryPath(), "scripts")
   
   
def pluginsPath():
   """return the plugins path"""
   
   return os.path.join(binaryPath(), "plugins")
   
   
def wwwLocalesPath():
   """return the locales path of the web client"""
   
   return os.path.join(binaryPath(), "www", "locales")
   
   
def start():
   """Start the Yadoms server"""
   
   if (platform.system() == "Windows"):
      return subprocess.Popen(os.path.join(binaryPath(), "yadoms.exe"))
   else:
      return subprocess.Popen(os.path.join(binaryPath(), "yadoms"))
   
   
def isProcessRunning(pid):  
   """Check if the process is running"""

   try:
      os.kill(pid, 0)
   except OSError as err:
      if err.errno == errno.ESRCH:
         return False
   return True
    
   
def stop(yadomsProcess):
   """Kill Yadoms server with its sup-processes"""

   parent = psutil.Process(yadomsProcess.pid)
   
   for child in parent.children(True):
      os.kill(child.pid, signal.SIGINT)
      while not isProcessRunning(child.pid):
         time.sleep(1)
         
   os.kill(parent.pid, signal.SIGINT)
   while not isProcessRunning(parent.pid):
      time.sleep(1)

           
def restart():
   """Restart the Yadoms server"""
   stop()
   start()

def openClient(browser):
   """Open a client on local server and wait for full loading"""
   
   browser.get("http://127.0.0.1:8080")
   WebDriverWait(browser, 10).until(lambda driver: driver.execute_script("return document.readyState") == u"complete" and driver.execute_script("return jQuery.active") == 0)
   
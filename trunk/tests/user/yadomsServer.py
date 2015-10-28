import subprocess
import signal
import time
import os
import os.path
import psutil
from selenium.webdriver.support.ui import WebDriverWait

def binaryPath():
   """return the server binary path"""
   
   # TODO dépend de l'OS
   return os.path.join("..", "..", "builds", "DEBUG")
   
   
def databasePath():
   """return the server database path"""
   
   return os.path.join(binaryPath(), "yadoms.db3")
   
   
def scriptsPath():
   """return the scripts database path"""
   
   return os.path.join(binaryPath(), "scripts")
   
   
def wwwLocalesPath():
   """return the locales path of the web client"""
   
   return os.path.join(binaryPath(), "www", "locales")
   
   
def start():
   """Start the Yadoms server"""
   
   # TODO nom de l'exécutable OS-dependant
   return subprocess.Popen(os.path.join(binaryPath(), "yadoms.exe"))
   
   
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

def openClient(browser):
   """Open a client on local server and wait for full loading"""
   
   browser.get("http://127.0.0.1:8080")
   WebDriverWait(browser, 10).until(lambda driver: driver.execute_script("return document.readyState") == u"complete" and driver.execute_script("return jQuery.active") == 0)
   
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

   if (platform.system() == "Windows"):
		try:
		   os.kill(pid, 0)
		except OSError as err:
		   if err.errno == errno.ESRCH:
		      return False
		return True
   else:
      return os.path.exists("/proc/" + str(pid))
      
      
def doKill(pid):
   if (platform.system() == "Windows"):
      os.kill(pid, signal.SIGINT)
   else:
      os.kill(pid, signal.SIGKILL)
      
  
def stop(yadomsProcess):
   """Kill Yadoms server with its sup-processes"""

   print "######## TRY TO STOP YADOMS ########"
   parent = psutil.Process(yadomsProcess.pid)
   
   print "Kill Yadoms..."
   for child in parent.children(True):
      doKill(child.pid)
      while isProcessRunning(child.pid):
         time.sleep(1)
         
   if (platform.system() != "Windows"):
      parent = yadomsProcess

   doKill(parent.pid)
   counter = 0
   while isProcessRunning(parent.pid):
      counter += 1
      time.sleep(1)
      if counter > 2:
         break

   print "Yadoms killed"

           
def restart():
   """Restart the Yadoms server"""
   stop()
   start()

def openClient(browser):
   """Open a client on local server and wait for full loading"""
   
   browser.get("http://127.0.0.1:8080")
   WebDriverWait(browser, 10).until(lambda driver: driver.execute_script("return document.readyState") == u"complete" and driver.execute_script("return jQuery.active") == 0)
   

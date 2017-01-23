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
   
   
def logsPath():
   """return the logs path"""
   
   return os.path.join(binaryPath(), "logs")
   
   
def pluginLogsPath():
   """return the plugin log path"""
   
   return os.path.join(logsPath(), "plugins")
   
   
def scriptLogsPath():
   """return the script log path"""
   
   return os.path.join(logsPath(), "scripts")
   
   
def scriptsPath():
   """return the scripts path"""
   
   return os.path.join(binaryPath(), "scripts")
   
   
def pluginsPath():
   """return the plugins path"""
   
   return os.path.join(binaryPath(), "plugins")
   
   
def wwwLocalesPath():
   """return the locales path of the web client"""
   
   return os.path.join(binaryPath(), "www", "locales")
   
   
def start(startupArgs=[]):
   """Start the Yadoms server"""
   
   # Set logLevel to none if not provided
   if not startupArgs or all("logLevel=" not in arg for arg in startupArgs):
      startupArgs.append("logLevel=none")

   argsLine = ""
   for arg in startupArgs:
      argsLine += " --" + arg

   if (platform.system() == "Windows"):
      return subprocess.Popen(os.path.join(binaryPath(), "yadoms.exe" + argsLine))
   else:
      return subprocess.Popen(os.path.join(binaryPath(), "yadoms" + argsLine))
   
   
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
         
      
def killProcTree(pid, including_parent=True):
   """Kill a parent process with its children"""
   parent = psutil.Process(pid)
   children = parent.children(recursive=True)
   for child in children:
      child.kill()
      psutil.wait_procs(children, timeout=5)
   if including_parent:
      try:
         parent.kill()
         parent.wait(20)
         print 'Process killed'
      except:
         print 'Error : process still alive'
         raise
      
  
def stop(yadomsProcess):
   """Kill Yadoms server with its sup-processes"""

   print 'Kill Yadoms...'
   killProcTree(yadomsProcess.pid)

           
def restart():
   """Restart the Yadoms server"""
   stop()
   start()

def openClient(browser):
   """Open a client on local server and wait for full loading"""

   import time
   time.sleep(10)  # TODO : improve that to be sure that yadomsServer is ready for web client connection

   browser.get("http://127.0.0.1:8080")
   WebDriverWait(browser, 10).until(lambda driver: driver.execute_script("return document.readyState") == u"complete" and driver.execute_script("return jQuery.active") == 0)
   

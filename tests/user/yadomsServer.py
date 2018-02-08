import subprocess
import signal
import time
import os
import os.path
import psutil
import platform
from selenium.webdriver.support.ui import WebDriverWait
from psutil import process_iter

def binaryPath():
   """return the server binary path"""
   
   if (platform.system() == "Windows"):
      return os.path.join("..", "..", "builds", "DEBUG")
   else:
      return os.path.join("..", "..", "builds")


def executableName():
   """Return the executable name"""
   return "yadoms.exe" if platform.system() == "Windows" else "yadoms"

   
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

   cmdLine = []
   cmdLine.append(os.path.join(binaryPath(), executableName()))
   for arg in startupArgs:
      cmdLine.append("--" + arg)

   print 'Start server ...' 
   print '  ', ' '.join(str(item) for item in cmdLine)
   serverProcess = psutil.Popen(cmdLine)

   # DEBUG
   import datetime
   print datetime.datetime.now(), ': [DEBUG] server process status =', serverProcess.status()   #TODO virer
   import imp
   try:
      imp.find_module('wmi')
      import wmi
      c = wmi.WMI ()
      for process in c.Win32_Process ():
         print process.ProcessId, process.Name
   except ImportError:
      print 'Unable to print running processes, "wmi" module not installed'

   # END DEBUG

   if waitServerStarted() == True:
      return serverProcess

   print 'Server failed to start'
   stop(serverProcess)
   assert False
   
   
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
   for child in parent.children(recursive=True):
      try:
         child.kill()
      except:
         pass
   psutil.wait_procs(parent.children(recursive=True), timeout=5)
   if including_parent:
      try:
         parent.kill()
         parent.wait(20)
         print 'Process killed'
      except:
         print 'Error : process still alive'
         assert False
      
  
def stop(yadomsProcess):
   """Kill Yadoms server with its sub-processes"""

   print 'Stop Yadoms...'
   # Sending signal.CTRL_C_EVENT doesn't work, so kill process tree
   killProcTree(yadomsProcess.pid)
   print 'Yadoms stopped'
      
  
def ensureStopped():
   """Ensure that Yadoms server is stopped"""
   for process in process_iter():
      if process.name() == executableName():
         try:
            killProcTree(process.ppid())
         except:
            pass


def waitServerStarted():
   import datetime
   import requests
   timeout = datetime.datetime.now() + datetime.timedelta(seconds = 30)
   while datetime.datetime.now() < timeout:
      try:
         response = requests.post('http://127.0.0.1:8080/rest/general/system', timeout=1)
         if response.status_code == requests.codes.ok:
            print 'Server started'
            return True
      except:
        pass

   print 'Server not responding'
   return False


def waitPageLoaded(browser, waitForReadyForNormalOperation = True):
   if WebDriverWait(browser, 60).until(lambda browser: browser.execute_script("return (document.readyState == 'complete' && jQuery.active == 0)")):
      if not waitForReadyForNormalOperation:
         return True
      if WebDriverWait(browser, 60).until(lambda browser: len(browser.find_elements_by_class_name("tabPagePills")) > 0):
         return True
   return False


def openClient(browser, waitForReadyForNormalOperation = True):
   """Open a client on local server and wait for full loading"""

   try:
      browser.get("http://127.0.0.1:8080")
      if waitPageLoaded(browser, waitForReadyForNormalOperation):
         return
   except:
      print 'Exception waiting page loading'

   print 'Unable to load client'
   assert False

import time
import inspect
import os.path
import shutil
import yadomsServer

def doUntil(actionFct, conditionFct, timeout = 10):
   """Do a action every second until a condition. Returns True if conditionFct was satisfed, False if timeout"""
   """Condition must return bool value"""

   tries = 0
   while True:
      actionFct()
      if conditionFct():
         return True
      
      time.sleep(1)
      tries += 1
      if tries > timeout:
         return False


def waitUntil(conditionFct, timeout = 10):
   """Wait until a condition. Returns True if conditionFct was satisfed, False if timeout"""
   """Condition must return bool value"""

   return doUntil(lambda: None, conditionFct, timeout)
         

def tryWhile(actionFct, timeout = 10):
   """Try to do an action every second until a timeout. Raise original action exception if still fails after timeout"""

   tries = 0
   while True:
      try:
         actionFct()
         return
      except:
         time.sleep(1)
         tries += 1
         if tries > 10:
            print ('Try {timeout} times to do {actionFct} unsuccessfull')
            raise


def waitReadyForInput(element):
   """Wait that element is ready for an input"""
   assert waitUntil(lambda: element.is_enabled())


def deleteContext():
   contexteMainDirectory = os.path.join('report', 'debug')
   if os.path.exists(contexteMainDirectory):
      try:
         tryWhile(lambda : shutil.rmtree(contexteMainDirectory))
      except:
         print ('Unable to delete context backup directory')
         raise


def saveContext(browser):
   callerFonctionName = inspect.stack()[1][3]
   contexteDirectory = os.path.join('report', 'debug', callerFonctionName)
   print ('Save contexte to {contexteDirectory}')
   if not os.path.exists(contexteDirectory):
      os.makedir(contexteDirectory)
      
   browser.save_screenshot(os.path.join(contexteDirectory, 'screenshot.png'))
   if os.path.exists(yadomsServer.databasePath()):
      shutil.copyfile(yadomsServer.databasePath(), os.path.join(contexteDirectory, os.path.basename(yadomsServer.databasePath())))
   if os.path.exists(yadomsServer.scriptsPath()):
      shutil.copytree(yadomsServer.scriptsPath(), os.path.join(contexteDirectory))

def highlight(self, element):
    """Highlights (blinks) a Selenium Webdriver element"""
    def apply_style(s):
        self.browser.execute_script("arguments[0].setAttribute('style', arguments[1]);",
                              element, s)
    original_style = element.get_attribute('style')
    apply_style("background: yellow; border: 2px solid red;")
    time.sleep(10)
    apply_style(original_style)
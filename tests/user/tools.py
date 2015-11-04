import time


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
            print "Try ", timeout, " times to do ", actionFct, ", unsuccessfull"
            raise

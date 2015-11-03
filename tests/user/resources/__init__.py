import os.path
import shutil
import time


def deleteTree(directory):
   """Remove directory even if not empty (retry for 10 seconds if remove fails)"""

   if (os.path.isdir(directory)):
      tries = 0
      while True:
         try:
            shutil.rmtree(directory)
            return
         except:
            time.sleep(1)
            tries += 1
            if tries > 10:
               print "Unable to delete ", directory, " path"
               raise
               
               
def deleteFile(file):
   """Remove file (retry for 10 seconds if remove fails)"""

   if (os.path.isfile(file)):
      tries = 0
      while True:
         try:
            os.remove(file)
            return
         except:
            time.sleep(1)
            tries += 1
            if tries > 10:
               print "Unable to delete ", file, " file"
               raise
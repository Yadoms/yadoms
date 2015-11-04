import os.path
import shutil
import time
import tools


def deleteTree(directory):
   """Remove directory even if not empty (retry for 10 seconds if remove fails)"""

   if (os.path.isdir(directory)):
      try:
         tools.tryWhile(lambda : shutil.rmtree(directory))
      except:
         print "Unable to delete ", directory, " path"
         raise
               
               
def deleteFile(file):
   """Remove file (retry for 10 seconds if remove fails)"""

   if (os.path.isfile(file)):
      try:
         tools.tryWhile(lambda : os.remove(file))
      except:
         print "Unable to delete ", file, " file"
         raise
         
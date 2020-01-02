import os.path
import shutil
import time
import tools
import sys


def deleteTree(directory):
   """Remove directory even if not empty (retry for 10 seconds if remove fails)"""

   if (os.path.isdir(directory)):
      try:
         tools.tryWhile(lambda : shutil.rmtree(directory))
      except:
         print ('Unable to delete {directory} path')
         raise
               
               
def deleteFile(file):
   """Remove file (retry for 10 seconds if remove fails)"""

   if (os.path.isfile(file)):
      try:
         tools.tryWhile(lambda : os.remove(file))
      except:
         print ('Unable to delete {file} file')
         raise


def deployFile(srcFile, targetFile):
   """Deploy a file"""
   
   # In some case (at least under Windows), if target file is in use, copyFile does nothing without any error
   # So try to delete target file previously (will fail if not possible)
   try:
      deleteFile(targetFile)
      shutil.copyfile(srcFile, targetFile)
   except:
      print ('Unable to deploy database file {srcFile}, sys.exc_info()[0]')
      raise
   

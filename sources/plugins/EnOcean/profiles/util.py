#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# Util common functions

import os
import os.path
import xml.etree.ElementTree
import string
import sys
import inspect

EnableMessages = False


#-------------------------------------------------------------------------------
def createParentDir(path):
   parentDir = os.path.dirname(path)
   if not parentDir:
      return
   try:
      os.mkdir(parentDir)
   except:
      pass


#-------------------------------------------------------------------------------
def __LINE__():
   return inspect.getouterframes(inspect.currentframe())[2][2]
def __FILE__():
   return os.path.abspath(inspect.getouterframes(inspect.currentframe())[2][1])

__nb_warnings__ = 0
__nb_errors__ = 0

def info(s):
   if not EnableMessages:
      return
   print(__FILE__() + "(" + str(__LINE__()) + "):" + " information: " + s)

def warning(s):
   global __nb_warnings__
   __nb_warnings__ += 1
   if not EnableMessages:
      return
   print(__FILE__() + "(" + str(__LINE__()) + "):" + " warning: " + s)

def error(s):
   global __nb_errors__
   __nb_errors__ += 1
   print(__FILE__() + "(" + str(__LINE__()) + "):" + " error: " + s)

def finish():
   resultText = "Conversion failed" if __nb_errors__ != 0 else "Successfully converted"
   print("   --- " + resultText + " (" +  str(__nb_errors__) + ") error(s), (" + str(__nb_warnings__) + ") warning(s) ---")
   sys.exit(0 if __nb_errors__ == 0 else 1)



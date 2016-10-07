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

def warning(s):
   print __FILE__() + "(" + str(__LINE__()) + "):" + " warning: " + s

__ERROR__ = False
def error(s):
   print __FILE__() + "(" + str(__LINE__()) + "):" + " error: " + s
   __ERROR__ = True

def getError():
   return  __ERROR__



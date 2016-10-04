#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# Util common functions

import os.path
import xml.etree.ElementTree
import string


#-------------------------------------------------------------------------------
def createParentDir(path):
   parentDir = os.path.dirname(path)
   if not parentDir:
      return
   try:
      os.mkdir(parentDir)
   except:
      pass

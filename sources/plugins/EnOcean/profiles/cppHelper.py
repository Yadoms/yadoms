#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# Util common functions for CPP generation

import glob
import re
import os.path
from sets import Set




#-------------------------------------------------------------------------------
def toCppName(label):
   def acceptedChar(char):
      return True if char.isalnum() or char == "_" else False
      
   normalizedLabel = label.strip().replace(" ", "_").replace("+", "_plus_").replace("-", "_").replace(".", "_").replace(",", "_").replace("(", "_") \
      .replace(")", "_").replace("/", "_").replace(u"μ", "u").replace("&", "_and_")
   return ''.join([char for char in list(normalizedLabel) if acceptedChar(char)])


#-------------------------------------------------------------------------------
def toEnumValueName(label):
   return "k" + toCppName(label)


#-------------------------------------------------------------------------------
def getMapInitCode(enumValues):
   initCode = "boost::assign::map_list_of\n"
   for enumValue in enumValues:
      enumValueName = enumValue[0]
      initCode += "   (" + toEnumValueName(enumValueName)
      initCode += ", \"" + enumValueName.strip() + "\")\n"
   initCode += ";\n"
   return initCode


#-------------------------------------------------------------------------------
class HardCodedProfiles():

   def __init__(self, profilePath):
      self.__hardCodedProfiles__ = []
      self.__profileHardCodedFiles__ = Set()
      pattern = re.compile(r'^class CProfile_(.*) : public IType')
      for profilePath in glob.glob(os.path.join(profilePath, "hardCoded", '*.h')):
         with open(profilePath, 'r') as hardCodedFile:
            for line in hardCodedFile:
               items = pattern.match(line)
               if items is not None:
                  self.__hardCodedProfiles__.append(items.group(1).replace('_', '-'))
                  self.__profileHardCodedFiles__.add(os.path.basename(profilePath))

   def isProfileHardCoded(self, profile):
      return profile in self.__hardCodedProfiles__

   def getProfileHardCodedFiles(self):
      return self.__profileHardCodedFiles__

   def getProfilesHardCoded(self):
      return self.__hardCodedProfiles__


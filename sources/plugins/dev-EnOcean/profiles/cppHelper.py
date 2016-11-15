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
typesHardCoded = None
typesHardCodedFiles = Set()
def buildTypesHardCodedList(profilePath):
   typesHardCoded = []
   global typesHardCodedFiles
   pattern = re.compile(r'^class (.*) : public IType')
   for hardCodedPath in glob.glob(os.path.join(profilePath, "hardCoded", '*.h')):
      with open(hardCodedPath, 'r') as hardCodedFile:
         for line in hardCodedFile:
            items = pattern.match(line)
            if items is not None:
               typesHardCoded.append(items.group(1))
               typesHardCodedFiles.add(os.path.basename(hardCodedPath))
   return typesHardCoded

def isTypeHardCoded(typeClassName, profilePath):
   global typesHardCoded
   if not typesHardCoded:
      typesHardCoded = buildTypesHardCodedList(profilePath)
   return typeClassName in typesHardCoded

def getTypesHardCodedFiles():
   global typesHardCodedFiles
   return typesHardCodedFiles

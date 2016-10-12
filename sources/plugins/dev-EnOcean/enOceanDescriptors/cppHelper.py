#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# Util common functions for CPP generation






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

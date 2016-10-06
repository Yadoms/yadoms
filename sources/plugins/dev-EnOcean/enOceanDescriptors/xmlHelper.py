#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# Util common functions

import os.path
import xml.etree.ElementTree
import string




#-------------------------------------------------------------------------------
def getAllNodes(xmlInputFile, xmlElementPath):
   xmlRootNode = xml.etree.ElementTree.parse(xmlInputFile).getroot()

   # Some checks on xmlElementPath input
   xmlElementPathArray = string.split(xmlElementPath, "/")
   if xmlElementPathArray.count < 2:
      raise Exception("getAllNodes : Invalid xmlElementPath \"" + xmlElementPath + "\" : must at least be in the form \"root.element\"")

   if xmlElementPathArray[0] != xmlRootNode.tag:
      raise Exception("getAllNodes : Invalid xmlElementPath \"" + xmlElementPath + "\" : invalid root name")

   # Remove root name
   xmlElementPathArray.pop(0)

   return xmlRootNode.findall(string.join(xmlElementPathArray, "/"))




#-------------------------------------------------------------------------------
def getEnumValues(inNode, foreachSubNode, enumValueNameTag, enumValueTag = None):
   enumValues = []
   for child in inNode.findall(foreachSubNode):
      enumValues.append([child.find(enumValueNameTag).text, child.find(enumValueTag).text if enumValueTag else None])
   return enumValues




#-------------------------------------------------------------------------------
def findInDatafield(datafieldXmlNode, select, where, equals):
   for item in datafieldXmlNode.find("enum").findall("item"):
      if (item.find(where).text == equals):
         return item.find(select).text
   return None
   


#-------------------------------------------------------------------------------
def findUsefulDataFieldNodes(inXmlNode):
   usefulDataFieldNodes = []
   for xmlDataFieldNode in inXmlNode.findall("datafield"):
      if xmlDataFieldNode.find("data") is not None:
         dataText = xmlDataFieldNode.find("data").text
         if dataText and dataText != "LRN Bit":
            usefulDataFieldNodes.append(xmlDataFieldNode)
   return usefulDataFieldNodes

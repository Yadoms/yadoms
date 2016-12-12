#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP class generation

import re

#-------------------------------------------------------------------------------
def __normalizeItem__(item):
   return re.match(r"^(0x)?([0-9A-F]{2})$", item, flags=re.IGNORECASE).group(2).upper()

#-------------------------------------------------------------------------------
def profileName(rorgXmlNode, funcXmlNode, typeXmlNode):
   """ Create the profile name (ie A5-02-05) from XML nodes """
   rorg = __normalizeItem__(rorgXmlNode.find("number").text)
   func = __normalizeItem__(funcXmlNode.find("number").text)
   type = __normalizeItem__(typeXmlNode.find("number").text)
   return rorg + "-" + func + "-" + type


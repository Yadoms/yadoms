#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP class generation

import re

#-------------------------------------------------------------------------------
def _normalizeItem(item):
   return re.match(r"^(0x)?([0-9A-F]{2})$", item, flags=re.IGNORECASE).group(2).upper()

#-------------------------------------------------------------------------------
def profileName(rorgXmlNode, funcXmlNode, typeXmlNode):
   """ Create the profile name (ie A5-02-05) from XML nodes """
   rorg = _normalizeItem(rorgXmlNode.find("number").text)
   func = _normalizeItem(funcXmlNode.find("number").text)
   type = _normalizeItem(typeXmlNode.find("number").text)
   return rorg + "-" + func + "-" + type


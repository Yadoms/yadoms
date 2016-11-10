#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP code generation script for EnOcean EEP profiles

import sys
import os.path
import string
import codecs
import re
import json

import util

from collections import OrderedDict


def sortProfiles(inProfiles):
   tempProfiles = {}
   for profile in inProfiles:
      # Split rorg, func and type as number to sort them
      profilItems = re.match(r"([0-9A-F]{2})-([0-9A-F]{2})-([0-9A-F]{2})", profile, flags=re.IGNORECASE)
      rorg = int(profilItems.group(1), 16)
      func = int(profilItems.group(2), 16)
      type = int(profilItems.group(3), 16)
      if rorg not in tempProfiles:
         tempProfiles[rorg] = {}
      if func not in tempProfiles[rorg]:
         tempProfiles[rorg][func] = {}
      tempProfiles[rorg][func][type] = 0
   # Sort profiles
   orderedTempProfiles = OrderedDict(sorted(tempProfiles.items()))
   for rorg in orderedTempProfiles:
      orderedTempProfiles[rorg] = OrderedDict(sorted(tempProfiles[rorg].items()))
      for func in orderedTempProfiles[rorg]:
         orderedTempProfiles[rorg][func] = OrderedDict(sorted(tempProfiles[rorg][func].items()))
   # Format sorted profiles
   outProfiles = []
   for rorg in orderedTempProfiles:
      for func in orderedTempProfiles[rorg]:
         for type in orderedTempProfiles[rorg][func]:
            outProfiles.append(format(rorg, '02X') + '-' + format(func, '02X') + '-' + format(type, '02X'))
   return outProfiles


def generate(packageJsonInPath, packageJsonPath, supportedProfiles):
   util.createParentDir(packageJsonPath)
   with codecs.open(packageJsonInPath, 'r', 'utf_8') as packageJsonInFile, codecs.open(packageJsonPath, 'w', 'utf_8') as packageJsonFile:

      package = json.load(packageJsonInFile, object_pairs_hook=OrderedDict)

      supportedProfiles = sortProfiles(supportedProfiles)

      package['manuallyDeviceCreationConfigurationSchema']['profile']['content'] = OrderedDict()
      package['deviceConfiguration']['staticConfigurationSchema'][0]['schema']['profile']['content'] = OrderedDict()
      for profile in supportedProfiles:
         profileNode = OrderedDict()
         profileNode['type']='section'
         if profile == 'D2-01-12' or profile == 'F6-02-01':
            paramNode = OrderedDict()
            paramNode['localControl'] = OrderedDict()
            paramNode['localControl']['type'] = 'enum'
            paramNode['localControl']['values'] = OrderedDict()
            paramNode['localControl']['values']['enable'] = 'enable'
            paramNode['localControl']['values']['disable'] = 'disable'
            paramNode['localControl']['defaultValue'] = 'enable'
            paramNode['taughtIn'] = OrderedDict()
            paramNode['taughtIn']['type'] = 'enum'
            paramNode['taughtIn']['values'] = OrderedDict()
            paramNode['taughtIn']['values']['allDevices'] = 'all devices'
            paramNode['taughtIn']['values']['yadomsOnly'] = 'Yadoms only'
            paramNode['taughtIn']['defaultValue'] = 'allDevices'
            profileNode['content'] = paramNode
         else:
            profileNode['content'] = ''
         package['manuallyDeviceCreationConfigurationSchema']['profile']['content'][profile] = profileNode
         package['deviceConfiguration']['staticConfigurationSchema'][0]['schema']['profile']['content'][profile] = profileNode
      json.dump(package, packageJsonFile, indent=2)


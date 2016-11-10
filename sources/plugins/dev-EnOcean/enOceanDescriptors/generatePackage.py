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
import copy

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
   orderedProfiles = OrderedDict(sorted(tempProfiles.items()))
   for rorg in orderedProfiles:
      orderedProfiles[rorg] = OrderedDict(sorted(tempProfiles[rorg].items()))
      for func in orderedProfiles[rorg]:
         orderedProfiles[rorg][func] = OrderedDict(sorted(tempProfiles[rorg][func].items()))
   return orderedProfiles


#
# Generate package.json from package.in.json
#
# Take the general structure from package.in.json, and the specific configurations for certain devices
#
def generate(packageJsonInPath, packageJsonPath, supportedProfiles):
   util.createParentDir(packageJsonPath)
   with codecs.open(packageJsonInPath, 'r', 'utf_8') as packageJsonInFile, codecs.open(packageJsonPath, 'w', 'utf_8') as packageJsonFile:

      inPackage = json.load(packageJsonInFile, object_pairs_hook=OrderedDict)

      profiles = sortProfiles(supportedProfiles)

      outPackage = copy.deepcopy(inPackage)
      outPackage['manuallyDeviceCreationConfigurationSchema']['profile']['content'] = OrderedDict()
      outPackage['deviceConfiguration']['staticConfigurationSchema'][0]['schema']['profile']['content'] = OrderedDict()
      del outPackage['specificProfilesConfigurations']

      for rorg in profiles:
         for func in profiles[rorg]:
            for type in profiles[rorg][func]:
               profileName = format(rorg, '02X') + '-' + format(func, '02X') + '-' + format(type, '02X')

               profileNode = OrderedDict()
               profileNode['name']=profileName
               profileNode['type']='section'
               if profileName in inPackage['specificProfilesConfigurations']:
                  profileNode['content'] = inPackage['specificProfilesConfigurations'][profileName]['content']
               else:
                  profileNode['content'] = ''
               outPackage['manuallyDeviceCreationConfigurationSchema']['profile']['content'][profileName] = profileNode
               outPackage['deviceConfiguration']['staticConfigurationSchema'][0]['schema']['profile']['content'][profileName] = profileNode

      json.dump(outPackage, packageJsonFile, indent=2)


#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP code generation script for EnOcean EEP profiles

import sys
import os.path
import string
import codecs
import glob
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
# Generate package.json from package.in.json, and locales XX.json files from XX.in.json files
#
# Take the general structure from package.in.json, and the specific configurations for certain devices (idem for locales files)
#
def generate(packageJsonInPath, packageJsonPath, localesInPath, localesPath, supportedProfiles):
   # packege.json

   profiles = sortProfiles(supportedProfiles)

   util.createParentDir(packageJsonPath)
   with codecs.open(packageJsonInPath, 'r', 'utf_8') as packageJsonInFile:

      inPackage = json.load(packageJsonInFile, object_pairs_hook=OrderedDict)

      outPackage = copy.deepcopy(inPackage)
      outPackage['deviceConfiguration']['staticConfigurationSchema']['schemas']['all']['content']['profile']['content'] = OrderedDict()
      del outPackage['specificProfilesConfigurations']

      for rorg in profiles:
         for func in profiles[rorg]:
            for type in profiles[rorg][func]:
               profileName = format(rorg, '02X') + '-' + format(func, '02X') + '-' + format(type, '02X')

               profileNode = OrderedDict()
               profileNode['name'] = profileName
               profileNode['type'] = 'section'
               if profileName in inPackage['specificProfilesConfigurations']:
                  if 'sameAs' in inPackage['specificProfilesConfigurations'][profileName]:
                     aliasProfile = inPackage['specificProfilesConfigurations'][profileName]['sameAs']
                     profileNode['content'] = inPackage['specificProfilesConfigurations'][aliasProfile]['content']
                  else:
                     profileNode['content'] = inPackage['specificProfilesConfigurations'][profileName]['content']
               else:
                  profileNode['content'] = ''
               outPackage['deviceConfiguration']['staticConfigurationSchema']['schemas']['all']['content']['profile']['content'][profileName] = profileNode

      with codecs.open(packageJsonPath, 'w', 'utf_8') as packageJsonFile:
         json.dump(outPackage, packageJsonFile, indent=2, ensure_ascii=False)

   # Locales
   for localesInPath in glob.glob(os.path.join(localesInPath, '*.json')):
      print 'Process locales', localesInPath, 'file...',
      with codecs.open(localesInPath, 'r', 'utf_8') as localesInFile:

         inPackage = json.load(localesInFile, object_pairs_hook=OrderedDict)

         outPackage = copy.deepcopy(inPackage)
         outPackage['deviceConfiguration']['staticConfigurationSchema']['schemas']['all']['content']['profile']['content'] = OrderedDict()
         del outPackage['specificProfilesConfigurations']

         for rorg in profiles:
            for func in profiles[rorg]:
               for type in profiles[rorg][func]:
                  profileName = format(rorg, '02X') + '-' + format(func, '02X') + '-' + format(type, '02X')

                  profileNode = OrderedDict()
                  profileNode['name'] = profileName
                  if profileName in inPackage['specificProfilesConfigurations']:
                     if 'sameAs' in inPackage['specificProfilesConfigurations'][profileName]:
                        aliasProfile = inPackage['specificProfilesConfigurations'][profileName]['sameAs']
                        profileNode['content'] = inPackage['specificProfilesConfigurations'][aliasProfile]['content']
                     else:
                        profileNode['content'] = inPackage['specificProfilesConfigurations'][profileName]['content']
                  else:
                     profileNode['content'] = ''
                  outPackage['deviceConfiguration']['staticConfigurationSchema']['schemas']['all']['content']['profile']['content'][profileName] = profileNode

         localesOutPath = os.path.join(localesPath, os.path.basename(localesInPath))
         util.createParentDir(localesOutPath)
         with codecs.open(localesOutPath, 'w', 'utf_8') as localesOutFile:
            json.dump(outPackage, localesOutFile, indent=2, ensure_ascii=False)
      print 'OK'


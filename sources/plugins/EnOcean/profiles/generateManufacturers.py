#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP code generation script for EnOcean manufacturers

import sys
import os.path
import xml.etree.ElementTree
import string

import cppClass
import cppHelper
import xmlHelper
import util


#-------------------------------------------------------------------------------
# Input parameters
xmlInputFilePath = sys.argv[1]
outputPath = sys.argv[2]




#-------------------------------------------------------------------------------


xmlRootNode = xml.etree.ElementTree.parse(xmlInputFilePath).getroot()
if xmlRootNode.tag != 'manufacturers':
   raise Exception('getAllNodes : Invalid root "' + xmlRootNode.tag + '", "manufacturers" expected')

manufacturersClassName = 'CManufacturers'
manufacturersClass = cppClass.CppClass(manufacturersClassName)
manufacturersClass.addSubType(cppClass.CppEnumType('EManufacturerIds', \
   xmlHelper.getEnumValues(inNode=xmlRootNode, foreachSubNode='manufacturer', enumValueNameTag='name', enumValueTag='id'), cppClass.PUBLIC))
manufacturersClass.addMember(cppClass.CppMember('ManufacturersMap', 'std::map<unsigned int, std::string>', cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, \
   cppHelper.getMapInitCode(xmlHelper.getEnumValues(inNode=xmlRootNode, foreachSubNode='manufacturer', enumValueNameTag='name'))))
manufacturersClass.addMethod(cppClass.CppMethod('toManufacturerId', manufacturersClassName + '::EManufacturerIds', 'unsigned int id', cppClass.PUBLIC, cppClass.STATIC, \
   '   if (ManufacturersMap.find(id) == ManufacturersMap.end())\n' \
   '      throw std::out_of_range("Unknown manufacturer");\n' \
   '   return static_cast<EManufacturerIds>(id);\n'))
manufacturersClass.addMethod(cppClass.CppMethod('name', 'const std::string&', 'unsigned int id', cppClass.PUBLIC, cppClass.STATIC, \
   '   try {\n' \
   '      return ManufacturersMap.at(id);\n' \
   '   } catch(std::out_of_range&) {\n' \
   '      static const std::string UnknownManufacturer("Unknown manufacturer");\n' \
   '      return UnknownManufacturer;\n' \
   '   }'))
manufacturersClass.addMethod(cppClass.CppMethod('name', 'const std::string&', 'EManufacturerIds id', cppClass.PUBLIC, cppClass.STATIC, \
   '   return name(static_cast<unsigned int>(id));'))


# Start
print ('Script run with Python version ' + sys.version)
try:
   os.makedirs(outputPath)
except OSError:
   pass

# Generate Header
with open(os.path.join(outputPath, 'manufacturers.h'), 'w') as cppHeaderFile:

   cppHeaderFile.write('// Generated file, don\'t modify\n')
   cppHeaderFile.write('#pragma once\n')
   cppHeaderFile.write('\n')

   manufacturersClass.generateHeader(cppHeaderFile)

# Generate Source
with open(os.path.join(outputPath, 'manufacturers.cpp'), 'w') as cppSourceFile:

   cppSourceFile.write('// Generated file, don\'t modify\n')
   cppSourceFile.write('#include "stdafx.h"\n')
   cppSourceFile.write('#include "manufacturers.h"\n')
   cppSourceFile.write('\n')

   manufacturersClass.generateSource(cppSourceFile)

util.finish()

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
headerPath = sys.argv[2]
sourcePath = sys.argv[3]


#-------------------------------------------------------------------------------
def ManufacturersMapInitCode():
   initCode = "boost::assign::map_list_of\n"
   for child in xmlHelper.getAllNodes(xmlInputFilePath, "manufacturers/manufacturer"):
      initCode += "   (" + cppHelper.toEnumValueName(child.find("name").text)
      initCode += ", \"" + child.find("name").text.strip() + "\")\n"
   initCode += ";\n"
   return initCode


#-------------------------------------------------------------------------------
def EnumValues():
   enumValues = []
   for child in xmlHelper.getAllNodes(xmlInputFilePath, "manufacturers/manufacturer"):
      enumValues.append([cppHelper.toEnumValueName(child.find("name").text), child.find("id").text])
   return enumValues




#-------------------------------------------------------------------------------

manufacturersClass = cppClass.CppClass("CManufacturers")
manufacturersClass.addSubType(cppClass.CppEnumType("EManufacturerIds", lambda: EnumValues(), cppClass.PUBLIC))
manufacturersClass.addMember(cppClass.CppMember("ManufacturersMap", "std::map<unsigned int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, lambda: ManufacturersMapInitCode()))
manufacturersClass.addMethod(cppClass.CppMethod("name", "const std::string&", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
   "   try {\n" \
   "      return ManufacturersMap.at(id);\n" \
   "   } catch(std::out_of_range&) {\n" \
   "      static const std::string UnknownManufacturer(\"Unknown manufacturer\");\n" \
   "      return UnknownManufacturer;\n" \
   "   }"))


# Generate Header
util.createParentDir(headerPath)
with open(headerPath, 'w') as cppHeaderFile:

   cppHeaderFile.write("// Generated file, don't modify\n")
   cppHeaderFile.write("#pragma once\n")
   cppHeaderFile.write("\n")

   manufacturersClass.generateHeader(cppHeaderFile)

# Generate Source
util.createParentDir(sourcePath)
with open(sourcePath, 'w') as cppSourceFile:

   cppSourceFile.write("// Generated file, don't modify\n")
   cppSourceFile.write("#include \"stdafx.h\"\n")
   cppSourceFile.write("#include \"{}\"\n".format(os.path.basename(headerPath)))
   cppSourceFile.write("\n")

   manufacturersClass.generateSource(cppSourceFile)

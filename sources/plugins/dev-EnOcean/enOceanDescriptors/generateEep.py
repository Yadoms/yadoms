#-------------------------------------------------------------------------------
# CPP code generation script for EnOcean EEP profiles

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
def RorgMapInitCode():
   initCode = "boost::assign::map_list_of\n"
   for child in xmlHelper.getAllNodes(xmlInputFilePath, "eep/profile/rorg"):
      initCode += "   (" + cppHelper.toEnumValueName(child.find("title").text)
      initCode += ", \"" + child.find("title").text.strip() + "\")\n"
   initCode += ";\n"
   return initCode


#-------------------------------------------------------------------------------
def RorgEnumValues():
   enumValues = []
   for child in xmlHelper.getAllNodes(xmlInputFilePath, "eep/profile/rorg"):
      enumValues.append([cppHelper.toEnumValueName(child.find("title").text), child.find("number").text])
   return enumValues




#-------------------------------------------------------------------------------
classes = []

rorgsClass = cppClass.CppClass("CRorgs")
classes.append(rorgsClass)
rorgsClass.addSubType(cppClass.CppEnumType("ERorgIds", lambda: RorgEnumValues(), cppClass.PUBLIC))
rorgsClass.addMember(cppClass.CppMember("RorgMap", "std::map<unsigned int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, lambda: RorgMapInitCode()))
rorgsClass.addMethod(cppClass.CppMethod("name", "const std::string&", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
   "   try {\n" \
   "      return RorgMap.at(id);\n" \
   "   } catch(std::out_of_range&) {\n" \
   "      static const std::string UnknownRorg(\"Unknown rorg\");\n" \
   "      return UnknownRorg;\n" \
   "   }"))


for rorg in xmlHelper.getAllNodes(xmlInputFilePath, "eep/profile/rorg"):
   rorgClass = cppClass.CppClass("C" + rorg.find("telegram").text + "Telegram")
   classes.append(rorgClass)
   rorgClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.STATIC, "   return " + rorg.find("number").text + ";"))
   rorgClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.STATIC, \
   "   static const std::string title(\"" + rorg.find("title").text + "\");\n" \
   "   return title;"))
   rorgClass.addMethod(cppClass.CppMethod("fullname", "const std::string&", "", cppClass.PUBLIC, cppClass.STATIC, \
   "   static const std::string fullname(\"" + rorg.find("fullname").text + "\");\n" \
   "   return fullname;"))


# Generate Header
util.createParentDir(headerPath)
with open(headerPath, 'w') as cppHeaderFile:

   cppHeaderFile.write("// Generated file, don't modify\n")
   cppHeaderFile.write("#pragma once\n")
   cppHeaderFile.write("\n")

   for oneClass in classes:
      oneClass.generateHeader(cppHeaderFile)

# Generate Source
util.createParentDir(sourcePath)
with open(sourcePath, 'w') as cppSourceFile:

   cppSourceFile.write("// Generated file, don't modify\n")
   cppSourceFile.write("#include \"stdafx.h\"\n")
   cppSourceFile.write("#include \"{}\"\n".format(os.path.basename(headerPath)))
   cppSourceFile.write("\n")

   for oneClass in classes:
      oneClass.generateSource(cppSourceFile)

import sys
import os.path
import xml.etree.ElementTree
import string

import cppClass


#-------------------------------------------------------------------------------
# Input parameters
xmlInputFilePath = sys.argv[1]

headerPath = sys.argv[2]
sourcePath = sys.argv[3]



#-------------------------------------------------------------------------------
def getXmlEnumNodes(xmlInputFile, xmlElementPath):
   xmlRootNode = xml.etree.ElementTree.parse(xmlInputFile).getroot()

   # Some checks on xmlElementPath input
   xmlElementPathArray = string.split(xmlElementPath, ".")
   if xmlElementPathArray.count < 2:
      raise Exception("getXmlEnumNodes : Invalid xmlElementPath \"" + xmlElementPath + "\" : must at least be in the form \"root.element\"")

   if xmlElementPathArray[0] != xmlRootNode.tag:
      raise Exception("getXmlEnumNodes : Invalid xmlElementPath \"" + xmlElementPath + "\" : invalid root name")

   # Remove root name
   xmlElementPathArray.pop(0)

   return xmlRootNode.findall(string.join(xmlElementPathArray, "."))


#-------------------------------------------------------------------------------
def toEnumValueName(label):
   return "k" + label.strip().replace(" ", "_").replace("+", "plus").replace("-", "_").replace(".", "_").replace(",", "_")


#-------------------------------------------------------------------------------
def ManufacturersMapInitCode():
   initCode = "boost::assign::map_list_of\n"
   for child in getXmlEnumNodes(xmlInputFilePath, "manufacturers.manufacturer"):
      initCode += "   (" + toEnumValueName(child.find("name").text)
      initCode += ", \"" + child.find("name").text.strip() + "\")\n"
   initCode += ";\n"
   return initCode


#-------------------------------------------------------------------------------
def EnumValues():
   enumValues = []
   for child in getXmlEnumNodes(xmlInputFilePath, "manufacturers.manufacturer"):
      enumValues.append([toEnumValueName(child.find("name").text), child.find("id").text])
   return enumValues



#-------------------------------------------------------------------------------

manufacturersClass = cppClass.CppClass("CManufacturers")
manufacturersClass.addSubType(cppClass.CppEnumType("EManufacturerIds", lambda: EnumValues(), cppClass.PUBLIC))
manufacturersClass.addMember(cppClass.CppMember("ManufacturersMap", "std::map<int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, lambda: ManufacturersMapInitCode()))
manufacturersClass.addMethod(cppClass.CppMethod("name", "const std::string&", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, "   return ManufacturersMap.at(id);"))

# Generate Header
with open(headerPath, 'w') as cppHeaderFile:

   cppHeaderFile.write("// Generated file, don't modify\n")
   cppHeaderFile.write("#pragma once\n")
   cppHeaderFile.write("\n")

   manufacturersClass.generateHeader(cppHeaderFile)

# Generate Source
with open(sourcePath, 'w') as cppSourceFile:

   cppSourceFile.write("// Generated file, don't modify\n")
   cppSourceFile.write("#include \"stdafx.h\"\n")
   cppSourceFile.write("#include \"{}\"\n".format(os.path.basename(headerPath)))
   cppSourceFile.write("\n")

   manufacturersClass.generateSource(cppSourceFile)


import sys
import os.path
import xml.etree.ElementTree
import string


xmlInputFilePath = sys.argv[1]

headerPath = sys.argv[2]
sourcePath = sys.argv[3]



#-------------------------------------------------------------------------------
def getXmlEnumNodes(xmlInputFile, xmlElementPath):
   xmlRootNode = xml.etree.ElementTree.parse(xmlInputFile).getroot()

   # Some checks on xmlElementPath input
   xmlElementPathArray = string.split(xmlElementPath, ".")
   if xmlElementPathArray.count < 2:
      raise Exception("delcareEnum : Invalid xmlElementPath \"" + xmlElementPath + "\" : must at least be in the form \"root.element\"")

   if xmlElementPathArray[0] != xmlRootNode.tag:
      raise Exception("delcareEnum : Invalid xmlElementPath \"" + xmlElementPath + "\" : invalid root name")

   # Remove root name
   xmlElementPathArray.pop(0)

   return xmlRootNode.findall(string.join(xmlElementPathArray, "."))


#-------------------------------------------------------------------------------
def toEnumValueName(label):
   return "k" + label.strip().replace(" ", "_").replace("+", "plus").replace("-", "_").replace(".", "_").replace(",", "_")


#-------------------------------------------------------------------------------
def delcareEnum(cppOutFile, cppEnumName, xmlInputFile, xmlElementPath, xmlNameKey, xmlValueKey = None):
   cppOutFile.write("enum " + cppEnumName + " {\n")
   for child in getXmlEnumNodes(xmlInputFile, xmlElementPath):
      cppOutFile.write("   " + toEnumValueName(child.find(xmlNameKey).text))
      if xmlValueKey:
         cppOutFile.write(" = " + child.find(xmlValueKey).text)
      cppOutFile.write(",\n")
   cppOutFile.write("};\n")


#-------------------------------------------------------------------------------
def declareMap(cppOutFile, cppMapName, cppKeyType, cppValueType, isStatic = False, isConst = False):
   if isStatic:
      cppOutFile.write("static ");
   if isConst:
      cppOutFile.write("const ");
   cppOutFile.write("std::map<" + cppKeyType + ", " + cppValueType + "> " + cppMapName + ";\n")


#-------------------------------------------------------------------------------
def implementMap(cppOutFile, cppMapName, cppKeyType, cppValueType, xmlInputFile, xmlElementPath, xmlNameKey):
   cppOutFile.write("const std::map<" + cppKeyType + ", " + cppValueType + "> " + cppMapName + " = boost::assign::map_list_of\n")
   for child in getXmlEnumNodes(xmlInputFile, xmlElementPath):
      cppOutFile.write("   (" + toEnumValueName(child.find(xmlNameKey).text))
      cppOutFile.write(", \"" + child.find(xmlNameKey).text.strip() + "\")\n")
   cppOutFile.write(";\n")


#-------------------------------------------------------------------------------
def declareFunction(cppOutFile, cppReturnType, cppFunctionName, cppArgs, isVirtual = False, isStatic = False, isConst = False, isPureVirtual = False):
   if isStatic:
      cppOutFile.write("static ");
   if isVirtual | isPureVirtual:
      cppOutFile.write("virtual ");
   cppOutFile.write(cppReturnType + " " + cppFunctionName + "(" + cppArgs + ")")
   if isConst:
      cppOutFile.write(" const");
   if isPureVirtual:
      cppOutFile.write(" = 0");
   cppOutFile.write(";\n");


#-------------------------------------------------------------------------------
def implementFunction(cppOutFile, cppReturnType, cppNamespaceName, cppFunctionName, cppArgs, isStatic = False, isConst = False):
   if isStatic:
      cppOutFile.write("static ");
   cppOutFile.write(cppReturnType + " ");
   if cppNamespaceName:
      cppOutFile.write(cppNamespaceName + "::");
   cppOutFile.write(cppFunctionName + "(" + cppArgs + ")");
   if isConst:
      cppOutFile.write(" const");
   cppOutFile.write(" {\n");


#-------------------------------------------------------------------------------
def generateHeader() :
   with open(headerPath, 'w') as f :
      f.write("// Generated file, don't modify\n")
      f.write("#pragma once\n")
      f.write("\n")
      f.write("class CManufacturers {\n")
      f.write("public:\n")
      delcareEnum(f, "EManufacturerIds", xmlInputFilePath, "manufacturers.manufacturer", "name", "id")
      f.write("\n")
      f.write("CManufacturers();\n")
      f.write("virtual ~CManufacturers();\n")
      f.write("\n")
      declareFunction(f, "const std::string&", "name", "unsigned int id", False, True, False);
      f.write("private:\n")
      declareMap(f, "ManufacturersMap", "int", "std::string", True, True);
      f.write("};\n")

#-------------------------------------------------------------------------------
def generateSource() :
   with open(sourcePath, 'w') as f :
      f.write("// Generated file, don't modify\n")
      f.write("#include \"stdafx.h\"\n")
      f.write("#include \"{}\"\n".format(os.path.basename(headerPath)))
      f.write("\n")

      implementMap(f, "CManufacturers::ManufacturersMap", "int", "std::string", xmlInputFilePath, "manufacturers.manufacturer", "name")

      f.write("\n")
      f.write("CManufacturers::CManufacturers(){}\n")
      f.write("\n")
      f.write("CManufacturers::~CManufacturers(){}\n")
      f.write("\n")
      implementFunction(f, "const std::string&", "CManufacturers", "name", "unsigned int id");
      f.write("   return ManufacturersMap.at(id);\n")
      f.write("}\n")

#-------------------------------------------------------------------------------
generateHeader()
generateSource()
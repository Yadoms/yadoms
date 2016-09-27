
import sys
import os.path
import xml.etree.ElementTree


xmlInputFilePath = sys.argv[1]

headerPath = sys.argv[2]
sourcePath = sys.argv[3]


#-------------------------------------------------------------------------------
def delcareEnum(cppOutFile, cppEnumName, xmlInputFile, xmlElementName, xmlNameKey, xmlValueKey = None):
   xmlRootNode = xml.etree.ElementTree.parse(xmlInputFile).getroot()
   cppOutFile.write("enum " + cppEnumName + " {\n")
   cppOutFile.write("   kMulti_user_Manufacturer_ID = 0x7FF,\n")
   print xmlRootNode
   for child in xmlRootNode:
      print child.attrib
      #cppOutFile.write(child.get(xmlNameKey))
   cppOutFile.write("};\n")

#-------------------------------------------------------------------------------
def generateHeader() :
   with open(headerPath, 'w') as f :
      f.write("// Generated file, don't modify\n")
      f.write("#pragma once\n")
      f.write("\n")
      f.write("class CManufacturers {\n")
      f.write("public:\n")
      delcareEnum(f, "EManufacturerIds", xmlInputFilePath, "manufacturer", "name", "id")
      f.write("\n")
      f.write("   CManufacturers();\n")
      f.write("   virtual ~CManufacturers();\n")
      f.write("\n")
      f.write("   static const std::string& name(unsigned int id);\n")
      f.write("};\n")

#-------------------------------------------------------------------------------
def generateSource() :
   with open(sourcePath, 'w') as f :
      f.write("// Generated file, don't modify\n")
      f.write("#include \"stdafx.h\"\n")
      f.write("#include \"{}\"\n".format(os.path.basename(headerPath)))
      f.write("\n")
      f.write("const std::map<int, std::string> ManufacturersMap = boost::assign::map_list_of(CManufacturers::kMulti_user_Manufacturer_ID, \"Multi user Manufacturer ID\");\n")
      f.write("\n")
      f.write("CManufacturers::CManufacturers(){}\n")
      f.write("\n")
      f.write("CManufacturers::~CManufacturers(){}\n")
      f.write("\n")
      f.write("const std::string& CManufacturers::name(unsigned int id) {\n")
      f.write("   return ManufacturersMap.at(id);\n")
      f.write("}\n")

#-------------------------------------------------------------------------------
generateHeader()
generateSource()
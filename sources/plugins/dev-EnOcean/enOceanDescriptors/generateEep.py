#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP code generation script for EnOcean EEP profiles

import sys
import os.path
import xml.etree.ElementTree
import string
import codecs

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
classes = []


xmlRootNode = xml.etree.ElementTree.parse(xmlInputFilePath).getroot()
if xmlRootNode.tag != "eep":
   raise Exception("getAllNodes : Invalid root \"" + xmlRootNode.tag + "\", \"eep\" expected")
xmlProfileNode = xmlRootNode.find("profile")



# IType : Type interface
itypeClass = cppClass.CppClass("IType", createDefaultCtor=False)
classes.append(itypeClass)
itypeClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
itypeClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
itypeClass.addMethod(cppClass.CppMethod("states", "boost::shared_ptr<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > >", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))


# IFunc : Func interface
ifuncClass = cppClass.CppClass("IFunc", createDefaultCtor=False)
classes.append(ifuncClass)
ifuncClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
ifuncClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
ifuncClass.addMethod(cppClass.CppMethod("createType", "boost::shared_ptr<IType>", "unsigned int typeId, const boost::dynamic_bitset<>& data", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))


# IRorg : Rorg interface
irorgClass = cppClass.CppClass("IRorg", createDefaultCtor=False)
classes.append(irorgClass)
irorgClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
irorgClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
irorgClass.addMethod(cppClass.CppMethod("fullname", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
irorgClass.addMethod(cppClass.CppMethod("dump", "std::string", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
irorgClass.addMethod(cppClass.CppMethod("isTeachIn", "bool", "", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))
irorgClass.addMethod(cppClass.CppMethod("createFunc", "boost::shared_ptr<IFunc>", "unsigned int funcId", cppClass.PUBLIC, cppClass.CONST | cppClass.PURE_VIRTUAL))


# CRorgs : Main Rorgs class, listing Rorg messages
rorgsClass = cppClass.CppClass("CRorgs")
classes.append(rorgsClass)
rorgsClass.addSubType(cppClass.CppEnumType("ERorgIds", \
   xmlHelper.getEnumValues(inNode=xmlProfileNode, foreachSubNode="rorg", enumValueNameTag="title", enumValueTag="number"), cppClass.PUBLIC))
rorgsClass.addMember(cppClass.CppMember("RorgMap", "std::map<unsigned int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, \
   cppHelper.getMapInitCode(xmlHelper.getEnumValues(inNode=xmlProfileNode, foreachSubNode="rorg", enumValueNameTag="title"))))
rorgsClass.addMethod(cppClass.CppMethod("toRorgId", "CRorgs::ERorgIds", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
   "   if (RorgMap.find(id) == RorgMap.end())\n" \
   "      throw std::out_of_range(\"Unknown rorg\");\n" \
   "   return static_cast<ERorgIds>(id);\n"))
rorgsClass.addMethod(cppClass.CppMethod("name", "const std::string&", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
   "   try {\n" \
   "      return RorgMap.at(id);\n" \
   "   } catch(std::out_of_range&) {\n" \
   "      static const std::string UnknownRorg(\"Unknown rorg\");\n" \
   "      return UnknownRorg;\n" \
   "   }"))

def createRorgCode(xmlProfileNode):
   code = "   switch(id)\n"
   code += "   {\n"
   for child in xmlProfileNode.findall("rorg"):
      enumValue = cppHelper.toEnumValueName(child.find("title").text)
      className = "C" + child.find("telegram").text + "Telegram"
      code += "   case " + enumValue + ": return boost::make_shared<" + className + ">(erp1Data);\n"
   code += "   default : throw std::out_of_range(\"Invalid EOrgId\");\n"
   code += "   }\n"
   return code
rorgsClass.addMethod(cppClass.CppMethod("createRorg", "boost::shared_ptr<IRorg>", "ERorgIds id, const boost::dynamic_bitset<>& erp1Data", cppClass.PUBLIC, cppClass.STATIC, createRorgCode(xmlProfileNode)))



# Create each Rorg telegram class
for xmlRorgNode in xmlProfileNode.findall("rorg"):

   # Rorg telegram classes
   rorgClassName = "C" + xmlRorgNode.find("telegram").text + "Telegram"
   rorgClass = cppClass.CppClass(rorgClassName, createDefaultCtor=False)
   rorgClass.inheritFrom("IRorg", cppClass.PUBLIC)
   classes.append(rorgClass)
   rorgClass.addSubType(cppClass.CppEnumType("EFuncIds", xmlHelper.getEnumValues(inNode=xmlRorgNode, foreachSubNode="func", enumValueNameTag="title", enumValueTag="number"), cppClass.PUBLIC))
   rorgClass.addMember(cppClass.CppMember("m_erp1Data", "boost::dynamic_bitset<>&", cppClass.PRIVATE, cppClass.CONST))
   rorgClass.addConstructor(cppClass.CppClassConstructor(args="const boost::dynamic_bitset<>& erp1Data", init="m_erp1Data(erp1Data)"))
   rorgClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, "   return " + xmlRorgNode.find("number").text + ";"))
   rorgClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, \
      "   static const std::string title(\"" + xmlRorgNode.find("title").text + "\");\n" \
      "   return title;"))
   rorgClass.addMethod(cppClass.CppMethod("fullname", "const std::string&", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, \
      "   static const std::string fullname(\"" + xmlRorgNode.find("fullname").text + "\");\n" \
      "   return fullname;"))
   rorgClass.addMethod(cppClass.CppMethod("dump", "std::string", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, \
      "   std::stringstream ss;\n" \
      "   ss << std::setfill('0') << std::setw(2) << std::hex;\n" \
      "   for (size_t bit = 0; bit < m_erp1Data.count(); ++bit)\n" \
      "      ss << m_erp1Data[bit] << \" \";\n" \
      "   return ss.str();")) # TODO dumper en bytes

   def createIsTeachInCode(xmlRorgNode):
      if xmlRorgNode.find("teachin") is None:
         return "   return false;"
      for teachinCase in xmlRorgNode.findall("teachin/type/case"):
         lrnBitDatafieldNode = teachinCase.find("./datafield[data='LRN Bit']")
         if lrnBitDatafieldNode is not None:
            offset = lrnBitDatafieldNode.find("bitoffs").text
            if lrnBitDatafieldNode.find("bitsize").text != "1":
               raise Exception(xmlRorgNode.find("telegram").text + " telegram : teachin LRN Bit wrong size, expected 1")
            teachInValue = xmlHelper.findInDatafield(datafieldXmlNode=lrnBitDatafieldNode, select="value", where="description", equals="Teach-in telegram")
            return "   return m_erp1Data[" + offset + "] == " + teachInValue + ";\n"
         else:
            return "   return false;"
      return "   return false;"
   rorgClass.addMethod(cppClass.CppMethod("isTeachIn", "bool", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, createIsTeachInCode(xmlRorgNode)))

   def createFuncCode(xmlRorgNode):
      code = "   switch(static_cast<EFuncIds>(funcId))\n"
      code += "   {\n"
      for xmlFuncNode in xmlRorgNode.findall("func"):
         enumValue = cppHelper.toEnumValueName(xmlFuncNode.find("title").text)
         className = "C" + xmlRorgNode.find("telegram").text + "_" + cppHelper.toCppName(xmlFuncNode.find("title").text)
         code += "   case " + enumValue + ": return boost::make_shared<" + className + ">();\n"
      code += "   default : throw std::out_of_range(\"Invalid EFuncIds\");\n"
      code += "   }\n"
      return code
   rorgClass.addMethod(cppClass.CppMethod("createFunc", "boost::shared_ptr<IFunc>", "unsigned int funcId", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, createFuncCode(xmlRorgNode)))



   rorgClass.addMember(cppClass.CppMember("FuncMap", "std::map<unsigned int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, \
      cppHelper.getMapInitCode(xmlHelper.getEnumValues(inNode=xmlRorgNode, foreachSubNode="func", enumValueNameTag="title"))))
   rorgClass.addMethod(cppClass.CppMethod("toFuncId", rorgClassName + "::EFuncIds", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
      "   if (FuncMap.find(id) == FuncMap.end())\n" \
      "      throw std::out_of_range(\"Unknown func\");\n" \
      "   return static_cast<EFuncIds>(id);\n"))
   rorgClass.addMethod(cppClass.CppMethod("toFuncName", "const std::string&", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
      "   try {\n" \
      "      return FuncMap.at(id);\n" \
      "   } catch(std::out_of_range&) {\n" \
      "      static const std::string UnknownFunc(\"Unknown func\");\n" \
      "      return UnknownFunc;\n" \
      "   }"))


   # Func classes
   for xmlFuncNode in xmlRorgNode.findall("func"):
      funcClass = cppClass.CppClass("C" + xmlRorgNode.find("telegram").text + "_" + cppHelper.toCppName(xmlFuncNode.find("title").text))
      funcClass.inheritFrom("IFunc", cppClass.PUBLIC)
      classes.append(funcClass)
      funcClass.addSubType(cppClass.CppEnumType("ETypeIds", xmlHelper.getEnumValues(inNode=xmlFuncNode, foreachSubNode="type", enumValueNameTag="number", enumValueTag="number"), cppClass.PUBLIC))
      funcClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, "   return " + xmlFuncNode.find("number").text + ";"))
      funcClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, \
         "   static const std::string title(\"" + xmlFuncNode.find("title").text + "\");\n" \
         "   return title;"))

      def createTypeCode(xmlRorgNode, xmlFuncNode):
         code = "   switch(static_cast<ETypeIds>(typeId))\n"
         code += "   {\n"
         for xmlTypeNode in xmlFuncNode.findall("type"):
            enumValue = cppHelper.toEnumValueName(xmlTypeNode.find("number").text)
            className = "C" + xmlRorgNode.find("telegram").text + "_" + cppHelper.toCppName(xmlFuncNode.find("title").text) + "_" + xmlTypeNode.find("number").text
            code += "   case " + enumValue + ": return boost::make_shared<" + className + ">(data);\n"
         code += "   default : throw std::out_of_range(\"Invalid EFuncIds\");\n"
         code += "   }\n"
         return code
      funcClass.addMethod(cppClass.CppMethod("createType", "boost::shared_ptr<IType>", "unsigned int typeId, const boost::dynamic_bitset<>& data", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, createTypeCode(xmlRorgNode, xmlFuncNode)))



      # Type classes
      for xmlTypeNode in xmlFuncNode.findall("type"):
         typeClassName = "C" + xmlRorgNode.find("telegram").text + "_" + cppHelper.toCppName(xmlFuncNode.find("title").text) + "_" + xmlTypeNode.find("number").text
         typeClass = cppClass.CppClass(typeClassName, createDefaultCtor=False)
         typeClass.inheritFrom("IType", cppClass.PUBLIC)
         classes.append(typeClass)
         typeClass.addMember(cppClass.CppMember("m_data", "boost::dynamic_bitset<>&", cppClass.PRIVATE, cppClass.CONST))
         typeClass.addConstructor(cppClass.CppClassConstructor(args="const boost::dynamic_bitset<>& data", init="m_data(data)"))
         typeClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, "   return " + xmlTypeNode.find("number").text + ";"))
         typeClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, \
            "   static const std::string title(\"" + xmlTypeNode.find("title").text + "\");\n" \
            "   return title;"))


         def statesCodeForDoubleValue(xmlDataFieldNode, keywordHistorizerClass, keywordName):
            offset = xmlDataFieldNode.find("bitoffs").text
            size = xmlDataFieldNode.find("bitsize").text
            code = "   {\n"
            code += "      unsigned int rawValue = bitset_extract(m_data, " + offset + ", " + size + ");\n"
            rangeMin = xmlDataFieldNode.find("range/min").text
            rangeMax = xmlDataFieldNode.find("range/max").text
            scaleMin = xmlDataFieldNode.find("scale/min").text
            scaleMax = xmlDataFieldNode.find("scale/max").text
            code += "      double value = scaleToDouble(rawValue, " + rangeMin + ", " + rangeMax + ", " + scaleMin + ", " + scaleMax + ");\n"
            code += "\n"
            code += "      auto keyword(boost::make_shared<" + keywordHistorizerClass + ">(\"" + keywordName + "\"));\n"
            code += "      keyword->set(value);\n"
            code += "      historizers->push_back(keyword);\n"
            code += "   }\n"
            return code

         def statesCodeForTemperature(xmlDataFieldNode):
            if xmlDataFieldNode.find("unit") is not None and xmlDataFieldNode.find("unit").text != u"°C":
               print "Unsupported unit \"" + xmlDataFieldNode.find("unit").text + "\" for temperature, corresponding data will be ignored"
               return ""
            return statesCodeForDoubleValue(xmlDataFieldNode, "yApi::historization::CTemperature", "temperature")

         def statesCodeForHumidity(xmlDataFieldNode):
            if xmlDataFieldNode.find("unit") is not None and xmlDataFieldNode.find("unit").text != u"%":
               print "Unsupported unit \"" + xmlDataFieldNode.find("unit").text + "\" for humidity, corresponding data will be ignored"
               return ""
            return statesCodeForDoubleValue(xmlDataFieldNode, "yApi::historization::CHumidity", "humidity")

         def statesCode(xmlTypeNode):
            code = "   auto historizers(boost::make_shared<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > >());\n"
            if len(xmlTypeNode.findall("case")) != 1:
               #TODO erreur unicode                  print "Warning : func/type : Unsupported number of \"case\" tags (expected 1) for \"" + xmlTypeNode.find("title").text + "\" node. This profile will be ignored."#TODO
               code += "   return historizers;\n"
               return code
            xmlCaseNode = xmlTypeNode.find("case")
            for xmlDataFieldNode in xmlHelper.findUsefulDataFieldNodes(inXmlNode=xmlCaseNode):
               dataText = xmlDataFieldNode.find("data").text
               if dataText == "Temperature":
                  code += statesCodeForTemperature(xmlDataFieldNode) + "\n"
               elif dataText == "Humidity":
                  code += statesCodeForHumidity(xmlDataFieldNode) + "\n"
               else:
                  #TODO erreur unicode                  print ("Warning : func/type : Unsupported data type \"" + dataText + "\" for \"" + xmlTypeNode.find("title").text + "\" node. This data will be ignored.")#TODO
                  code += "return historizers;\n"
                  return code
            code += "   return historizers;\n"
            return code

         typeClass.addMethod(cppClass.CppMethod("states", "boost::shared_ptr<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > >", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, statesCode(xmlTypeNode)))




# Generate Header
util.createParentDir(headerPath)
with codecs.open(headerPath, 'w', 'utf_8') as cppHeaderFile:

   cppHeaderFile.write("// Generated file, don't modify\n")
   cppHeaderFile.write("#pragma once\n")
   cppHeaderFile.write("#include \"bitsetHelpers.hpp\"\n")
   cppHeaderFile.write("#include \"commonHelpers.hpp\"\n")
   cppHeaderFile.write("#include <plugin_cpp_api/IPlugin.h>\n")
   cppHeaderFile.write("\n")
   cppHeaderFile.write("namespace yApi = shared::plugin::yPluginApi;\n")
   cppHeaderFile.write("\n")

   for oneClass in classes:
      oneClass.generateHeader(cppHeaderFile)

# Generate Source
util.createParentDir(sourcePath)
with codecs.open(sourcePath, 'w', 'utf_8') as cppSourceFile:

   cppSourceFile.write("// Generated file, don't modify\n")
   cppSourceFile.write("#include \"../stdafx.h\"\n")
   cppSourceFile.write("#include \"" + os.path.basename(headerPath) + "\"\n")
   cppSourceFile.write("\n")

   for oneClass in classes:
      oneClass.generateSource(cppSourceFile)

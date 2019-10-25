#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP code generation script for EnOcean EEP profiles
#
# Note : this script create code only for read-only devices. Please consider to
# write "hard-coded" classes to support read-write devices
#

import sys
import os.path
import xml.etree.ElementTree
import string
import codecs
import re
import json
import copy

import cppClass
import cppHelper
import xmlHelper
import profileHelper
import util



#-------------------------------------------------------------------------------
# Input parameters
xmlInputFilePath = sys.argv[1]
outputPath = sys.argv[2]
packageJsonInPath = sys.argv[3]
packageJsonPath = sys.argv[4]
localesPath = sys.argv[5]
localesInPath = sys.argv[6]

profilePath = os.path.dirname(xmlInputFilePath)




#-------------------------------------------------------------------------------
hardCodedProfiles = cppHelper.HardCodedProfiles(profilePath)
# Supported profiles are at least composed of hard coded profiles
supportedProfiles = copy.deepcopy(hardCodedProfiles.getProfilesHardCoded())


#-------------------------------------------------------------------------------
xmlRootNode = xml.etree.ElementTree.parse(xmlInputFilePath).getroot()
if xmlRootNode.tag != "eep":
   raise Exception("getAllNodes : Invalid root \"" + xmlRootNode.tag + "\", \"eep\" expected")
xmlProfileNode = xmlRootNode.find("profile")

util.info("Hard-coded profiles are : " + str(hardCodedProfiles.getProfilesHardCoded()))



# CRorgs : Main Rorgs class, listing Rorg messages
rorgsClass = cppClass.CppClass("CRorgs")
rorgsItems = xmlHelper.getEnumValues(inNode=xmlProfileNode, foreachSubNode="rorg", enumValueNameTag="title", enumValueTag="number")
# Add 0xD4 (Universal Teach-in message) to RORG list as it doesn't not appear in eepXX.xml file
rorgsItems.append(['UTE Telegram ', '0xD4'])
rorgsClass.addSubType(cppClass.CppEnumType("ERorgIds", rorgsItems, cppClass.PUBLIC))
rorgsClass.addMember(cppClass.CppMember("RorgMap", "std::map<unsigned int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, \
   cppHelper.getMapInitCode(rorgsItems)))
rorgsClass.addMethod(cppClass.CppMethod("toRorgId", "CRorgs::ERorgIds", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
   "   if (RorgMap.find(id) == RorgMap.end())\n" \
   "      throw std::out_of_range(\"Unknown rorg \" + CProfileHelper::byteToHexString(id));\n" \
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
      code += "   case " + enumValue + ": return boost::make_shared<" + className + ">();\n"
   code += "   default : throw std::out_of_range(\"Invalid EOrgId\");\n"
   code += "   }\n"
   return code
rorgsClass.addMethod(cppClass.CppMethod("createRorg", "boost::shared_ptr<IRorg>", "ERorgIds id", cppClass.PUBLIC, cppClass.STATIC, createRorgCode(xmlProfileNode)))
rorgsClass.addMethod(cppClass.CppMethod("createRorg", "boost::shared_ptr<IRorg>", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
   "   return createRorg(static_cast<ERorgIds>(id));"))



#------------------------------------------------------------------------
# Create each Rorg telegram class
for xmlRorgNode in xmlProfileNode.findall("rorg"):
   rorgClassName = "C" + xmlRorgNode.find("telegram").text + "Telegram"
   rorgClass = cppClass.CppClass(rorgClassName)
   rorgClass.inheritFrom("IRorg", cppClass.PUBLIC)
   rorgsClass.addDependency(rorgClass)
   rorgClass.addSubType(cppClass.CppEnumType("EFuncIds", xmlHelper.getEnumValues(inNode=xmlRorgNode, foreachSubNode="func", enumValueNameTag="title", enumValueTag="number"), cppClass.PUBLIC))
   rorgClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, "   return " + xmlRorgNode.find("number").text + ";"))
   rorgClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, \
      "   static const std::string title(\"" + xmlRorgNode.find("title").text + "\");\n" \
      "   return title;"))
   rorgClass.addMethod(cppClass.CppMethod("fullname", "const std::string&", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, \
      "   static const std::string fullname(\"" + xmlRorgNode.find("fullname").text + "\");\n" \
      "   return fullname;"))

   def isTeachInCode(xmlRorgNode):
      if xmlRorgNode.find("teachin") is None:
         return "   return false;"
      for teachinCase in xmlRorgNode.findall("teachin/type/case"):
         lrnBitDatafieldNode = teachinCase.find("./datafield[data='LRN Bit']")
         if lrnBitDatafieldNode is None:
            return "   return false;"
         offset = lrnBitDatafieldNode.find("bitoffs").text
         if lrnBitDatafieldNode.find("bitsize").text != "1":
            util.error(xmlRorgNode.find("telegram").text + " telegram : teachin LRN Bit wrong size, expected 1")
         teachInValue = xmlHelper.findInDatafield(datafieldXmlNode=lrnBitDatafieldNode, select="value", where="description", equals="Teach-in telegram")
         return "   return erp1Data[" + offset + "] == " + teachInValue + ";\n"
      return "   return false;"
   rorgClass.addMethod(cppClass.CppMethod("isTeachIn", "bool", "const boost::dynamic_bitset<>& erp1Data", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, isTeachInCode(xmlRorgNode)))

   def isEepProvidedCode(xmlRorgNode):
      if xmlRorgNode.find("teachin") is None:
         return "   return false;"
      teachinType = xmlRorgNode.find("teachin/type")
      if teachinType is None:
         return "   return false;"
      variation2CaseNode = teachinType.find("./case[title='Variation 2']")
      if variation2CaseNode is None:
         return "   return false;"
      lrnTypeDatafieldNode = variation2CaseNode.find("./datafield[data='LRN Type']")
      if lrnTypeDatafieldNode is None:
         util.error(xmlRorgNode.find("telegram").text + " teachin variation 2, \"LRN Type\" bit description not found")
      offset = lrnTypeDatafieldNode.find("bitoffs").text
      if lrnTypeDatafieldNode.find("bitsize").text != "1":
         util.error(xmlRorgNode.find("telegram").text + " telegram : teachin LRN Type wrong size, expected 1")
      eepProvidedValue = xmlHelper.findInDatafield(datafieldXmlNode=lrnTypeDatafieldNode, select="value", where="description", equals="telegram with EEP number and Manufacturer ID")
      return "   return erp1Data[" + offset + "] == " + eepProvidedValue + ";\n"
   rorgClass.addMethod(cppClass.CppMethod("isEepProvided", "bool", "const boost::dynamic_bitset<>& erp1Data", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, isEepProvidedCode(xmlRorgNode)))

   def createFuncCode(xmlRorgNode):
      code = "   switch(static_cast<EFuncIds>(funcId))\n"
      code += "   {\n"
      for xmlFuncNode in xmlRorgNode.findall("func"):
         enumValue = cppHelper.toEnumValueName(xmlFuncNode.find("title").text)
         className = "C" + xmlRorgNode.find("telegram").text + "_" + cppHelper.toCppName(xmlFuncNode.find("number").text)
         code += "   case " + enumValue + ": return boost::make_shared<" + className + ">();\n"
      code += "   default : throw std::out_of_range(\"Invalid EFuncIds\");\n"
      code += "   }\n"
      return code
   rorgClass.addMethod(cppClass.CppMethod("createFunc", "boost::shared_ptr<IFunc>", "unsigned int funcId", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, createFuncCode(xmlRorgNode)))

   rorgClass.addMember(cppClass.CppMember("FuncMap", "std::map<unsigned int, std::string>", cppClass.PRIVATE, cppClass.STATIC | cppClass.CONST, \
      cppHelper.getMapInitCode(xmlHelper.getEnumValues(inNode=xmlRorgNode, foreachSubNode="func", enumValueNameTag="title"))))
   rorgClass.addMethod(cppClass.CppMethod("toFuncId", rorgClassName + "::EFuncIds", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
      "   if (FuncMap.find(id) == FuncMap.end())\n" \
      "      throw std::out_of_range(\"Unknown func \" + CProfileHelper::byteToHexString(id));\n" \
      "   return static_cast<EFuncIds>(id);\n"))
   rorgClass.addMethod(cppClass.CppMethod("toFuncName", "const std::string&", "unsigned int id", cppClass.PUBLIC, cppClass.STATIC, \
      "   try {\n" \
      "      return FuncMap.at(id);\n" \
      "   } catch(std::out_of_range&) {\n" \
      "      static const std::string UnknownFunc(\"Unknown func\");\n" \
      "      return UnknownFunc;\n" \
      "   }"))


   #------------------------------------------------------------------------
   # Func cppTypes
   for xmlFuncNode in xmlRorgNode.findall("func"):
      funcClass = cppClass.CppClass("C" + xmlRorgNode.find("telegram").text + "_" + cppHelper.toCppName(xmlFuncNode.find("number").text))
      funcClass.addComment(cppHelper.toCppName(xmlFuncNode.find("title").text))
      funcClass.inheritFrom("IFunc", cppClass.PUBLIC)
      rorgClass.addDependency(funcClass)
      funcClass.addSubType(cppClass.CppEnumType("ETypeIds", xmlHelper.getEnumValues(inNode=xmlFuncNode, foreachSubNode="type", enumValueNameTag="number", enumValueTag="number"), cppClass.PUBLIC))
      funcClass.addMethod(cppClass.CppMethod("id", "unsigned int", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, "   return " + xmlFuncNode.find("number").text + ";"))
      funcClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, \
         "   static const std::string title(\"" + xmlFuncNode.find("title").text + "\");\n" \
         "   return title;"))

      #------------------------------------------------------------------------
      # Type cppTypes
      for xmlTypeNode in xmlFuncNode.findall("type"):
         profileName = profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode)
         typeClassName = cppHelper.toCppName("CProfile_" + profileName)
         if hardCodedProfiles.isProfileHardCoded(profileName):
            # Profile is hard-coded, nothing to do
            continue
         typeClass = cppClass.CppClass(typeClassName)
         typeClass.inheritFrom("IType", cppClass.PUBLIC)
         typeClass.addConstructor(cppClass.CppClassConstructor("const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api"))
         funcClass.addDependency(typeClass)


         def isLinearValue(xmlDataFieldNode):
            return True if xmlDataFieldNode.find("range/min") is not None \
               and xmlDataFieldNode.find("range/max") is not None \
               and xmlDataFieldNode.find("scale/min") is not None \
               and xmlDataFieldNode.find("scale/max") is not None else False

         def isBoolValue(xmlDataFieldNode):
            return True if xmlDataFieldNode.find("enum") is not None \
               and len(xmlDataFieldNode.findall("enum/item")) == 2 \
               and int(xmlDataFieldNode.find("bitsize").text) == 1 \
               and xmlDataFieldNode.find("shortcut").text != "LRN" else False

         def isEnumValue(xmlDataFieldNode):
            if xmlDataFieldNode.find("enum") is None \
               or len(xmlDataFieldNode.findall("enum/item")) <= 2 \
               or len(xmlDataFieldNode.findall("enum/item")) != len(xmlDataFieldNode.findall("enum/item/value")):
               return False
            # Check if value is convertible to int
            try:
               for value in xmlDataFieldNode.findall("enum/item/value"):
                  int(value.text)
            except:
               return False
            return True


         def supportedUnit(xmlDataFieldNode, expectedUnit):
            if expectedUnit is not None and xmlDataFieldNode.find("unit") is not None and xmlDataFieldNode.find("unit").text == expectedUnit:
               return True
            util.warning("Unsupported unit \"" + str(xmlDataFieldNode.find("unit").text.encode("utf-8")) + \
               "\" for \"" + str(xmlDataFieldNode.find("data").text.encode("utf-8")) + "\" (expected \"" + str(expectedUnit.encode("utf-8")) + "\"), corresponding data will be ignored")
            return False

         # Create historizers
         def createSpecificEnumHistorizer(xmlDataFieldNode, xmlTypeNode):
            enumValues = xmlHelper.getEnumValues(xmlDataFieldNode.find("enum"), "item", "description", "value")
            for enumValue in enumValues:
               enumValue[1] = re.split(":|<", enumValue[1])[0]
            cppHistorizerClassName = "C" + cppHelper.toCppName(xmlTypeNode.find("title").text) + "_" + cppHelper.toCppName(xmlDataFieldNode.find("data").text) + "Historizer"
            historizerEnumName = "E" + cppHelper.toCppName(xmlTypeNode.find("title").text) + "_" + cppHelper.toCppName(xmlDataFieldNode.find("data").text) + "_" + cppHelper.toCppName(xmlDataFieldNode.find("data").text)
            cppHistorizerClass = cppClass.CppClass(cppHistorizerClassName, createDefaultCtor=False)
            cppHistorizerClass.inheritFrom("yApi::historization::CSingleHistorizableData<" + historizerEnumName + ">", cppClass.PUBLIC)
            cppHistorizerClass.addConstructor(cppClass.CppClassConstructor("const std::string& keywordName", \
               "CSingleHistorizableData<" + historizerEnumName + ">(keywordName, " + historizerEnumName + "Capacity(), yApi::EKeywordAccessMode::kGet)"))
            cppHistorizerClass.addDependency(cppClass.CppNoDataCapacity(historizerEnumName))
            cppHistorizerClass.addDependency(cppClass.CppExtendedEnumType(historizerEnumName, enumValues))
            return cppHistorizerClass


         def printCtorExtraParameters(ctorExtraParameters):
            if not ctorExtraParameters:
               return ""
            return ", ".join(ctorExtraParameters)


         historizersCppName = []
         if len(xmlTypeNode.findall("case")) != 1:            
            util.warning("func/type : Unsupported number of \"case\" tags (expected 1) for \"" + str(xmlTypeNode.find("title").text.encode("utf-8")) + "\" node. This profile will be ignored.")
         else:
            for xmlDataFieldNode in xmlHelper.findUsefulDataFieldNodes(inXmlNode=xmlTypeNode.find("case")):
               dataText = xmlDataFieldNode.find("data").text
               keywordName = xmlDataFieldNode.find("shortcut").text + " - " + dataText
               historizerCppName = "m_" + cppHelper.toCppName(keywordName)
               cppHistorizerClassName = ""
               ctorExtraParameters = []
               if isLinearValue(xmlDataFieldNode):
                  if dataText == "Temperature":
                     if not supportedUnit(xmlDataFieldNode, u"°C"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CTemperature"
                  elif dataText == "Humidity":
                     if not supportedUnit(xmlDataFieldNode, u"%"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CHumidity"
                  elif dataText == "Barometer":
                     if not supportedUnit(xmlDataFieldNode, u"hPa"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CPressure"
                  elif dataText == "Supply voltage" and xmlDataFieldNode.find("range") is not None:
                     if not supportedUnit(xmlDataFieldNode, u"V"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CVoltage"
                  elif dataText == "Illumination":
                     if not supportedUnit(xmlDataFieldNode, u"lx"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CIllumination"
                  elif dataText == "Illuminance":
                     if not supportedUnit(xmlDataFieldNode, u"lx"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CIllumination"
                  elif str(dataText.encode("utf-8")) == "Sun – West" \
                     or str(dataText.encode("utf-8")) == "Sun – South" \
                     or str(dataText.encode("utf-8")) == "Sun – East":            
                     if not supportedUnit(xmlDataFieldNode, u"klx"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CIllumination"
                  elif dataText == "Energy Storage":
                     if not supportedUnit(xmlDataFieldNode, u"%"):
                        continue
                     cppHistorizerClassName = "yApi::historization::CBatteryLevel"
                  else:
                     util.warning("func/type : Unsupported linear data type \"" + str(dataText.encode("utf-8")) + "\" for \"" + str(xmlTypeNode.find("title").text.encode("utf-8")) + "\" node. This data will be ignored.")
                     continue
               elif isBoolValue(xmlDataFieldNode):
                  cppHistorizerClassName = "yApi::historization::CSwitch"
                  ctorExtraParameters.append(", yApi::EKeywordAccessMode::kGet")
               elif isEnumValue(xmlDataFieldNode):
                  cppHistorizerClass = createSpecificEnumHistorizer(xmlDataFieldNode, xmlTypeNode)
                  typeClass.addDependency(cppHistorizerClass)
                  cppHistorizerClassName = cppHistorizerClass.cppClassName()
               else:
                  util.warning("func/type : Unsupported data type \"" + str(xmlDataFieldNode.find("data").text.encode("utf-8")) + "\" for \"" + str(xmlTypeNode.find("title").text.encode("utf-8")) + "\" node. This data will be ignored.")
                  continue
               typeClass.addMember(cppClass.CppMember(historizerCppName, "boost::shared_ptr<" + cppHistorizerClassName + ">", \
                  cppClass.PRIVATE, cppClass.NO_QUALIFER, initilizationCode= historizerCppName + "(boost::make_shared<" + cppHistorizerClassName + ">(\"" + keywordName + "\"" + printCtorExtraParameters(ctorExtraParameters) + "))"))
               historizersCppName.append(historizerCppName)
               
         typeClass.addMember(cppClass.CppMember("m_historizers", "std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >", cppClass.PRIVATE, cppClass.NO_QUALIFER, \
            initilizationCode="m_historizers( { " + ", ".join(historizersCppName) + " } )"))

         typeClass.addMethod(cppClass.CppMethod("profile", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, \
            "   static const std::string profile(\"" + profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode) + "\");\n" \
            "   return profile;"))
         typeClass.addMethod(cppClass.CppMethod("title", "const std::string&", "", cppClass.PUBLIC, cppClass.CONST | cppClass.OVERRIDE, \
            "   static const std::string title(\"" + xmlTypeNode.find("title").text + "\");\n" \
            "   return title;"))
         typeClass.addMethod(cppClass.CppMethod("allHistorizers", "std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >", "", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, "   return m_historizers;"))
         typeClass.addMethod(cppClass.CppMethod("readInitialState", "void", "const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, "   // No initial state read for generated profile"))
         typeClass.addMethod(cppClass.CppMethod("sendConfiguration", "void", "const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, "   // Device supports no configuration"))
         typeClass.addMethod(cppClass.CppMethod("sendCommand", "void", "const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, "   throw std::logic_error(\"device supports no command sending\");"))

         if not historizersCppName:
            util.warning("No historizer can be created for " + profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode) + " profile. Profile will be not supported.")
            continue


         def statesCodeForLinearValue(xmlDataFieldNode, applyCoef = None, finalCast = None):
            offset = xmlDataFieldNode.find("bitoffs").text
            size = xmlDataFieldNode.find("bitsize").text
            code = "   {\n"
            code += "      auto rawValue = bitset_extract(data, " + offset + ", " + size + ");\n"
            rangeMin = int(xmlDataFieldNode.find("range/min").text)
            rangeMax = int(xmlDataFieldNode.find("range/max").text)
            scaleMin = float(xmlDataFieldNode.find("scale/min").text)
            scaleMax = float(xmlDataFieldNode.find("scale/max").text)
            if applyCoef is not None:
               scaleMin = scaleMin * float(applyCoef)
               scaleMax = scaleMax * float(applyCoef)
            multiplier = (scaleMax - scaleMin) / (rangeMax - rangeMin);
            code += "      auto value = " + str(multiplier) + " * (static_cast<signed>(rawValue) - " + str(rangeMin) + ") + " + str(scaleMin) + ";\n"
            keywordName = xmlDataFieldNode.find("shortcut").text + " - " + xmlDataFieldNode.find("data").text
            historizerCppName = "m_" + cppHelper.toCppName(keywordName)
            if finalCast is not None:
               code += "      " + historizerCppName + "->set(static_cast<" + finalCast + ">(value));\n"
            else:
               code += "      " + historizerCppName + "->set(value);\n"
            code += "   }\n"
            return code

         def statesCodeForBoolValue(xmlDataFieldNode):
            offset = xmlDataFieldNode.find("bitoffs").text
            keywordName = xmlDataFieldNode.find("shortcut").text + " - " + xmlDataFieldNode.find("data").text
            historizerCppName = "m_" + cppHelper.toCppName(keywordName)
            return "   " + historizerCppName + "->set(data[" + offset + "]);\n"

         def statesCode(xmlTypeNode):
            if len(xmlTypeNode.findall("case")) != 1:
               util.warning("func/type : Unsupported number of \"case\" tags (expected 1) for \"" + xmlTypeNode.find("title").text.encode("utf-8") + "\" node. This profile will be ignored.")
               return "   return m_historizers;\n"
            code = ""
            for xmlDataFieldNode in xmlHelper.findUsefulDataFieldNodes(inXmlNode=xmlTypeNode.find("case")):
               if isLinearValue(xmlDataFieldNode):
                  dataText = xmlDataFieldNode.find("data").text
                  if dataText == "Temperature" or \
                     dataText == "Humidity" or \
                     dataText == "Barometer" or \
                     dataText == "Supply voltage" or \
                     dataText == "Illumination" or \
                     dataText == "Illuminance":
                     code += statesCodeForLinearValue(xmlDataFieldNode)
                  elif str(dataText.encode("utf-8")) == "Sun – West" \
                     or str(dataText.encode("utf-8")) == "Sun – South" \
                     or str(dataText.encode("utf-8")) == "Sun – East": # Provided as kilo-lux when Yadoms knows only lux
                     code += statesCodeForLinearValue(xmlDataFieldNode, 1000)
                  elif dataText == "Energy Storage":
                     code += statesCodeForLinearValue(xmlDataFieldNode, applyCoef=None, finalCast="int")
                  else:
                     util.warning("func/type : Unsupported linear data type \"" + str(dataText.encode("utf-8")) + "\" for \"" + str(xmlTypeNode.find("title").text.encode("utf-8")) + "\" node. This data will be ignored.")
                     continue
               elif isBoolValue(xmlDataFieldNode):
                  code += statesCodeForBoolValue(xmlDataFieldNode)
               else:
                  util.warning("func/type : Unsupported data type \"" + str(xmlDataFieldNode.find("data").text.encode("utf-8")) + "\" for \"" + str(xmlTypeNode.find("title").text.encode("utf-8")) + "\" node. This data will be ignored.")
                  continue
            code += "   return m_historizers;"
            return code

         typeClass.addMethod(cppClass.CppMethod("states", "std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >", "unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, statesCode(xmlTypeNode)))
         supportedProfiles.append(profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode))


      def createTypeCode(xmlRorgNode, xmlFuncNode):
         itemNumber = 0
         for xmlTypeNode in xmlFuncNode.findall("type"):
            if profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode) in supportedProfiles:
               itemNumber += 1
         if itemNumber == 0:
            return "   throw std::out_of_range(\"Invalid EFuncIds\");"

         code = "   switch(static_cast<ETypeIds>(typeId))\n"
         code += "   {\n"
         for xmlTypeNode in xmlFuncNode.findall("type"):
            if profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode) not in supportedProfiles:
               continue
            enumValue = cppHelper.toEnumValueName(xmlTypeNode.find("number").text)
            className = cppHelper.toCppName("CProfile_" + profileHelper.profileName(xmlRorgNode, xmlFuncNode, xmlTypeNode))
            code += "   case " + enumValue + ": return boost::make_shared<" + className + ">(deviceId, api);\n"
         code += "   default : throw std::out_of_range(\"Invalid EFuncIds\");\n"
         code += "   }"
         return code
      funcClass.addMethod(cppClass.CppMethod("createType", "boost::shared_ptr<IType>", "unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api", cppClass.PUBLIC, cppClass.OVERRIDE | cppClass.CONST, createTypeCode(xmlRorgNode, xmlFuncNode)))








# Start
print ('Script run with Python version ' + sys.version)
try:
   os.makedirs(outputPath)
except OSError:
   pass

# Generate Header
with codecs.open(os.path.join(outputPath, 'eep.h'), 'w', 'utf_8') as cppHeaderFile:

   cppHeaderFile.write('// Generated file, don\'t modify\n')
   cppHeaderFile.write('#pragma once\n')
   cppHeaderFile.write('#include <shared/plugin/yPluginApi/IYPluginApi.h>\n')
   cppHeaderFile.write('#include <profiles/IRorg.h>\n')
   for dependency in rorgsClass.dependencies():
      cppHeaderFile.write('#include "' + dependency.name() + '.h"\n')
   cppHeaderFile.write('\n')
   cppHeaderFile.write('namespace yApi = shared::plugin::yPluginApi;\n')
   cppHeaderFile.write('\n')

   def generateDependenciesInSameFile(cppType, cppHeaderFile):
      for dependency in cppType.dependencies():
         generateDependenciesInSameFile(dependency, cppHeaderFile)
         dependency.generateHeader(cppHeaderFile)

   def generateRorgDependency(dependency, outputPath):
      with codecs.open(os.path.join(outputPath, dependency.name() + '.h'), 'w', 'utf_8') as cppHeaderSubFile:
         cppHeaderSubFile.write('// Generated file, don\'t modify\n')
         cppHeaderSubFile.write('#pragma once\n')
         cppHeaderSubFile.write('#include <shared/plugin/yPluginApi/IYPluginApi.h>\n')
         cppHeaderSubFile.write('#include <profiles/IRorg.h>\n')
         cppHeaderSubFile.write('\n')
         cppHeaderSubFile.write('namespace yApi = shared::plugin::yPluginApi;\n')
         cppHeaderSubFile.write('\n')
         generateDependenciesInSameFile(dependency, cppHeaderSubFile)
         dependency.generateHeader(cppHeaderSubFile)

   for dependency in rorgsClass.dependencies():
      generateRorgDependency(dependency, outputPath)
   rorgsClass.generateHeader(cppHeaderFile)

# Generate Source
with codecs.open(os.path.join(outputPath, 'eep.cpp'), 'w', 'utf_8') as cppSourceFile:

   cppSourceFile.write('// Generated file, don\'t modify\n')
   cppSourceFile.write('#include "stdafx.h"\n')
   cppSourceFile.write('#include "' + os.path.basename(os.path.join(outputPath, 'eep.h')) + '"\n')
   cppSourceFile.write('#include <shared/plugin/yPluginApi/StandardUnits.h>\n')
   cppSourceFile.write('\n')
   cppSourceFile.write('#include "ProfileHelper.h"\n')
   cppSourceFile.write('\n')

   def generateDependenciesInSameFile(cppType, cppSourceFile):
      for dependency in cppType.dependencies():
         generateDependenciesInSameFile(dependency, cppSourceFile)
         dependency.generateSource(cppSourceFile)

   def generateRorgDependency(dependency, outputPath):
      with codecs.open(os.path.join(outputPath, dependency.name() + '.cpp'), 'w', 'utf_8') as cppSourceSubFile:
         cppSourceSubFile.write('// Generated file, don\'t modify\n')
         cppSourceSubFile.write('#include "stdafx.h"\n')
         cppSourceSubFile.write('#include "' + os.path.basename(os.path.join(outputPath, dependency.name() + '.h')) + '"\n')
         cppSourceSubFile.write('#include <shared/plugin/yPluginApi/StandardUnits.h>\n')
         cppSourceSubFile.write('\n')
         cppSourceSubFile.write('#include <profiles/bitsetHelpers.hpp>\n')
         cppSourceSubFile.write('#include "ProfileHelper.h"\n')
         cppSourceSubFile.write('\n')
         for hardCodedFile in hardCodedProfiles.getProfileHardCodedFiles():
            cppSourceSubFile.write('#include "' + os.path.join('profiles', 'hardCoded', hardCodedFile) + '"\n')
         cppSourceSubFile.write('\n')
         generateDependenciesInSameFile(dependency, cppSourceSubFile)
         dependency.generateSource(cppSourceSubFile)

   for dependency in rorgsClass.dependencies():
      generateRorgDependency(dependency, outputPath)
   rorgsClass.generateSource(cppSourceFile)

# Generate package.json
import generatePackage
generatePackage.generate(packageJsonInPath, packageJsonPath, localesPath, localesInPath, supportedProfiles)

util.finish()

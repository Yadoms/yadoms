#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# CPP class generation

import sys
import os.path
import xml.etree.ElementTree
import string
import cppHelper
import traceback



#-------------------------------------------------------------------------------
# Qualifiers
STATIC = 1
CONST = 2
VIRTUAL = 4
PURE_VIRTUAL = 8
OVERRIDE = 16
NO_QUALIFER = 0

# Member visibility
PUBLIC = 1
PROTECTED = 2
PRIVATE = 3

def visibilityCppTag(visibility):
   return {
        PUBLIC: "public",
        PROTECTED: "protected",
        PRIVATE: "private",
    }[visibility]


#-------------------------------------------------------------------------------
class CppMethod():
   """ Object for generating a cpp class method """

   def __init__(self, cppMethodName, cppReturnType, cppArgs, visibility, qualifier, content = None):
      self.__cppMethodName = cppMethodName
      self.__cppReturnType = cppReturnType
      self.__cppArgs = cppArgs
      self.__visibility = visibility
      self.__qualifier = qualifier
      self.__content = content

   def visibility(self):
      return self.__visibility

   def generateHeader(self, f, parentClassName):
      try:
         f.write("   ");
         if self.__qualifier & VIRTUAL or self.__qualifier & PURE_VIRTUAL:
            f.write("virtual ");
         if self.__qualifier & STATIC:
            f.write("static ");
         f.write(self.__cppReturnType + " " + self.__cppMethodName + "(" + self.__cppArgs + ")")
         if self.__qualifier & CONST:
            f.write(" const");
         if self.__qualifier & OVERRIDE:
            f.write(" override");
         if self.__qualifier & PURE_VIRTUAL:
            f.write(" = 0");
         f.write(";\n")
      except Exception as e:
         print "error : generating header of " + parentClassName + "::" + self.__cppMethodName
         print traceback.format_exc()

   def generateSource(self, f, parentClassName):
      try:
         if self.__qualifier & PURE_VIRTUAL:
            return
         f.write(self.__cppReturnType + " " + parentClassName + "::" + self.__cppMethodName + "(" + self.__cppArgs + ")");
         if self.__qualifier & CONST:
            f.write(" const");
         f.write(" {\n");
         f.write(self.__content + "\n");
         f.write("}\n");
      except Exception as e:
         print "error : generating source of " + parentClassName + "::" + self.__cppMethodName
         print traceback.format_exc()


#-------------------------------------------------------------------------------
class CppMember():
   """ Object for generating a cpp class member """

   def __init__(self, cppMemberName, cppType, visibility = PRIVATE, qualifier = NO_QUALIFER, initilizationCode = None):
      self.__cppMemberName = cppMemberName
      self.__cppType = cppType
      self.__visibility = visibility
      self.__qualifier = qualifier
      self.__initilizationCode = initilizationCode

   def qualifier(self):
      return self.__qualifier

   def visibility(self):
      return self.__visibility

   def initilizationCode(self):
      return self.__initilizationCode

   def generateHeader(self, f, parentClassName):
      try:
         f.write("   ");
         if self.__qualifier & STATIC:
            f.write("static ");
         if self.__qualifier & CONST:
            f.write("const ");
         f.write(self.__cppType + " " + self.__cppMemberName + ";\n")
      except Exception as e:
         print "error : generating header of " + parentClassName + "::" + self.__cppMemberName
         print traceback.format_exc()

   def generateSource(self, f, parentClassName):
      try:
         if not self.__qualifier & STATIC:
            return

         if self.__qualifier & CONST:
            f.write("const ");
         f.write(self.__cppType + " " + parentClassName + "::" + self.__cppMemberName);
         if self.__initilizationCode:
            f.write(" = " + self.__initilizationCode)
         f.write("\n")
      except Exception as e:
         print "error : generating source of " + parentClassName + "::" + self.__cppMemberName
         print traceback.format_exc()



#-------------------------------------------------------------------------------
class CppType(object):
   """ Object for generating a cpp type of higher level """

   def __init__(self, cppTypeName):
      self._cppTypeName = cppTypeName
      self.__dependencies = []

   def generateHeader(self, f):
      raise NotImplementedError()

   def generateSource(self, f):
      raise NotImplementedError()

   def addDependency(self, dependency):
      self.__dependencies.append(dependency)

   def dependencies(self):
      return self.__dependencies



#-------------------------------------------------------------------------------
class CppSubType(object):
   """ Object for generating a cpp sub type """

   def __init__(self, cppTypeName, visibility = PRIVATE):
      self._cppTypeName = cppTypeName
      self.__visibility = visibility

   def visibility(self):
      return self.__visibility

   def generateHeader(self, f, parentClassName):
      raise NotImplementedError()

   def generateSource(self, f, parentClassName):
      raise NotImplementedError()


#-------------------------------------------------------------------------------
class CppEnumType(CppSubType):
   """ Object for generating a cpp enum type """

   def __init__(self, cppEnumName, listItems, visibility = PRIVATE):
      super(CppEnumType, self).__init__(cppEnumName, visibility)
      self.__listItems = listItems

   def generateHeader(self, f, parentClassName):
      f.write("enum " + self._cppTypeName + " {\n")
      for item in self.__listItems:
         # Enum item name
         f.write("   " + cppHelper.toEnumValueName(item[0]))
         # If available, enum item value
         if item[1]:
            f.write(" = " + item[1])
         f.write(",\n")
      f.write("};\n")

   def generateSource(self, f, parentClassName):
      pass


#-------------------------------------------------------------------------------
class CppExtendedEnumType(CppType):
   """ Object for generating a Yadoms extended enum type """

   def __init__(self, cppEnumName, listItems, visibility = PRIVATE):
      super(CppExtendedEnumType, self).__init__(cppEnumName)
      self.__listItems = listItems

   def generateHeader(self, f):
      f.write("DECLARE_ENUM_HEADER(" + self._cppTypeName + ",\n")
      for item in self.__listItems:
         # Enum item name
         f.write("   ((" + cppHelper.toEnumValueName(item[0]) + ")")
         # If available, enum item value
         if item[1]:
            f.write("(" + item[1] + ")")
         f.write(")\n")
      f.write(");\n\n")

   def generateSource(self, f):
      f.write("DECLARE_ENUM_IMPLEMENTATION(" + self._cppTypeName + ",\n")
      for item in self.__listItems:
         # Enum item name
         f.write("   ((" + cppHelper.toEnumValueName(item[0]) + "))\n")
      f.write(");\n")


#-------------------------------------------------------------------------------
class CppClassConstructor():
   """ Object for generating a CPP class constructor """

   def __init__(self, args, init = None, code = None, visibility = PUBLIC):
      self.__args = args
      self.__init = init
      self.__code = code
      self.__visibility = visibility

   def generateHeader(self, f, parentClassName):
      try:
         f.write(visibilityCppTag(self.__visibility) + ":\n")
         f.write("   ")
         if self.__args and len(self.__args) > 0 and self.__args.count(',') == 0:
            f.write("explicit ")
         f.write(parentClassName + "(" + self.__args + ");\n")
      except Exception as e:
         print "error : generating header of " + parentClassName + "::" + parentClassName
         print traceback.format_exc()

   def generateSource(self, f, initializersCode, parentClassName):
      try:
         f.write(parentClassName + "::" + parentClassName + "(" + self.__args + ")\n")
         if initializersCode or self.__init:
            f.write(": ")
         if initializersCode:
            f.write(",\n  ".join(initializersCode))
            if self.__init:
               f.write(",\n")
         if self.__init:
            f.write(self.__init + "\n")
         f.write("{\n")
         if self.__code:
            f.write(self.__code + "\n")
         f.write("}\n")
      except Exception as e:
         print "error : generating source of " + parentClassName + "::" + parentClassName
         print traceback.format_exc()


#-------------------------------------------------------------------------------
class CppClass(CppType):
   """ Object for generating a cpp class """

   def __init__(self, cppClassName, createDefaultCtor=True):
      super(CppClass, self).__init__(cppClassName)
      self.__cppClassName = cppClassName
      self.__createDefaultCtor = createDefaultCtor
      self.__subTypes = []
      self.__constructors = []
      self.__members = []
      self.__methods = []
      self.__extraContentInHeader = []
      self.__extraContentInSource = []
      self.__parentClasses = dict()
      self.__comments = []


   def inheritFrom(self, parentClasses, visibility):
      try:
         self.__parentClasses[visibility] += parentClasses + " "
      except:
         self.__parentClasses[visibility] = parentClasses + " "


   def addSubType(self, cppSubType):
      if not isinstance(cppSubType, CppSubType) :
         raise TypeError(str(cppSubType) + " object is not an instance of CppSubType")
      self.__subTypes.append(cppSubType)


   def addConstructor(self, cppClassConstructor):
      self.__constructors.append(cppClassConstructor)


   def addMethod(self, cppMethod):
      if not isinstance(cppMethod, CppMethod) :
         raise TypeError(str(cppMethod) + " object is not an instance of CppMethod")
      self.__methods.append(cppMethod)


   def addMember(self, cppMember):
      if not isinstance(cppMember, CppMember) :
         raise TypeError(str(cppMember) + " object is not an instance of CppMember")
      self.__members.append(cppMember)


   def addComment(self, comment):
      self.__comments.append(comment)


   def name(self):
      return self.__cppClassName # TODO revoir

   def cppClassName(self):
      return self.__cppClassName


   def __generateHeaderVisibilityBlock(self, cppHeaderFile, visibility):
      cppHeaderFile.write(visibilityCppTag(visibility) + ":\n")
      for subType in self.__subTypes:
         if subType.visibility() is visibility:
            subType.generateHeader(cppHeaderFile, self.__cppClassName)
      for member in self.__members:
         if member.visibility() is visibility:
            member.generateHeader(cppHeaderFile, self.__cppClassName)
      for method in self.__methods:
         if method.visibility() is visibility:
            method.generateHeader(cppHeaderFile, self.__cppClassName)


   def generateHeader(self, f):
      try:
         if self.__comments is not None:
            for comment in self.__comments:
               f.write("// " + comment + "\n")

         f.write("class " + self.__cppClassName)

         # Parent classes
         if self.__parentClasses:
            f.write(" : ")
            if self.__parentClasses.has_key(PUBLIC):
               publicClassesString = visibilityCppTag(PUBLIC) + " " + self.__parentClasses[PUBLIC]
            if self.__parentClasses.has_key(PROTECTED):
               protectedClassesString = visibilityCppTag(PROTECTED) + " " + self.__parentClasses[PROTECTED]
            if self.__parentClasses.has_key(PRIVATE):
               privateClassesString = visibilityCppTag(PRIVATE) + " " + self.__parentClasses[PRIVATE]

            if 'publicClassesString' in locals():
               f.write(publicClassesString)
               if 'protectedClassesString' in locals() or 'privateClassesString' in locals():
                  f.write(", ")
            if 'protectedClassesString' in locals():
               f.write(protectedClassesString)
               if 'privateClassesString' in locals():
                  f.write(", ")
            if 'privateClassesString' in locals():
               f.write(privateClassesString)

         f.write(" {\n")

         # Ctor and dtor
         f.write(visibilityCppTag(PUBLIC) + ":\n")
         if self.__createDefaultCtor:
            f.write("   " + self.__cppClassName + "();\n")
         for constructor in self.__constructors:
            constructor.generateHeader(f, self.__cppClassName)
         f.write("   virtual ~" + self.__cppClassName + "();\n")

         self.__generateHeaderVisibilityBlock(f, PUBLIC)
         self.__generateHeaderVisibilityBlock(f, PROTECTED)
         self.__generateHeaderVisibilityBlock(f, PRIVATE)

         f.write("};\n\n")

      except Exception as e:
         print "error : generating header of " + self.__cppClassName
         print traceback.format_exc()


   def __generateMembersInitializersCode(self):
      initializers = []
      for member in self.__members:
         if member.qualifier() & STATIC:
            continue
         if member.initilizationCode():
            initializers.append(member.initilizationCode())
      return initializers


   def generateSource(self, f):
      try:
         # Subtypes
         for subType in self.__subTypes:
            subType.generateSource(f, self.__cppClassName)

         # Initialization of static members
         for member in self.__members:
            if member.qualifier() & STATIC:
               member.generateSource(f, self.__cppClassName)

         # Ctor and dtor
         if self.__createDefaultCtor:
            f.write(self.__cppClassName + "::" + self.__cppClassName + "()\n")
            initializersCode = ",\n  ".join(self.__generateMembersInitializersCode())
            if initializersCode:
               f.write(": " + initializersCode + "\n")
            f.write("{}\n")

         for constructor in self.__constructors:
            constructor.generateSource(f, self.__generateMembersInitializersCode(), self.__cppClassName)

         f.write(self.__cppClassName + "::~" + self.__cppClassName + "()\n")
         f.write("{}\n")

         # Methods
         for method in self.__methods:
            method.generateSource(f, self.__cppClassName)
         f.write("\n\n")

      except Exception as e:
         print "error : generating source of " + self.__cppClassName
         print traceback.format_exc()

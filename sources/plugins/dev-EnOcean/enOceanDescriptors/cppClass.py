import sys
import os.path
import xml.etree.ElementTree
import string



#-------------------------------------------------------------------------------
# Qualifiers
STATIC = 1
CONST = 2
VIRTUAL = 4
PURE_VIRTUAL = 8
NO_QUALIFER = None

# Member visibility
PUBLIC = 1
PROTECTED = 2
PRIVATE = 3

#-------------------------------------------------------------------------------
class CppMethod():
   """ Object for generating a cpp class method """

   def __init__(self, cppMethodName, cppReturnType, cppArgs, visibility, qualifier, content):
      self.__cppMethodName = cppMethodName
      self.__cppReturnType = cppReturnType
      self.__cppArgs = cppArgs
      self.__visibility = visibility
      self.__qualifier = qualifier
      self.__content = content

   def visibility(self):
      return self.__visibility

   def generateHeader(self, f):
      f.write("   ");
      if self.__qualifier & VIRTUAL or self.__qualifier & PURE_VIRTUAL:
         f.write("virtual ");
      if self.__qualifier & STATIC:
         f.write("static ");
      f.write(self.__cppReturnType + " " + self.__cppMethodName + "(" + self.__cppArgs + ")")
      if self.__qualifier & CONST:
         f.write(" const");
      if self.__qualifier & PURE_VIRTUAL:
         f.write(" = 0");
      f.write(";\n")

   def generateSource(self, f, parentClassName):
      if self.__qualifier & PURE_VIRTUAL:
         return
      f.write(self.__cppReturnType + " " + parentClassName + "::" + self.__cppMethodName + "(" + self.__cppArgs + ") {\n");
      f.write(self.__content + "\n");
      f.write("}\n");


#-------------------------------------------------------------------------------
class CppMember():
   """ Object for generating a cpp class member """

   def __init__(self, cppMemberName, cppType, visibility = PRIVATE, qualifier = NO_QUALIFER, initilizationCodeFct = None):
      self.__cppMemberName = cppMemberName
      self.__cppType = cppType
      self.__visibility = visibility
      self.__qualifier = qualifier
      self.__initilizationCodeFct = initilizationCodeFct

   def qualifier(self):
      return self.__qualifier

   def visibility(self):
      return self.__visibility

   def generateHeader(self, f):
      f.write("   ");
      if self.__qualifier & STATIC:
         f.write("static ");
      if self.__qualifier & CONST:
         f.write("const ");
      f.write(self.__cppType + " " + self.__cppMemberName + ";\n")

   def generateSource(self, f, parentClassName):
      if not self.__qualifier & STATIC:
         return

      if self.__qualifier & CONST:
         f.write("const ");
      f.write(self.__cppType + " " + parentClassName + "::" + self.__cppMemberName);
      if self.__initilizationCodeFct:
         f.write(" = " + self.__initilizationCodeFct())
      f.write("\n")



#-------------------------------------------------------------------------------
class CppSubType(object):
   """ Object for generating a cpp sub type """

   def __init__(self, cppTypeName, visibility = PRIVATE):
      self._cppTypeName = cppTypeName
      self.__visibility = visibility

   def visibility(self):
      return self.__visibility

   def generateHeader(self, f):
      raise NotImplementedError()



#-------------------------------------------------------------------------------
class CppEnumType(CppSubType):
   """ Object for generating a cpp enum type """

   def __init__(self, cppEnumName, listItemsFct, visibility = PRIVATE):
      super(CppEnumType, self).__init__(cppEnumName, visibility)
      self.__listItemsFct = listItemsFct 

   def generateHeader(self, f):
      f.write("enum " + self._cppTypeName + " {\n")
      for item in self.__listItemsFct():
         # Enum item name
         f.write("   " + item[0])
         # If available, enum item value
         if item[1]:
            f.write(" = " + item[1])
         f.write(",\n")
      f.write("};\n")


#-------------------------------------------------------------------------------
class CppClass():
   """ Object for generating a cpp class """

   def __init__(self, cppClassName):
      self.__cppClassName = cppClassName
      self.__subTypes = []
      self.__members = []
      self.__methods = []
      self.__extraContentInHeader = []
      self.__extraContentInSource = []


   def addSubType(self, cppSubType):
      if not isinstance(cppSubType, CppSubType) :
         raise TypeError(str(cppSubType) + " object is not an instance of CppSubType")
      self.__subTypes.append(cppSubType)


   def addMethod(self, cppMethod):
      if not isinstance(cppMethod, CppMethod) :
         raise TypeError(str(cppMethod) + " object is not an instance of CppMethod")
      self.__methods.append(cppMethod)


   def addMember(self, cppMember):
      if not isinstance(cppMember, CppMember) :
         raise TypeError(str(cppMember) + " object is not an instance of CppMember")
      self.__members.append(cppMember)


   def addExtraContentInHeader(self, extraContentInHeaderFct, visibility = PRIVATE):
      if type(extraContentInHeaderFct) is not string:
         raise TypeError(extraContentInHeaderFct)
      self.__extraContentInHeader.append([visibility, extraContentInHeaderFct])


   def addExtraContentInSource(self, extraContentInSourceFct, visibility = PRIVATE):
      if type(extraContentInSourceFct) is not string:
         raise TypeError(extraContentInSourceFct)
      self.__extraContentInSource.append([visibility, extraContentInSourceFct])


   def __generateHeaderVisibilityBlock(self, cppHeaderFile, visibility, visibilityCppTag):
      cppHeaderFile.write(visibilityCppTag + ":\n")
      for subType in self.__subTypes:
         if subType.visibility() is visibility:
            subType.generateHeader(cppHeaderFile)
      for member in self.__members:
         if member.visibility() is visibility:
            member.generateHeader(cppHeaderFile)
      for method in self.__methods:
         if method.visibility() is visibility:
            method.generateHeader(cppHeaderFile)


   def generateHeader(self, f):
      f.write("class " + self.__cppClassName + " {\n")

      # Ctor and dtor
      f.write("public:\n")
      f.write("   " + self.__cppClassName + "();\n")
      f.write("   virtual ~" + self.__cppClassName + "();\n")

      self.__generateHeaderVisibilityBlock(f, PUBLIC, "public")
      self.__generateHeaderVisibilityBlock(f, PROTECTED, "protected")
      self.__generateHeaderVisibilityBlock(f, PRIVATE, "private")

      f.write("};\n")


   def generateSource(self, f):

      # Initialization of static members
      for member in self.__members:
         if member.qualifier() & STATIC:
            member.generateSource(f, self.__cppClassName)

      # Ctor and dtor
      f.write(self.__cppClassName + "::" + self.__cppClassName + "(){}\n")
      f.write("\n")
      f.write(self.__cppClassName + "::~" + self.__cppClassName + "(){}\n")
      f.write("\n")
      for method in self.__methods:
         method.generateSource(f, self.__cppClassName)
import yadomsServer
import resources
import os.path
import shutil
import re

def deleteAll():
   """Remove all existing rules"""
   
   resources.deleteTree(yadomsServer.scriptsPath())
            
         
def deploy(scripts):
   """Deploy scripts into Yadoms runtime path. Scripts are deployed in order"""
   
   deleteAll()
   
   for index, script in enumerate(scripts):
      ruleTargetPath = os.path.join(yadomsServer.scriptsPath(), "locals", "rule_" + str(index + 1))
      os.makedirs(ruleTargetPath)
      shutil.copyfile(os.path.join("resources", "scripts", script + ".py"), os.path.join(ruleTargetPath, "yadomsScript.py"))
      
      
def checkLocalRuleCodeById(id, expectedCode):
   """Check the local rule for expected code"""
   
   rulePath = os.path.join(yadomsServer.scriptsPath(), "locals", "rule_" + str(id))
   if not os.path.isdir(rulePath):
      return False
   
   ruleFilePath = os.path.join(rulePath, "yadomsScript.py")
   if not os.path.isfile(ruleFilePath):
      return False
      
   readCode = []
   with open(ruleFilePath, 'r') as ruleFile:
      for line in ruleFile:
         readCode.append(line.rstrip())

   # Remove BOM
   if len(readCode) > 0 and readCode[0][0] == '\xef' and readCode[0][1] == '\xbb' and readCode[0][2] == '\xbf':
      readCode[0] = readCode[0][3:]
      
   if (readCode != expectedCode):
      print ">>>>>>>>>>> Read code : "
      print readCode
      print ">>>>>>>>>>> Expected code : "
      print expectedCode
      return False
      
   return True
   
      
def checkLocalRuleLogById(id, expectedLog):
   """Check the local rule for expected log"""
   
   rulePath = os.path.join(yadomsServer.scriptsPath(), "locals", "rule_" + str(id))
   if not os.path.isdir(rulePath):
      return False
   
   ruleLogFilePath = os.path.join(rulePath, "yadomsScript.log")
   if not os.path.isfile(ruleLogFilePath):
      return False
   
   readLog = []
   with open(ruleLogFilePath, 'r') as ruleFile:
      for line in ruleFile:
         # Remove date/time for comparison
         readLog.append(re.sub("\d{4}\/\d{1,2}\/\d{1,2} \d{2}:\d{2}:\d{2} : ", "", line))
         
   if (readLog != expectedLog):
      print ">>>>>>>>>>> Read log : "
      print readLog
      print ">>>>>>>>>>> Expected log : "
      print expectedLog
      return False         

   return True
   
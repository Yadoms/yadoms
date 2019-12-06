import yadomsServer
import resources
import os.path
import shutil
import re

def deleteAll():
   print ('Remove all scripts and associated logs')
   resources.deleteTree(yadomsServer.scriptsPath())
   resources.deleteTree(yadomsServer.scriptLogsPath())
            
         
def deploy(scripts):
   
   deleteAll()
   print ('Deploy scripts {scripts}')
   
   for index, script in enumerate(scripts):
      ruleTargetPath = os.path.join(yadomsServer.scriptsPath(), "locals", "rule_" + str(index + 1))
      os.makedirs(ruleTargetPath)
      shutil.copyfile(os.path.join("resources", "scripts", script + ".py"), os.path.join(ruleTargetPath, "yadomsScript.py"))
            
         
def ruleFullPath(id):
   """Get the rule full path for rule id"""
   rulePath = os.path.join(yadomsServer.scriptsPath(), "locals", "rule_" + str(id))
   ruleFilePath = os.path.join(rulePath, "yadomsScript.py")
   return ruleFilePath
      
      
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
      print ('>>>>>>>>>>> Read code : ')
      print readCode
      print ('>>>>>>>>>>> Expected code : ')
      print expectedCode
      return False
      
   return True
   
      
def checkLocalRuleLogById(id, expectedLog):
   """Check the local rule for expected log"""
   
   ruleLogPath = os.path.join(yadomsServer.scriptLogsPath(), str(id))
   if not os.path.isdir(ruleLogPath):
      return False
   
   ruleLogFilePath = os.path.join(ruleLogPath, "rule.log")
   if not os.path.isfile(ruleLogFilePath):
      return False
   
   readLog = []
   with open(ruleLogFilePath, 'r') as ruleFile:
      # Skip first line (containing something like "# Log file created/rotated Monday, 23 Jan 17 13:54:56 GMT")
      next(ruleFile)
      for line in ruleFile:
         # Remove date/time for comparison
         readLog.append(re.sub("\w{3}, \d{2}:\d{2}:\d{2} : ", "", line))
         
   if (readLog != expectedLog):
      print ('>>>>>>>>>>> Read log     : {readLog}')
      print ('>>>>>>>>>>> Expected log : {expectedLog}')
      return False         

   return True
   
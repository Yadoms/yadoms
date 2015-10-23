import yadomsServer
import os.path
import shutil

def deleteAll():
   """Remove all existing rules"""

   if (os.path.isdir(yadomsServer.scriptsPath())):
      shutil.rmtree(yadomsServer.scriptsPath())
      
      
def checkLocalRuleById(id, expectedCode):
   """Check the local rule for expected code"""
   
   rulePath = os.path.join(yadomsServer.scriptsPath(), "locals", "rule_" + str(id))
   assert os.path.isdir(rulePath)
   
   ruleFilePath = os.path.join(rulePath, "yadomsScript.py")
   assert os.path.isfile(ruleFilePath)
   
   readCode = []
   with open(ruleFilePath, 'r') as ruleFile:
      for line in ruleFile:
         readCode.append(line.rstrip())
         
   assert readCode == expectedCode

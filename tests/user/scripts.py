import yadomsServer
import resources
import os.path
import shutil

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

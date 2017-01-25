import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
import database
import config
import scripts
import yadomsServer
import dashboard
import dashboard.automation
import notification
import i18n
import tools
import os.path

class CreateRule(unittest.TestCase):
   """Create rule test"""
   
   def setUp(self):
      database.new()
      config.deploy("nominal")
      scripts.deleteAll()
      # We have to activate log to be able to check rules logs
      self.serverProcess = yadomsServer.start(["logLevel=trace"])
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      

   def checkCreateOkRule(self, ruleName, ruleDescription, ruleCode, ruleLog):
      print '  Check notification'
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      
      print '  Check rule was inserted in rules table'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)

      ruleNumber = 0
      
      print '  Check rule data in rules table'
      self.assertEqual(len(dashboard.automation.getRuleDatas(rulesTable, ruleNumber)), 5)
      self.assertEqual(dashboard.automation.getRuleName(rulesTable, ruleNumber), ruleName)
      self.assertEqual(dashboard.automation.getRuleDescription(rulesTable, ruleNumber), ruleDescription)
      self.assertTrue(dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber))

      buttons = dashboard.automation.getRuleButtons(rulesTable, ruleNumber)
      self.assertEqual(len(buttons), 4)
      self.assertEqual(dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-startStop btn-warning")
      self.assertEqual(dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-delete btn-danger")

      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Running)
      
      
      print '  Check rule was created on disk (corresponding script file)'
      self.assertTrue(scripts.checkLocalRuleCodeById(1, ruleCode))
      self.assertTrue(tools.waitUntil(lambda: scripts.checkLocalRuleLogById(1, ruleLog)))
            
            
   def test_createOkRule(self):
      print '=== Nominal test of creation rule ==='
      self.doTest(
         "TestingOkRule",
         "This rule is just for testing",
         ["import time",
          "",
          "def yMain(yApi):",
          "   while(True):",
          "      print 'Some log entry...'",
          "      while(True):",
          "         time.sleep(100)"],
         lambda ruleName, ruleDescription, ruleCode: self.checkCreateOkRule(ruleName, ruleDescription, ruleCode,
            ['[Information] : #### START ####\n',
             '[Information] : Some log entry...\n']))
            
            


   def checkCreateErroneousRule(self, ruleName, ruleDescription, ruleCode, ruleLog):
      print '  Check notifications'
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      notification.waitSubText(self.browser, notification.Type.Error, i18n.get()["eventLogger"]["RuleFailed"].replace("__who__", ruleName))
      
      print '  Check rule was inserted in rules table'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)

      ruleNumber = 0
      
      print '  Check rule data in rules table'
      self.assertEqual(len(dashboard.automation.getRuleDatas(rulesTable, ruleNumber)), 5)
      self.assertEqual(dashboard.automation.getRuleName(rulesTable, ruleNumber), ruleName)
      self.assertEqual(dashboard.automation.getRuleDescription(rulesTable, ruleNumber), ruleDescription)
      self.assertTrue(dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber))

      buttons = dashboard.automation.getRuleButtons(rulesTable, ruleNumber)
      self.assertEqual(len(buttons), 4)
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-enableDisable btn-success"))
      self.assertEqual(dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-delete btn-danger")
      
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleState(rulesTable, ruleNumber) == dashboard.automation.RuleState.Error))
      
      print '  Check rule was created on disk (corresponding script file)'
      self.assertTrue(scripts.checkLocalRuleCodeById(1, ruleCode))
      self.assertTrue(tools.waitUntil(lambda: scripts.checkLocalRuleLogById(1, ruleLog)))
      
      
   def test_createErroneousRule(self):
      print '=== Test of rule creation, with error in code ==='
      self.doTest(
         "TestingErroneousRule",
         "This rule is just for testing",
         ["import time",
          "",
          "def yMain(yApi):",
          "   while(True)",
          "      print 'Some log entry...'",
          "      while(True):",
          "         time.sleep(100)"],
         lambda ruleName, ruleDescription, ruleCode: self.checkCreateErroneousRule(ruleName, ruleDescription, ruleCode,
            ['[Information] : #### START ####\n',
             '[Error] : Traceback (most recent call last):\n',
             '[Error] :   File "scriptCaller.py", line 36, in <module>\n',
             '[Error] :     script = __import__(scriptModule)\n',
             '[Error] :   File "' + os.path.abspath(scripts.ruleFullPath(1)) + '", line 4\n',
             '[Error] :     while(True)\n',
             '[Error] :               ^\n',
             '[Error] : SyntaxError: invalid syntax\n']))

         
      
   def doTest(self, ruleName, ruleDescription, ruleCode, checkRuleFct):
      print 'Open rules dashboard'
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)

      print 'Create new rule'
      tools.waitUntil(lambda: dashboard.automation.getCreateRuleButton(self.browser).is_enabled())
      dashboard.automation.getCreateRuleButton(self.browser).click()
      newRuleModal = dashboard.automation.waitNewRuleModal(self.browser)
      
      print 'Select code'
      newRuleModal.getEditorSelectionButton('code').click()

      print 'Configure rule'
      editRuleModal = dashboard.automation.waitEditRuleModal(self.browser)
      print '  Set rule name'
      editRuleModal.setRuleName(ruleName)
      print '  Set rule description'
      editRuleModal.setRuleDescription(ruleDescription)
         
      print '  Set rule code'
      editRuleModal.getRuleCodeEditor().clear()
      editRuleModal.getRuleCodeEditor().writeCode(ruleCode)


      print 'Confirm'
      editRuleModal.ok()
      
      
      print 'Check created rule'
      checkRuleFct(ruleName, ruleDescription, ruleCode)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
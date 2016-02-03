import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
import database
import scripts
import yadomsServer
import dashboard
import dashboard.automation
import notification
import i18n
import tools

class CreateRule(unittest.TestCase):
   """Create rule test"""
   
   def setUp(self):
      database.new()
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      

   def checkCreateOkRule(self, ruleName, ruleDescription, ruleCode, ruleLog):
      # - notification
      notification.wait(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      
      # - in web client
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)

      ruleNumber = 0
      
      self.assertEqual(len(dashboard.automation.getRuleDatas(rulesTable, ruleNumber)), 4)
      self.assertEqual(dashboard.automation.getRuleName(rulesTable, ruleNumber), ruleName)
      self.assertEqual(dashboard.automation.getRuleDescription(rulesTable, ruleNumber), ruleDescription)

      buttons = dashboard.automation.getRuleButtons(rulesTable, ruleNumber)
      self.assertEqual(len(buttons), 4)
      self.assertEqual(dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-enableDisable btn-warning")
      self.assertEqual(dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-delete btn-danger")

      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Running)
      
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleCodeById(1, ruleCode)
      self.assertTrue(tools.waitUntil(lambda: scripts.checkLocalRuleLogById(1, ruleLog)))
            
            
   def test_createOkRule(self):
      """Nominal test of creation rule"""
      self.doTest(
         "TestingOkRule",
         "This rule is just for testing",
         ["import time",
          "",
          "def yMain(yApi):",
          "   while(True):",
          "      print 'Some log entry...'",
          "      time.sleep(100)"],
         lambda ruleName, ruleDescription, ruleCode: self.checkCreateOkRule(ruleName, ruleDescription, ruleCode,
            ['#### START ####\n',
             '#### END ####\n',
             'Some log entry...\n']))
            
            


   def checkCreateErroneousRule(self, ruleName, ruleDescription, ruleCode, ruleLog):
      # - notifications
      notification.wait(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      notification.waitIn(self.browser, notification.Type.Error, i18n.get()["eventLogger"]["RuleFailed"].replace("__who__", ruleName))
      
      # - in web client
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)

      ruleNumber = 0
      
      self.assertEqual(len(dashboard.automation.getRuleDatas(rulesTable, ruleNumber)), 4)
      self.assertEqual(dashboard.automation.getRuleName(rulesTable, ruleNumber), ruleName)
      self.assertEqual(dashboard.automation.getRuleDescription(rulesTable, ruleNumber), ruleDescription)

      buttons = dashboard.automation.getRuleButtons(rulesTable, ruleNumber)
      self.assertEqual(len(buttons), 4)
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-enableDisable btn-success"))
      self.assertEqual(dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber).get_attribute("class"), "btn btn-delete btn-danger")
      
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleState(rulesTable, ruleNumber) == dashboard.automation.RuleState.Error))
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleCodeById(1, ruleCode)
      self.assertTrue(tools.waitUntil(lambda: scripts.checkLocalRuleLogById(1, ruleLog)))
      
      
   def test_createErroneousRule(self):
      """Test of rule creation, with error in code"""
      self.doTest(
         "TestingErroneousRule",
         "This rule is just for testing",
         ["import time",
          "",
          "def yMain(yApi):",
          "   while(True)",
          "      print 'Some log entry...'",
          "      time.sleep(100)"],
         lambda ruleName, ruleDescription, ruleCode: self.checkCreateErroneousRule(ruleName, ruleDescription, ruleCode,
            ['#### START ####\n',
             '#### END ####\n',
             'Traceback (most recent call last):\n',
             '  File "scriptCaller.py", line 35, in <module>\n',
             '    script = __import__(scriptModule)\n',
             '  File "D:\Projets\Domotique\yadoms-code\\trunk\\builds\\DEBUG\\scripts\\locals\\rule_1\yadomsScript.py", line 4\n',
             '    while(True)\n',
             '              ^\n',
             'SyntaxError: invalid syntax\n']))

         
      
   def doTest(self, ruleName, ruleDescription, ruleCode, checkRuleFct):
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)

      # Create new rule
      dashboard.automation.getCreateRuleButton(self.browser).click()
      newRuleModal = dashboard.automation.waitNewRuleModal(self.browser)
      newRuleModal.getEditorSelectionButton("code").click()

      # - Rule configuration
      editRuleModal = dashboard.automation.waitEditRuleModal(self.browser)
      editRuleModal.getRuleName(self.browser).send_keys(ruleName)
      editRuleModal.getRuleDescription(self.browser).send_keys(ruleDescription)
         
      # - Rule code
      editRuleModal.getRuleCodeEditor(self.browser).writeCode(ruleCode)


      # Click OK
      editRuleModal.getConfirmConfigureRuleButton(self.browser).click()
      
      
      # Check created rule
      checkRuleFct(ruleName, ruleDescription, ruleCode)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
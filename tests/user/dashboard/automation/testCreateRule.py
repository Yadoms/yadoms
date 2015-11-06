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
      yadomsServer.openClient(self.browser)
      

   def checkCreateOkRule(self, ruleName, ruleDescription, ruleCode):
      # - notification
      notification.wait(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      
      # - in web client
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, 0)
         
      self.assertEqual(len(ruleDatas), 4)
      self.assertEqual(ruleDatas[0].text, ruleName)
      self.assertEqual(ruleDatas[1].text, ruleDescription)

      buttons = dashboard.automation.getRuleButtons(ruleDatas[2])
      self.assertEqual(len(buttons), 3)
      self.assertEqual(buttons[0].get_attribute("class"), "btn btn-enableDisable btn-warning")
      self.assertEqual(buttons[1].get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(buttons[2].get_attribute("class"), "btn btn-delete btn-danger")

      self.assertEqual(dashboard.automation.getRuleState(rulesTable, 0), dashboard.automation.RuleState.Running)
      
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleById(1, ruleCode)
            
            
   def test_createRule(self):
      """Nominal test of creation rule"""
      self.doTest(
         "TestingOkRule",
         "This rule is just for testing",
         ["import time",
                     "",
                     "def yMain(yApi):",
                     "   while(True):",
                     "      print 'location = ', yApi.getInfo(yApi.kLatitude)",
                     "      time.sleep(5)"],
         lambda ruleName, ruleDescription, ruleCode: self.checkCreateOkRule(ruleName, ruleDescription, ruleCode))
            
            


   def checkCreateErroneousRule(self, ruleName, ruleDescription, ruleCode):
      # - notifications
      notification.wait(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      notification.waitIn(self.browser, notification.Type.Error, i18n.get()["eventLogger"]["RuleFailed"].replace("__who__", ruleName))
      
      # - in web client
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, 0)
         
      self.assertEqual(len(ruleDatas), 4)
      self.assertEqual(ruleDatas[0].text, ruleName)
      self.assertEqual(ruleDatas[1].text, ruleDescription)

      buttons = dashboard.automation.getRuleButtons(ruleDatas[2])
      self.assertEqual(len(buttons), 3)
      self.assertEqual(buttons[0].get_attribute("class"), "btn btn-enableDisable btn-warning")
      self.assertEqual(buttons[1].get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(buttons[2].get_attribute("class"), "btn btn-delete btn-danger")
      
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleState(rulesTable, 0) == dashboard.automation.RuleState.Error))
      
      #TODO ajouter champ "last error"
      
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleById(1, ruleCode)
      
      
   def test_createErroneousRule(self):
      """Test of rule creation, with error in code"""
      self.doTest(
         "TestingErroneousRule",
         "This rule is just for testing",
         ["import time",
                     "",
                     "def yMain(yApi):",
                     "   while(True)",
                     "      print 'location = ', yApi.getInfo(yApi.kLatitude)",
                     "      time.sleep(5)"],
         lambda ruleName, ruleDescription, ruleCode: self.checkCreateErroneousRule(ruleName, ruleDescription, ruleCode))

         
      
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
      Select(editRuleModal.getInterpreterSelector(self.browser)).select_by_value('0') # Select Python interpreter
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
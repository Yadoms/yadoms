import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import database
import scripts
import yadomsServer
import dashboard
import dashboard.automation
import notification
import i18n
import tools

class EditRule(unittest.TestCase):
   """Edit rule test"""
   
   def setUp(self):
      database.withOneStoppedRule()
      scripts.deploy(["DisplayLatitude"])
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
         
      
   def test_editStoppedRule(self):
      ruleNumber = 0
     
      # Edit the first rule
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).click()
      
      # Modify rule description
      ruleNewDescription = "This is the new rule description"
      editRuleModal = dashboard.automation.waitEditRuleModal(self.browser)
      editRuleModal.getRuleDescription(self.browser).clear()
      editRuleModal.getRuleDescription(self.browser).send_keys(ruleNewDescription)
      editRuleModal.getConfirmConfigureRuleButton(self.browser).click()
      
      # Check modified rule
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyUpdated"])
      
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, ruleNumber)
      self.assertEqual(ruleDatas[1].text, ruleNewDescription)
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Stopped)
      self.assertTrue(dashboard.automation.getRuleAutoStart(rulesTable, ruleNumber))
         
      
   def test_editRunningRule(self):
      ruleNumber = 0

      # Start the rule
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).click()
      WebDriverWait(self.browser, 10).until(lambda driver: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Running)
      
      # Edit the first rule
      dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).click()
      
      # Modify rule description
      ruleNewDescription = "This is the new rule description"
      editRuleModal = dashboard.automation.waitEditRuleModal(self.browser)
      editRuleModal.getRuleDescription(self.browser).clear()
      editRuleModal.getRuleDescription(self.browser).send_keys(ruleNewDescription)
      editRuleModal.getConfirmConfigureRuleButton(self.browser).click()
      
      # Check modified rule
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyUpdated"])
      
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, ruleNumber)
      self.assertEqual(ruleDatas[1].text, ruleNewDescription)
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Running)
      self.assertTrue(dashboard.automation.getRuleAutoStart(rulesTable, ruleNumber))
      
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
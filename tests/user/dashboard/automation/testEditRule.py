import sys
sys.path.append('.')
import unittest
from selenium import webdriver
import browser
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import database
import config
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
      yadomsServer.ensureStopped()
      database.deploy('OneStoppedRule')
      config.deploy("nominal")
      scripts.deploy(["DisplayServerVersion"])
      self.serverProcess = yadomsServer.start()
      self.browser = browser.getBrowser()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
         
      
   def test_editStoppedRule(self):
      print '=== Edit of a stopped rule test ==='
      ruleNumber = 0
     
      # Edit the first rule
      print 'Open the rule edit modal'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).click()
      
      print 'Change rule description'
      ruleNewDescription = "This is the new rule description"
      editRuleModal = dashboard.automation.waitEditRuleModal(self.browser)
      editRuleModal.setRuleDescription(ruleNewDescription)
      editRuleModal.ok()
      
      print 'Check modified rule'
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyUpdated"])
      
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, ruleNumber)
      tools.waitUntil(lambda: len(ruleDatas[1].text) > 0)
      self.assertEqual(ruleDatas[1].text, ruleNewDescription)
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Stopped)
      self.assertFalse(dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber))
         
      
   def test_editRunningRule(self):
      print '=== Edit of a running rule test ==='
      ruleNumber = 0

      print 'Start the rule'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).click()
      WebDriverWait(self.browser, 10).until(lambda browser: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Running)
      
      # Edit the first rule
      print 'Open the rule edit modal'
      dashboard.automation.getRuleEditButton(rulesTable, ruleNumber).click()
      
      # Modify rule description
      print 'Change rule description'
      ruleNewDescription = "This is the new rule description"
      editRuleModal = dashboard.automation.waitEditRuleModal(self.browser)
      editRuleModal.setRuleDescription(ruleNewDescription)
      editRuleModal.ok()
      
      # Check modified rule
      print 'Check modified rule'
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyUpdated"])
      
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, ruleNumber)
      tools.waitUntil(lambda: len(ruleDatas[1].text) > 0)
      self.assertEqual(ruleDatas[1].text, ruleNewDescription)
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Running)
      self.assertFalse(dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber))
      
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
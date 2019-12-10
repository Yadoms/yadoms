import sys
sys.path.append('.')
import unittest
from selenium import webdriver
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
import modals
import notification
import i18n
import tools

class RemoveRule(unittest.TestCase):
   """Remove rule test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneStoppedRule')
      config.deploy("nominal")
      scripts.deploy(["DisplayServerVersion"])
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      
      
   def initialConditionsForRemoveStoppedRuleTest(self, rulesTable, ruleNumber):
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Stopped)
   
   def test_removeStoppedRule(self):
      print ('=== Remove of a stopped rule test ===')
      self.doTest_removeRule(lambda rulesTable, ruleNumber: self.initialConditionsForRemoveStoppedRuleTest(rulesTable, ruleNumber))
      
      
   def initialConditionsForRemoveRunningRuleTest(self, rulesTable, ruleNumber):
      tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).click()
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled()))
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Running))
   
   def test_removeRunningRule(self):
      print ('=== Remove of a running rule test ===')
      self.doTest_removeRule(lambda rulesTable, ruleNumber: self.initialConditionsForRemoveRunningRuleTest(rulesTable, ruleNumber))
      
      
   def doTest_removeRule(self, initialConditionsFct):
      print ('  Open rules dashboard')
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      ruleNumber = 0

      print ('  Get rule table')
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      removeButton = dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber)
      
      initialConditionsFct(rulesTable, ruleNumber)
      
      print ('  Remove rule')
      removeButton.click()
      confirmationModal = modals.waitOkCancelModal(self.browser)
      confirmationModal.ok()
      
      print ('  Expect notification')
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleDeleted"])
      print ('  Check table was updated')
      self.assertTrue(tools.waitUntil(lambda: dashboard.automation.getRuleNumberInTable(self.browser, rulesTable) == 0, 5))
  
      
      
   def test_dontConfirmRemoveRule(self):
      print ('=== Don\'t confirm remove rule test ===')

      # Wait startup notifications are closed
      self.assertTrue(tools.waitUntil(lambda: notification.noNotification(self.browser)))

      print ('  Open rules dashboard')
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      ruleNumber = 0

      print ('  Get rule table')
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      removeButton = dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber)
      
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Stopped)
      
      print ('  Remove rule')
      removeButton.click()
      confirmationModal = modals.waitOkCancelModal(self.browser)
      print ('  Cancel')
      confirmationModal.cancel()
      
      print ('  Expect no notification')
      self.assertTrue(notification.noNotification(self.browser))
      print ('  Expect no change in rule table')
      self.assertEqual(dashboard.automation.getRuleNumberInTable(self.browser, rulesTable), 1)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
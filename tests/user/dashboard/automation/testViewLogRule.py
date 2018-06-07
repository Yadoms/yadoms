import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import re
import database
import config
import modals
import scripts
import yadomsServer
import dashboard
import dashboard.automation
import tools

class ViewLogRule(unittest.TestCase):
   """View log rule test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneStoppedRule')
      config.deploy("nominal")
      scripts.deploy(["DisplayServerVersion"])
      self.serverProcess = yadomsServer.start(["logLevel=information"])
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      
   def test_viewLogStoppedRule(self):
      print '=== view log of stopped rule test ==='

      print '  Open rules dashboard'
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      ruleNumber = 0

      print '  Start and stop the rule (to have some log)'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).click()
      WebDriverWait(self.browser, 10).until(lambda browser: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Running)
      tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).click()
      WebDriverWait(self.browser, 10).until(lambda browser: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Stopped)
      
      print '  View log'
      dashboard.automation.getRuleLogButton(rulesTable, ruleNumber).click()

      print '  Check log'
      logModal = dashboard.automation.waitLogRuleModal(self.browser)
      self.assertIsNotNone(re.match(r'.*#### START ####.*', logModal.getLogContent(), re.DOTALL))

      print '  Clear log'
      logModal.getClearButton().click()
      confirmationModal = modals.waitOkCancelModal(self.browser)
      confirmationModal.ok()

      print '  Reopen log modal'
      dashboard.automation.getRuleLogButton(rulesTable, ruleNumber).click()
      self.assertIsNone(re.match(r'.*#### START ####.*', logModal.getLogContent(), re.DOTALL))

      print '  Close log modal'
      logModal.getConfirmButton().click()
      
      
   def test_viewLogRunningRule(self):
      print '=== view log of running rule test ==='

      print '  Open rules dashboard'
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      ruleNumber = 0

      print '  Start the rule'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).is_enabled())
      dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber).click()
      WebDriverWait(self.browser, 10).until(lambda browser: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Running)
      
      print '  View log'
      dashboard.automation.getRuleLogButton(rulesTable, ruleNumber).click()

      print '  Check log'
      logModal = dashboard.automation.waitLogRuleModal(self.browser)
      self.assertIsNotNone(re.match(r'.*#### START ####.*', logModal.getLogContent(), re.DOTALL))

      print '  Clear log'
      logModal.getClearButton().click()
      confirmationModal = modals.waitOkCancelModal(self.browser)
      confirmationModal.ok()

      print '  Reopen log modal'
      dashboard.automation.getRuleLogButton(rulesTable, ruleNumber).click()
      self.assertIsNone(re.match(r'.*#### START ####.*', logModal.getLogContent(), re.DOTALL))

      print '  Close log modal'
      logModal.getConfirmButton().click()

   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
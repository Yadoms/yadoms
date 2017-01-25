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
import tools

class StartStopRule(unittest.TestCase):
   """Start/Stop rule test"""
   
   def setUp(self):
      database.deploy('OneStoppedRule')
      config.deploy("nominal")
      scripts.deploy(["DisplayServerVersion"])
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      
   def test_startStopRule(self):
      print '=== start/stop rule test ==='

      print '  Open rules dashboard'
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      ruleNumber = 0

      print '  Get rule table'
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      startStopButton = dashboard.automation.getRuleStartStopButton(rulesTable, ruleNumber)
      
      self.assertEqual(dashboard.automation.getRuleState(rulesTable, ruleNumber), dashboard.automation.RuleState.Stopped)
      
      self.assertFalse(dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber))
      
      print '  Start rule'
      self.assertTrue(tools.waitUntil(lambda: startStopButton.is_enabled()))
      startStopButton.click()
      WebDriverWait(self.browser, 10).until(lambda driver: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Running and startStopButton.is_enabled())

      print '  Stop rule'
      self.assertTrue(tools.waitUntil(lambda: startStopButton.is_enabled()))
      startStopButton.click()
      WebDriverWait(self.browser, 10).until(lambda driver: dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Stopped and startStopButton.is_enabled())
            
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
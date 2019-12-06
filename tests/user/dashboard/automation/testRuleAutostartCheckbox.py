import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
import database
import config
import yadomsServer
import dashboard
import dashboard.automation
import tools

class AutostartCheckboxRule(unittest.TestCase):
   """Rule autostart checkbox test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneStoppedRule')
      config.deploy("nominal")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      

   def test_autostartCheckbox(self):
      print ('=== Autostart Checkbox test ===')
      print ('ref Issues : #182')
      ruleNumber = 0
     
      print ('  Verify initial state')
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      self.assertFalse(dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber))
      
      print ('  Uncheck the box')
      dashboard.automation.getRuleAutoStart(rulesTable, ruleNumber).click()
      tools.waitUntil(lambda: dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber) is True)

      print ('  Reload page and check box state')
      dashboard.openSummary(self.browser)
      dashboard.openAutomation(self.browser)
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      tools.waitUntil(lambda: dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber) is True)
      
      print ('  Check the box')
      dashboard.automation.getRuleAutoStart(rulesTable, ruleNumber).click()
      tools.waitUntil(lambda: dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber) is False)

      print ('  Reload page and check box state')
      dashboard.openAutomation(self.browser)
      tools.waitUntil(lambda: dashboard.automation.getRuleAutoStartState(rulesTable, ruleNumber) is False)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
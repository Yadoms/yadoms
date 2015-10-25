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

class StartStopRule(unittest.TestCase):
   """Start/Stop rule test"""
   
   def setUp(self):
      database.withOneStoppedRule()
      scripts.deploy(["DisplayLatitude"])
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.get("http://127.0.0.1:8080")
      
      
   def test_startStopRule(self):
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)

      # Get rule table
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, 0)
      startStopButton = dashboard.automation.getRuleButtons(ruleDatas[2])[0]
      ruleStateCell = ruleDatas[3]
      
      assert dashboard.automation.getRuleState(ruleStateCell) is dashboard.automation.RuleState.Stopped
      
      # Start rule
      startStopButton.click()
      WebDriverWait(self.browser, 10).until(lambda driver: dashboard.automation.getRuleState(ruleStateCell) is dashboard.automation.RuleState.Running)

      # Stop rule
      startStopButton.click()
      WebDriverWait(self.browser, 10).until(lambda driver: dashboard.automation.getRuleState(ruleStateCell) is dashboard.automation.RuleState.Stopped)
            
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
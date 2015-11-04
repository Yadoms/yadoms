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

class RemoveRule(unittest.TestCase):
   """Remove rule test"""
   
   def setUp(self):
      database.withOneStoppedRule()
      scripts.deploy(["DisplayLatitude"])
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      yadomsServer.openClient(self.browser)
      
      
   def test_removeStoppedRule(self):
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)
      ruleNumber = 0

      # Get rule table
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      removeButton = dashboard.automation.getRuleRemoveButton(rulesTable, ruleNumber)
      
      assert dashboard.automation.getRuleState(rulesTable, ruleNumber) is dashboard.automation.RuleState.Stopped
      
      #TODO ajouter la non confirmation
      
      # Remove rule
      removeButton.click()
      confirmationModal = dashboard.automation.waitRuleRemovingConfirmationModal(self.browser)
      dashboard.automation.getRuleRemovingConfirmationModalOkButton(confirmationModal).click()
      
      notification.wait(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleDeleted"])
      
      time.sleep(5)
      assert dashboard.automation.getRuleNumberInTable(self.browser, rulesTable) is 0

   
   
   #TODO ajouter un test test_removeRunningRule
   
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
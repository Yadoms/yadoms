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
import time

class CreateRule(unittest.TestCase):
   """Create rule test"""
   
   def setUp(self):
      database.new()
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.get("http://127.0.0.1:8080")
         
      
   def test_createRule(self):
      # Open rules dashboard
      dashboard.open(self.browser)
      dashboard.openAutomation(self.browser)

      # Create new rule
      dashboard.automation.getCreateRuleButton(self.browser).click()
      dashboard.automation.waitEditorSelectionModal(self.browser)
      dashboard.automation.getEditorSelectionButton(self.browser, "code").click()

      # Fill rule data
      ruleName = "TestingRule"
      ruleDescription = "This rule is just for testing"
      ruleCode = ["import time",
                  "",
                  "def yMain(yApi):",
                  "   while(True):",
                  "      print 'location = ', yApi.getInfo(yApi.kLatitude)",
                  "      time.sleep(5)"];

      # - Rule configuration
      dashboard.automation.waitEditRuleModal(self.browser)
      Select(dashboard.automation.getInterpreterSelector(self.browser)).select_by_value('0') # Select Python interpreter
      dashboard.automation.getRuleName(self.browser).send_keys(ruleName)
      dashboard.automation.getRuleDescription(self.browser).send_keys(ruleDescription)
         
      # - Rule code
      dashboard.automation.getRuleCodeEditor(self.browser).writeCode(ruleCode)


      # Click OK
      dashboard.automation.getConfirmConfigureRuleButton(self.browser).click()
      
      
      # Check created rule

      # - in web client
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, 0)
         
      assert len(ruleDatas) is 4
#      assert ruleDatas[0].text is u'Activé'    ## TODO language-dependent
      assert ruleDatas[1].text == ruleName
      assert ruleDatas[2].text == ruleDescription
      ## TODO contrôler bouton
      
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleById(1, ruleCode)
      
      

            
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
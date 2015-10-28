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

class CreateRule(unittest.TestCase):
   """Create rule test"""
   
   def setUp(self):
      database.new()
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      yadomsServer.openClient(self.browser)

         
      
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

      # - notification
      notification.wait(self.browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["automation-center"]["ruleSuccessfullyCreated"])
      
      
      # - in web client
      rulesTable = dashboard.automation.waitRulesTableHasNRules(self.browser, 1)
      ruleDatas = dashboard.automation.getRuleDatas(rulesTable, 0)
         
      assert len(ruleDatas) is 4
      assert ruleDatas[0].text == ruleName
      assert ruleDatas[1].text == ruleDescription

      buttons = dashboard.automation.getRuleButtons(ruleDatas[2])
      assert len(buttons) is 3
      assert buttons[0].get_attribute("class") == "btn btn-enableDisable btn-warning"
      assert buttons[1].get_attribute("class") == "btn btn-edit btn-primary"
      assert buttons[2].get_attribute("class") == "btn btn-delete btn-danger"

      assert "label-success" in ruleDatas[3].find_element_by_class_name("label-status").get_attribute("class")
      
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleById(1, ruleCode)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
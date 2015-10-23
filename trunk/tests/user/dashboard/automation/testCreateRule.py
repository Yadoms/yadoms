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
import dashboard.tools
import time

class CreateRule(unittest.TestCase):
   """Create rule test"""
   
   def setUp(self):
      database.new()
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.get("http://127.0.0.1:8080")
      time.sleep(3) # TODO pas trouvé mieux pour l'instant
         
      
   def test_createRule(self):
      # Enter rules dashboard
      dashboard.tools.enterDashboard(self.browser)
      dashboard.tools.selectAutomation(self.browser)

      # Create new rule
      self.browser.find_element_by_id("btn-add-new-automation-rule").click()
      editorButtonsContainer = WebDriverWait(self.browser, 10).until(Condition.visibility_of_element_located((By.CLASS_NAME, "editor-btns")))
      editorButtons = editorButtonsContainer.find_elements_by_xpath("./child::*")
      codeButtonFound = False
      for editorButton in editorButtons:
         if (editorButton.find_element_by_tag_name("span").get_attribute("id") == "code"):
            editorButton.click()
            codeButtonFound = True
            break;
      assert codeButtonFound
      
      # Fill rule data
      ruleName = "TestingRule"
      ruleDescription = "This rule is just for testing"
      ruleCode = ["def yMain(yApi):",
                  "   while(True):",
                  "      print 'location = ', yApi.getInfo(yApi.kLatitude)"];

      # - Rule configuration
      ruleConfiguration = WebDriverWait(self.browser, 10).until(Condition.visibility_of_element_located((By.ID, "automation-rule-configuration")))
      controlGroups = ruleConfiguration.find_elements_by_xpath("./child::*")
      for controlGroup in controlGroups:
         label = controlGroup.find_element_by_class_name("configuration-label")
         control = controlGroup.find_element_by_class_name("configuration-control")
         
         controlContents = control.find_elements_by_tag_name("select")
         if (len(controlContents) == 0):
            controlContents = control.find_elements_by_tag_name("input")
         if (len(controlContents) == 0):
            print ('Unknown control type found')
            assert False
            
         controlContent = controlContents[0]
       
         if (controlContent.get_attribute("data-i18n") == '[data-content]modals.edit-automation-rule.interpreter-rule.description'):
            # Interpreter selection
            select = Select(controlContent)
            select.select_by_value('0') # Select Python interpreter

         elif (controlContent.get_attribute("data-i18n") == '[data-content]modals.edit-automation-rule.name-rule.description'):
            # Rule name
            controlContent.send_keys(ruleName)
            
         elif (controlContent.get_attribute("data-i18n") == '[data-content]modals.edit-automation-rule.description-rule.description'):
            # Rule description
            controlContent.send_keys(ruleDescription)

         else:
            print ('Unknown control found')
            assert False
         
         
      # - Rule code
      ruleEditor = self.browser.find_element_by_class_name("ace_text-input")
      self.writeCode(ruleCode, ruleEditor)


      # Click OK
      self.browser.find_element_by_id("btn-confirm-configure-rule").click()
      
      # Check created rule

      # - in web client
      rulesTable = WebDriverWait(self.browser, 10).until(Condition.visibility_of_element_located((By.ID, "automation-rule-list")))
      waiter = WebDriverWait(self.browser, 10)
      waiter.until(lambda driver: len(rulesTable.find_elements_by_tag_name("tr")) == 2)

      rule = rulesTable.find_elements_by_tag_name("tr")[1]
      ruleDatas = rule.find_elements_by_tag_name("td")
      assert len(ruleDatas) is 4
#      assert ruleDatas[0].text is u'Activé'    ## TODO language-dependent
      assert ruleDatas[1].text == ruleName
      assert ruleDatas[2].text == ruleDescription
      ## TODO contrôler bouton
      
      
      # - on disk (corresponding script file)
      scripts.checkLocalRuleById(1, ruleCode)
      
      
   def writeCode(self, code, codeEditorWebElement):
      """ Write code into codeEditor """
   
      # Need a workaround for Selenium bug #1723 (Left parenthesis don't work, see https://code.google.com/p/selenium/issues/detail?id=1723)
      for codeLine in code:
         if ('(' in codeLine):
            subStrings = codeLine.split("(")
            for subString in subStrings[:-1]:
               codeEditorWebElement.send_keys(subString)
               codeEditorWebElement.send_keys(Keys.SHIFT + "9")
            codeEditorWebElement.send_keys(subStrings[-1])
            self.writeCR(codeLine, codeEditorWebElement)
         else:
            codeEditorWebElement.send_keys(codeLine)
            self.writeCR(codeLine, codeEditorWebElement)
      
      
   def writeCR(self, codeLine, codeEditorWebElement):
      """ Write a CR into codeEditor """
      """ As code is provided with its indentation, we have to rollback auto-indentation made by ACE editor """
      
      codeEditorWebElement.send_keys(Keys.ENTER)
      spacesPerIndentation = 3   ## 3 spaces for a tab
      indentationCount = (len(codeLine) - len(codeLine.lstrip(' '))) / spacesPerIndentation
      if (codeLine[-1] is ':'):
         # Rollback auto-indentation
         indentationCount += 1
      for indent in range(indentationCount):
         codeEditorWebElement.send_keys(Keys.BACKSPACE)
            
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import modals
import tools


""" Operations on automation dashboard page """

def getCreateRuleButton(browser):
   return browser.find_element_by_id("btn-add-new-automation-rule")
   
def waitNewRuleModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, 'automation-rule-new-modal')))
   modals.waitForOpened(browser.find_element_by_id('automation-rule-new-modal'))
   return NewRuleModal(browser.find_element_by_id('automation-rule-new-modal'))
   
      
def waitRulesTable(browser):
   return WebDriverWait(browser, 10).until(Condition.presence_of_element_located((By.ID, "automation-rule-list")))

def waitRulesTableHasNRules(browser, rulesNumberExpected):
   rulesTable = waitRulesTable(browser)
   tools.waitUntil(lambda: getRuleNumberInTable(browser, rulesTable) == rulesNumberExpected)
   return rulesTable

def getRuleNumberInTable(browser, rulesTable):
   return len(rulesTable.find_elements_by_tag_name("tr")) - 1
   
def getRuleDatas(rulesTable, ruleNumber):
   rule = rulesTable.find_elements_by_tag_name("tr")[ruleNumber + 1]
   return rule.find_elements_by_tag_name("td")

def getRuleName(rulesTable, ruleNumber):
   return getRuleDatas(rulesTable, ruleNumber)[0].text

def getRuleDescription(rulesTable, ruleNumber):
   return getRuleDatas(rulesTable, ruleNumber)[1].text
   
def getRuleAutoStart(rulesTable, ruleNumber):
   return getRuleDatas(rulesTable, ruleNumber)[2].find_element_by_tag_name("input")
   
def getRuleAutoStartState(rulesTable, ruleNumber):
   return getRuleAutoStart(rulesTable, ruleNumber).is_selected()
   
def getRuleButtons(rulesTable, ruleNumber):
   rulesDataButtonsCell = getRuleDatas(rulesTable, ruleNumber)[3]
   buttonsGroup = rulesDataButtonsCell.find_element_by_class_name("btn-group")
   return buttonsGroup.find_elements_by_xpath("./child::*")
   
def getRuleButton(rulesTable, ruleNumber, index):
   buttons = getRuleButtons(rulesTable, ruleNumber)
   return buttons[index]
   
def getRuleStartStopButton(rulesTable, ruleNumber):
   """ Start/Stop button is the first button of the buttons group """
   button = getRuleButton(rulesTable, ruleNumber, 0)
   assert "btn-startStop" in button.get_attribute("class")
   return button
   
def getRuleEditButton(rulesTable, ruleNumber):
   """ Edit button is the second button of the buttons group """
   button = getRuleButton(rulesTable, ruleNumber, 1)
   assert "btn-edit" in button.get_attribute("class")
   return button
   
def getRuleLogButton(rulesTable, ruleNumber):
   """ Log button is the third button of the buttons group """
   button = getRuleButton(rulesTable, ruleNumber, 2)
   assert "btn-edit" in button.get_attribute("class")
   return button
   
def getRuleRemoveButton(rulesTable, ruleNumber):
   """ Remove button is the 4th button of the buttons group """
   button = getRuleButton(rulesTable, ruleNumber, 3)
   assert "btn-delete" in button.get_attribute("class")
   return button
   

def waitEditRuleModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, 'edit-automation-rule-modal')))
   modals.waitForOpened(browser.find_element_by_id('edit-automation-rule-modal'))
   return EditRuleModal(browser.find_element_by_id('edit-automation-rule-modal'))
   
   
   
class RuleState:
   Stopped, Running, Error = range(3)
   
def getRuleState(rulesTable, ruleNumber):
   ruleStateCell = getRuleDatas(rulesTable, ruleNumber)[4]
   classes = ruleStateCell.find_element_by_class_name('label-status').get_attribute('class')
   if 'label-warning' in classes:
      return RuleState.Stopped
   if 'label-success' in classes:
      return RuleState.Running
   if 'label-danger' in classes:
      return RuleState.Error
   print 'Not expected class for label-status : ', classes
   assert False   

   
   
class NewRuleModal():
   """ Operations on new rule modal (interpreter selection) """
   
   def __init__(self, newRuleModalWebElement):
       self.__newRuleModalWebElement = newRuleModalWebElement

   def getEditorSelectionButton(self, expectedButtonId):
      editorButtonsContainer = WebDriverWait(self.__newRuleModalWebElement, 10).until(Condition.visibility_of_element_located((By.CLASS_NAME, "editor-btns")))
      editorButtons = editorButtonsContainer.find_elements_by_xpath("./child::*")
      codeButtonFound = False
      for editorButton in editorButtons:
         if (editorButton.find_element_by_tag_name("span").get_attribute("id") == expectedButtonId):
            return editorButton
      assert False
   



class EditRuleModal():
   """ Operations on rule edition modal """
   
   def __init__(self, editRuleModalWebElement):
      self.__editRuleModalWebElement = editRuleModalWebElement
      self.__configurationPanel = ConfigurationPanel(self.__editRuleModalWebElement)

   def getRuleName(self):
      return self.__configurationPanel.getItemByName('modals.edit-automation-rule.name-rule.name')

   def setRuleName(self, newName):
      field = self.getRuleName()
      field.send_keys(Keys.CONTROL + 'a')
      field.send_keys(Keys.DELETE)
      field.send_keys(newName)

   def getRuleDescription(self):
      return self.__configurationPanel.getItemByName('modals.edit-automation-rule.description-rule.name')

   def setRuleDescription(self, newDescription):
      field = self.getRuleDescription()
      field.send_keys(Keys.CONTROL + 'a')
      field.send_keys(Keys.DELETE)
      field.send_keys(newDescription)

   def getRuleCodeEditor(self):
      return AceCodeEditor(self.__editRuleModalWebElement.find_element_by_class_name("ace_text-input"))
         
   def getConfirmButton(self):
      return self.__editRuleModalWebElement.find_element_by_id("btn-confirm-configure-rule")

   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__editRuleModalWebElement)
      


class AceCodeEditor:
   """ Class used to deal with an ACE editor web element """
   
   def __init__(self, codeEditorWebElement):
      self.__codeEditorWebElement = codeEditorWebElement
          
   def clear(self):
      """ Remove the existing code """
   
      tools.waitReadyForInput(self.__codeEditorWebElement)
      
      self.__codeEditorWebElement.send_keys(Keys.CONTROL + "a")
      self.__codeEditorWebElement.send_keys(Keys.DELETE)
      
      
   def writeCode(self, code):
      """ Write code into codeEditor """
      """ code must be provided as list of code lines """

      tools.waitReadyForInput(self.__codeEditorWebElement)
   
      # Need a workaround for Selenium bug #1723 (Left parenthesis don't work, see https://code.google.com/p/selenium/issues/detail?id=1723)
      for codeLine in code:
         self.__codeEditorWebElement.send_keys(codeLine)
         self.__writeCR(codeLine)
      
      
   def __writeCR(self, codeLine):
      """ Write a CR into codeEditor """
      """ As code is provided with its indentation, we have to rollback auto-indentation made by ACE editor """
      
      self.__codeEditorWebElement.send_keys(Keys.ENTER)
      spacesPerIndentation = 3   ## 3 spaces for a tab
      indentationCount = (len(codeLine) - len(codeLine.lstrip(' '))) / spacesPerIndentation
      if (len(codeLine) > 0 and codeLine[-1] is ':'):
         # Rollback auto-indentation
         indentationCount += 1
      for indent in range(indentationCount):
         self.__codeEditorWebElement.send_keys(Keys.BACKSPACE)
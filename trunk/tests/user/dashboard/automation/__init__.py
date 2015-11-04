from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys


""" Operations on main dashboard page """

def getCreateRuleButton(browser):
   return browser.find_element_by_id("btn-add-new-automation-rule")
      
def waitRulesTable(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "automation-rule-list")))
   return browser.find_element_by_id("automation-rule-list")

def waitRulesTableHasNRules(browser, rulesNumberExpected):
   rulesTable = waitRulesTable(browser)
   WebDriverWait(browser, 10).until(lambda driver: getRuleNumberInTable(browser, rulesTable) == rulesNumberExpected)
   return rulesTable

def getRuleNumberInTable(browser, rulesTable):
   return len(rulesTable.find_elements_by_tag_name("tr")) - 1
   
def getRuleDatas(rulesTable, ruleNumber):
   rule = rulesTable.find_elements_by_tag_name("tr")[ruleNumber + 1]
   return rule.find_elements_by_tag_name("td")
   
def getRuleButtons(rulesDataButtonsCell):
   buttonsGroup = rulesDataButtonsCell.find_element_by_class_name("btn-group")
   return buttonsGroup.find_elements_by_xpath("./child::*")
   
def getRuleButton(rulesDataButtonsCell, index):
   buttons = getRuleButtons(rulesDataButtonsCell)
   return buttons[index]
   
def getRuleStartStopButton(rulesTable, ruleNumber):
   """ Start/Stop button is the first button of the buttons group """
   button = getRuleButton(getRuleDatas(rulesTable, ruleNumber)[2], 0)
   assert "btn-enableDisable" in button.get_attribute("class")
   return button
   
def getRuleEditButton(rulesTable, ruleNumber):
   """ Edit button is the second button of the buttons group """
   button = getRuleButton(getRuleDatas(rulesTable, ruleNumber)[2], 1)
   assert "btn-edit" in button.get_attribute("class")
   return button
   
def getRuleRemoveButton(rulesTable, ruleNumber):
   """ Remove button is the third button of the buttons group """
   button = getRuleButton(getRuleDatas(rulesTable, ruleNumber)[2], 2)
   assert "btn-delete" in button.get_attribute("class")
   return button
   
   
class RuleState:
   Stopped, Running, Error = range(3)
   
def getRuleState(rulesTable, ruleNumber):
   ruleStateCell = getRuleDatas(rulesTable, ruleNumber)[3]
   if ("label-warning" in ruleStateCell.find_element_by_class_name("label-status").get_attribute("class")):
      return RuleState.Stopped
   if ("label-success" in ruleStateCell.find_element_by_class_name("label-status").get_attribute("class")):
      return RuleState.Running
   if ("label-danger" in ruleStateCell.find_element_by_class_name("label-status").get_attribute("class")):
      return RuleState.Error
   assert False   
      
      
      
""" Operations on new rule modal (interpreter selection) """

def waitEditorSelectionModal(browser):
   editorButtonsContainer = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "automation-rule-new-modal")))
   return browser.find_element_by_id("automation-rule-new-modal")


def getEditorSelectionButton(browser, expectedButtonId):
   editorButtonsContainer = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.CLASS_NAME, "editor-btns")))
   editorButtons = editorButtonsContainer.find_elements_by_xpath("./child::*")
   codeButtonFound = False
   for editorButton in editorButtons:
      if (editorButton.find_element_by_tag_name("span").get_attribute("id") == expectedButtonId):
         return editorButton
   assert False

   
""" Operations on delete rule confirmation modal """

def waitRuleRemovingConfirmationModal(browser):
   editorButtonsContainer = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "confirmation-modal")))
   return browser.find_element_by_id("confirmation-modal")
   
def getRuleRemovingConfirmationModalText(modal):
   return modal.find_element_by_class_name("modal-body").text

def getRuleRemovingConfirmationModalOkButton(modal):
   footer = modal.find_element_by_class_name("modal-footer")
   return footer.find_element_by_class_name("btn-confirm")
   
def getRuleRemovingConfirmationModalCancelButton(modal):
   footer = modal.find_element_by_class_name("modal-footer")
   return footer.find_element_by_class_name("btn-cancel")
   

""" Operations on rule edition modal """

def waitEditRuleModal(browser):
   editorButtonsContainer = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "edit-automation-rule-modal")))
   return browser.find_element_by_id("edit-automation-rule-modal")
   

def waitEditRuleModal(browser):
   editorButtonsContainer = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "edit-automation-rule-modal")))
   return browser.find_element_by_id("edit-automation-rule-modal")
   

def getConfigurationItem(browser, configurationPanel, dataI18nString):
   """ Find a configuration item by its "data-i18n" field """
   controlGroups = configurationPanel.find_elements_by_class_name("control-group")
   for controlGroup in controlGroups:
      control = controlGroup.find_element_by_class_name("configuration-control")

      controlContents = control.find_elements_by_xpath("./child::*")
      assert len(controlContents) > 0
      controlContent = controlContents[0] # Can be select, input, etc...
      
      if (controlContent.get_attribute("data-i18n") == dataI18nString):
         return controlContent
            
   # Not found
   assert False
      

def getInterpreterSelector(browser):
   ruleConfigurationPanel = browser.find_element_by_id("automation-rule-configuration")
   return getConfigurationItem(browser, ruleConfigurationPanel, "[data-content]modals.edit-automation-rule.interpreter-rule.description")

def getRuleName(browser):
   ruleConfigurationPanel = browser.find_element_by_id("automation-rule-configuration")
   return getConfigurationItem(browser, ruleConfigurationPanel, "[data-content]modals.edit-automation-rule.name-rule.description")

def getRuleDescription(browser):
   ruleConfigurationPanel = browser.find_element_by_id("automation-rule-configuration")
   return getConfigurationItem(browser, ruleConfigurationPanel, "[data-content]modals.edit-automation-rule.description-rule.description")

def getRuleCodeEditor(browser):
   return AceCodeEditor(browser.find_element_by_class_name("ace_text-input"))
      
def getConfirmConfigureRuleButton(browser):
   return browser.find_element_by_id("btn-confirm-configure-rule")


class AceCodeEditor:
   """ Class used to deal with an ACE editor web element """
   
   def __init__(self, codeEditorWebElement):
       self.__codeEditorWebElement = codeEditorWebElement
          
   def writeCode(self, code):
      """ Write code into codeEditor """
      """ code must be provided as list of code lines """
   
      # Need a workaround for Selenium bug #1723 (Left parenthesis don't work, see https://code.google.com/p/selenium/issues/detail?id=1723)
      for codeLine in code:
         if ('(' in codeLine):
            subStrings = codeLine.split("(")
            for subString in subStrings[:-1]:
               self.__codeEditorWebElement.send_keys(subString)
               self.__codeEditorWebElement.send_keys(Keys.SHIFT + "9")
            self.__codeEditorWebElement.send_keys(subStrings[-1])
            self.__writeCR(codeLine)
         else:
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
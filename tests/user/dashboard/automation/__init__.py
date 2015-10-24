from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition



def getCreateRuleButton(browser):
   return browser.find_element_by_id("btn-add-new-automation-rule")
      
      
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

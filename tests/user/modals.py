﻿from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
import tools

      
def waitForOpened(modalWebElement):
   assert tools.waitUntil(lambda: 'display: block;' in modalWebElement.get_attribute('style'))

def waitForClosed(modalWebElement):
   def l(modalWebElement):
      try:
         return 'display: none;' in modalWebElement.get_attribute('style')
      except Condition.StaleElementReferenceException:
         return True
   assert tools.waitUntil(lambda: l(modalWebElement))

class OkCancelModal():
   """ Operations on delete object (plugins, rule...) confirmation modal """
   
   def __init__(self, removeObjectConfirmationModalWebElement):
       self.__removeObjectConfirmationModalWebElement = removeObjectConfirmationModalWebElement
   
   def getText(self):
      return self.__removeObjectConfirmationModalWebElement.find_element_by_class_name("modal-body").text

   def getOkButton(self):
      footer = self.__removeObjectConfirmationModalWebElement.find_element_by_class_name("modal-footer")
      return footer.find_element_by_class_name("btn-confirm")
      
   def getCancelButton(self):
      footer = self.__removeObjectConfirmationModalWebElement.find_element_by_class_name("modal-footer")
      return footer.find_element_by_class_name("btn-cancel")
      
   def ok(self):
      self.getOkButton().click()
      waitForClosed(self.__removeObjectConfirmationModalWebElement)
      
   def cancel(self):
      self.getCancelButton().click()
      waitForClosed(self.__removeObjectConfirmationModalWebElement)


def waitOkCancelModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, 'confirmation-modal')))
   waitForOpened(browser.find_element_by_id('confirmation-modal'))
   return OkCancelModal(browser.find_element_by_id('confirmation-modal'))


class PickupSelectorModal():
   """ Operations on a pickup selector object """
   
   def __init__(self, newItemModalWebElement):
       self.__newItemModalWebElement = newItemModalWebElement

   def selectItem(self, expectedItemName):
      select = WebDriverWait(self.__newItemModalWebElement, 10).until(Condition.visibility_of_element_located((By.ID, "ul-add-element")))
      for button in select.find_elements_by_tag_name('button'):
         if button.get_attribute('element-type') == expectedItemName:
            return button
      return None

   def getAddButton(self):
      return self.__newItemModalWebElement.find_element_by_xpath(".//button[@id='btn-confirm-add-element']")
         
   def getCancelButton(self):
      return self.__newItemModalWebElement.find_element_by_class_name('close')
      
   def clickAdd(self):
      self.getAddButton().click()
      waitForClosed(self.__newItemModalWebElement)
      
   def cancel(self):
      self.getCancelButton().click()
      waitForClosed(self.__newItemModalWebElement)
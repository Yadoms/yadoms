
import tools

      
def waitForOpened(modalWebElement):
   assert tools.waitUntil(lambda: 'display: block;' in modalWebElement.get_attribute('style'))

def waitForClosed(modalWebElement):
   assert tools.waitUntil(lambda: 'display: none;' in modalWebElement.get_attribute('style'))


class RemoveObjectConfirmationModal():
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
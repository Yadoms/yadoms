


class RemoveObjectConfirmationModal():
   """ Operations on delete object (plugind, rule...) confirmation modal """
   
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
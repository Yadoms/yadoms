


class RemoveObjectConfirmationModal():
   """ Operations on delete object (plugind, rule...) confirmation modal """
   
   def __init__(self, removeObjectConfirmationModalWebElement):
       self.__removeObjectConfirmationModalWebElement = removeObjectConfirmationModalWebElement
   
   def getConfirmationModalText(self):
      return self.__removeObjectConfirmationModalWebElement.find_element_by_class_name("modal-body").text

   def getConfirmationModalOkButton(self):
      footer = self.__removeObjectConfirmationModalWebElement.find_element_by_class_name("modal-footer")
      return footer.find_element_by_class_name("btn-confirm")
      
   def getConfirmationModalCancelButton(self):
      footer = self.__removeObjectConfirmationModalWebElement.find_element_by_class_name("modal-footer")
      return footer.find_element_by_class_name("btn-cancel")
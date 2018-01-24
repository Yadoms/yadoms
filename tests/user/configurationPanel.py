


class ConfigurationPanel():
   """ Class to get access to configuration panel fields """
   
   def __init__(self, panelWebElement):
      """ panelWebElement is the panel containing the configuration, or one of parents """
      self.__panelWebElement = panelWebElement 
      
   def __findField(self, dataI18nString):
      """ Find the configuration field """
      controlGroups = self.__panelWebElement.find_elements_by_class_name('control-group')
      for controlGroup in controlGroups:
         label = controlGroup.find_element_by_class_name('configuration-label')
         name = label.find_element_by_class_name('configuration-label-name')
         if dataI18nString in name.get_attribute('data-i18n'):
            return controlGroup
      # Not found
      assert False  
      
   def __findSection(self, dataI18nString):
      """ Find the configuration section """
      sections = self.__panelWebElement.find_elements_by_class_name('configuration-section')
      for section in sections:
         header = section.find_element_by_class_name('configuration-header')
         span = header.find_element_by_tag_name('span')
         if dataI18nString in span.get_attribute('data-i18n'):
            return section
      # Not found
      assert False 

   def getFielsCount(self, recursive=False):
      # TOFIX
      return len(self.__panelWebElement.find_elements_by_class_name('control-group'))
      
   def getAddObjectButton(self, dataI18nString):
      """ Get the Add object button of a configuration section """
      return self.__findSection(dataI18nString).find_element_by_tag_name('button')
      
   def getItemByName(self, dataI18nString):
      """ Find a configuration single item by its "data-i18n" field """
      return self.__findField(dataI18nString).find_element_by_class_name('configuration-control').find_element_by_class_name('form-control')

   def getItemsByName(self, dataI18nString):
      """ Find list of configuration items by its "data-i18n" field """
      return self.__findField(dataI18nString).find_element_by_class_name('configuration-control').find_element_by_tag_name('div').find_elements_by_class_name('form-control')
               
      # Not found
      assert False  
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition



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
      
   def __findSection(self, dataI18nSectionName):
      """ Find the configuration section """
      labelNode = self.__panelWebElement.find_element_by_xpath(".//div[@class='control-group configuration-section well']//span[@data-i18n='" + dataI18nSectionName + "']")
      return labelNode.find_element_by_xpath(".//ancestor::div[@class='control-group configuration-section well']")
      
   def enableOptionalSection(self, dataI18nSectionName, enable=True):
      """ Active an optional configuration section """
      section = self.__findSection(dataI18nSectionName)
      checkBoxElement = section.find_element_by_xpath(".//div[@class='checkbox']/label/input[@type='checkbox']")
      isChecked = True if checkBoxElement.get_attribute('checked') is not None else False
      if enable != isChecked:
         checkBoxElement.click()
         WebDriverWait(section, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//div[contains(@class, 'section-content')]")))
      return section

   def getFielsCount(self, recursive=False):
      return len(self.__panelWebElement.find_elements_by_tag_name('input')) + \
         len(self.__panelWebElement.find_elements_by_tag_name('select')) + \
         len(self.__panelWebElement.find_elements_by_xpath('.//button[@role=\"iconpicker\"]'))
      
   def getAddObjectButton(self, dataI18nString):
      """ Get the Add object button of a configuration section """
      return self.__findSection(dataI18nString).find_element_by_tag_name('button')
      
   def getItemByName(self, dataI18nString):
      """ Find a configuration single item by its "data-i18n" field """
      return self.__findField(dataI18nString).find_element_by_class_name('configuration-control').find_element_by_class_name('form-control')
      
   def getCheckboxItemByName(self, dataI18nString):
      """ Find a configuration single checkbox by its "data-i18n" field """
      return self.__findField(dataI18nString).find_element_by_xpath(".//input[@type='checkbox']")

   def getItemsByName(self, dataI18nString):
      """ Find list of configuration items by its "data-i18n" field """
      return self.__findField(dataI18nString).find_element_by_class_name('configuration-control').find_element_by_tag_name('div').find_elements_by_class_name('form-control')
               
      # Not found
      assert False  
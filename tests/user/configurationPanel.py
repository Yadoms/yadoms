from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
import tools


class ConfigurationPanel(object):
   """ Class to get access to configuration panel fields """
   
   def __init__(self, panelWebElement):
      """ panelWebElement is the panel containing the configuration, or one of parents """
      self._panelWebElement = panelWebElement 
      
   def findField(self, dataI18nString):
      """ Find the configuration field """
      controlGroups = self._panelWebElement.find_elements_by_class_name('control-group')
      for controlGroup in controlGroups:
         label = controlGroup.find_element_by_class_name('configuration-label')
         name = label.find_element_by_class_name('configuration-label-name')
         if dataI18nString in name.get_attribute('data-i18n'):
            return controlGroup
      # Not found
      assert False  
      
   def findSection(self, dataI18nSectionName):
      """ Find the configuration section """
      labelNode = self._panelWebElement.find_element_by_xpath(".//div[@class='configuration-header']//*[@data-i18n='" + dataI18nSectionName + "']")
      return labelNode.find_element_by_xpath(".//ancestor::div[@class='control-group configuration-section well']")
      
   def enableOptionalSection(self, dataI18nSectionName, enable=True):
      """ Active an optional configuration section """
      section = self.findSection(dataI18nSectionName)
      checkBoxElement = WebDriverWait(section, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//div[@class='checkbox']/label/input[@type='checkbox']")))
      isChecked = True if checkBoxElement.get_attribute('checked') is not None else False
      if enable != isChecked:
         checkBoxElement.click()
         WebDriverWait(section, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//div[contains(@class, 'section-content')]")))
      return section

   def isOptionalSectionEnabled(self, dataI18nSectionName):
      """ Check if optional configuration section is enabled """
      section = self.findSection(dataI18nSectionName)
      checkBoxElement = WebDriverWait(section, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//div[@class='checkbox']/label/input[@type='checkbox']")))
      return True if checkBoxElement.get_attribute('checked') is not None else False

   def getFielsCount(self, recursive=False):
      return len(self._panelWebElement.find_elements_by_tag_name('input')) + \
         len(self._panelWebElement.find_elements_by_tag_name('select')) + \
         len(self._panelWebElement.find_elements_by_xpath('.//button[@role=\"iconpicker\"]'))
      
   def getButton(self, dataI18nString):
      """ Get a button of a configuration section """
      return self._panelWebElement.find_element_by_xpath(".//button[@data-i18n='" + dataI18nString + "']")
      
   def getItemButton(self):
      """ Get 'add item button' - when we add multiple devices, configuration, ... """
      return self._panelWebElement.find_element_by_class_name('pull-right')
      
   def getDuplicateButton(self):
      """ Get 'duplicate item button' - when we add multiple devices, configuration, ... """
      return self._panelWebElement.find_element_by_class_name('btn-duplicate')
      
   def getTextItemByName(self, dataI18nString):
      """ Find a configuration single text box by its "data-i18n" title """
      return self.findField(dataI18nString).find_element_by_xpath(".//input[@type='text']")
      
   def getCheckboxItemByName(self, dataI18nString):
      """ Find a configuration single checkbox by its "data-i18n" title """
      return self.findField(dataI18nString).find_element_by_xpath(".//input[@type='checkbox']")
      
   def getPasswordItemByName(self, dataI18nString):
      """ Find a password configuration field by its "data-i18n" title """
      return self.findField(dataI18nString).find_element_by_xpath(".//input[@type='password']")

   def getItemsByName(self, dataI18nString):
      """ Find list of configuration items by its "data-i18n" title """
      return self.findField(dataI18nString).find_element_by_class_name('controls').find_element_by_tag_name('div').find_elements_by_class_name('form-control')
   
   def getControlGroupParentItem(self, dataI18nString, position):
      """ Find configuration item list parent by its "data-i18n" title, and it's position """
      return self._panelWebElement.find_element_by_xpath("(//span[@data-i18n='" + dataI18nString + "'])[" + str(position) + "]").find_element_by_xpath("./../../..")

   def getPanel(self):
      """ Return panel """
      return self._panelWebElement
      
   def getItemListByNameAndPosition(self, dataI18nString, position):
      """ Find configuration item list by its "data-i18n" title, and it's position """
      return self.getControlGroupParentItem(dataI18nString, position).find_element_by_class_name('controls').find_element_by_tag_name('div').find_elements_by_class_name('form-control')
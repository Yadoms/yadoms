from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
from selenium.webdriver.support.select import Select
import modals
import i18n
import tools
import dashboard
import notification


""" Operations on devices dashboard page """

def getCreateDeviceButton(browser):
   return browser.find_element_by_id("btn-add-new-device")
   
def waitNewDeviceModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "new-manually-device-modal")))
   return NewDeviceModal(browser.find_element_by_id("new-manually-device-modal"))
   
      
def waitDevicesTable(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "device-list")))
   return browser.find_element_by_id("device-list")

def findDeviceInTable(devicesTable, expectedDeviceName):
   # return device ID or None if not found
   try:
      lines = devicesTable.find_elements_by_xpath('//tr[@class="device"]')
      for line in lines:
         deviceNameElem = line.find_element_by_class_name('device-friendlyName')
         if deviceNameElem is not None and deviceNameElem.find_elements_by_tag_name('span')[1].text == expectedDeviceName:
            return line.get_attribute("device-id")
   except:
      pass
   return None

def countDevices(devicesTable):
   # return the number of devices in the table
   return len(devicesTable.find_elements_by_xpath('//tr[@class="device"]'))

def waitDevicesTableHasDeviceNamed(browser, expectedDeviceName):
   devicesTable = waitDevicesTable(browser)
   assert tools.waitUntil(lambda: findDeviceInTable(devicesTable, expectedDeviceName) is not None)
   return findDeviceInTable(devicesTable, expectedDeviceName)
   
def getDeviceDatas(devicesTable, deviceId):
   return devicesTable.find_elements_by_xpath('//tr[@device-id="' + deviceId + '"]/td')

def getDeviceName(devicesTable, deviceId):
   return getDeviceDatas(devicesTable, deviceId)[0].find_elements_by_tag_name('span')[1].text

def getAttachedPlugin(devicesTable, deviceId):
   return getDeviceDatas(devicesTable, deviceId)[1].find_element_by_tag_name('span').text
   
def getDeviceButtons(devicesTable, deviceId):
   devicesActionsButtonsCell = getDeviceDatas(devicesTable, deviceId)[3]
   buttonsGroup = devicesActionsButtonsCell.find_element_by_class_name("btn-group")
   return buttonsGroup.find_elements_by_tag_name("button")
   
def getConfigureDeviceButton(devicesTable, deviceId):
   for button in getDeviceButtons(devicesTable, deviceId):
      if 'btn-configure' in button.get_attribute('class'):
         return button
   return None
   
def getRemoveDeviceButton(devicesTable, deviceId):
   for button in getDeviceButtons(devicesTable, deviceId):
      if 'btn-delete' in button.get_attribute('class'):
         return button
   return None


# Operations on keywords   

def getKeywords(devicesTable):
   deviceDetails = devicesTable.find_element_by_class_name('device-details')
   keywordTable = deviceDetails.find_element_by_id('keyword-list')
   tablerows = keywordTable.find_elements_by_tag_name('tr')
   if len(tablerows) == 0:
      return []
   tablerows.pop(0)
   return tablerows
   
def getDeployKeywordsButton(devicesTable, deviceId):
   return getDeviceDatas(devicesTable, deviceId)[0].find_element_by_class_name('deploy')

def getKeywordName(keywordRow):
   item = keywordRow.find_element_by_class_name('keyword-friendlyName')
   if item is None:
      return None
   return item.find_element_by_tag_name('span').text

def getKeywordBoolValue(keywordRow):
   try:
      items = keywordRow.find_elements_by_class_name('keyword-value')
      if len(items) == 0:
         return None
      item = items[0]
      inputElement = item.find_elements_by_tag_name('input')
      return True if inputElement[0].get_attribute('checked') is not None else False
   except:
      pass
   return None

def getKeywordTextValue(keywordRow):
   try:
      items = keywordRow.find_elements_by_class_name('keyword-value')
      if len(items) == 0:
         return None
      item = items[0]
      inputElement = item.find_elements_by_tag_name('span')
      return inputElement[0].text
   except:
      pass
   return None

def getKeywordUnit(keywordRow):
   item = keywordRow.find_element_by_class_name('keyword-units')
   if item is None:
      return None
   return item.find_element_by_tag_name('span').text

def getKeywordActions(keywordRow):
   item = keywordRow.find_element_by_class_name('keyword-actions')
   if item is None:
      return None
   return item.find_elements_by_tag_name('button')

def getConfigureKeywordButton(keywordRow):
   for button in getKeywordActions(keywordRow):
      if 'btn-configure' in button.get_attribute('class'):
         return button
   return None;

def getCommandKeywordButton(keywordRow):
   for button in getKeywordActions(keywordRow):
      if 'btn-setvalue' in button.get_attribute('class'):
         return button
   return None;

   

def waitConfigureManuallyDeviceModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "configure-manually-device-modal")))
   return ConfigureDeviceModal(browser.find_element_by_id("configure-manually-device-modal"))

def waitConfigureDeviceModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "configure-device-modal")))
   return ConfigureDeviceModal(browser.find_element_by_id("configure-device-modal"))

def waitRemoveDeviceConfirmationModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "device-delete-modal")))
   return RemoveDeviceConfirmationModal(browser.find_element_by_id("device-delete-modal"))

def waitSetValueKeywordModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "set-value-keyword-modal")))
   return SetValueKeywordModal(browser.find_element_by_id("set-value-keyword-modal"))

   
   
class NewDeviceModal():
   """ Operations on new device modal (attached plugin selection) """
   
   def __init__(self, newDeviceModalWebElement):
       self.__newDeviceModalWebElement = newDeviceModalWebElement

   def selectAttachedPlugin(self, pluginName):
      select = WebDriverWait(self.__newDeviceModalWebElement, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//select[@data-i18n='[data-content]modals.add-manually-device.addManuallyDeviceSection.content.attachedToPlugin.content.plugins.description']")))
      for option in select.find_elements_by_tag_name('option'):
         if option.text == pluginName:
            return option
      print "selectAttachedPlugin : Nothing to select, ", pluginName, " not found"
      assert False
         
   def getConfirmButton(self):
      return self.__newDeviceModalWebElement.find_element_by_id("btn-confirm-add-manually-device")
         
   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__newDeviceModalWebElement)
            
            
            

class RemoveDeviceConfirmationModal(modals.RemoveObjectConfirmationModal):
   """ Operations on delete device confirmation modal """
   pass
   



class ConfigureDeviceModal():
   """ Operations on device configuration modal """
   
   def __init__(self, configureDeviceModalWebElement):
      self.__configureDeviceModalWebElement = configureDeviceModalWebElement

   def getDeviceName(self):
      return ConfigurationPanel(self.__configureDeviceModalWebElement).getItemByName("modals.configure-manually-device.friendlyName-configuration.name")
      
   def setDeviceName(self, newName):
      tools.waitReadyForInput(self.getDeviceName())
      nameField = self.getDeviceName()
      nameField.send_keys(Keys.CONTROL + "a")
      nameField.send_keys(Keys.DELETE)
      nameField.send_keys(newName)

   def getDeviceModel(self):
      return ConfigurationPanel(self.__configureDeviceModalWebElement).getItemByName("modals.configure-manually-device.model-configuration.name")
      
   def setDeviceModel(self, newModel):
      tools.waitReadyForInput(self.getDeviceName())
      nameField = self.getDeviceModel()
      nameField.send_keys(Keys.CONTROL + "a")
      nameField.send_keys(Keys.DELETE)
      nameField.send_keys(newModel)

   def getConfigurationFieldsCount(self):
      return ConfigurationPanel(self.__configureDeviceModalWebElement).getFielsCount()

   def getTextField(self, field):
      return ConfigurationPanel(self.__configureDeviceModalWebElement).getItemByName(field).get_attribute('value')

   def updateTextField(self, field, value):
      field = ConfigurationPanel(self.__configureDeviceModalWebElement).getItemByName(field)
      tools.waitReadyForInput(field)
      field.send_keys(Keys.CONTROL + "a")
      field.send_keys(Keys.DELETE)
      field.send_keys(value)
         
   def getConfirmButton(self):
      return self.__configureDeviceModalWebElement.find_element_by_id("btn-confirm-configure-device")
         
   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__configureDeviceModalWebElement)
         
   def getCancelButton(self):
      return self.__configureDeviceModalWebElement.find_element_by_xpath(".//button[@data-i18n='common.cancel']")
         
   def cancel(self):
      self.getCancelButton().click()
      modals.waitForClosed(self.__configureDeviceModalWebElement)
   



class SetValueKeywordModal():
   """ Operations on set keyword value modal """
   
   def __init__(self, setValueKeywordModalWebElement):
      self.__setValueKeywordModalWebElement = setValueKeywordModalWebElement
      pass

   def __getValueElement(self):
      return self.__setValueKeywordModalWebElement.find_element_by_class_name('control-group')

   def getBoolValue(self):
      valueElement = self.__getValueElement()
      checkbox = valueElement.find_element_by_xpath(".//input[@type='checkbox']")
      return True if checkbox.get_attribute('checked') is not None else False
      
   def setBoolValue(self, newValue):
      if newValue == self.getBoolValue():
         return      
      valueElement = self.__getValueElement()
      checkbox = valueElement.find_element_by_xpath(".//input[@type='checkbox']")
      checkbox.click()

   def getEnumValue(self):
      valueElement = self.__getValueElement()
      options = valueElement.find_elements_by_tag_name('option')
      for option in options:
         if option.get_attribute('selected') is not None:
            return option.text
      assert False
      
   def setEnumValue(self, newValue):
      if newValue == self.getEnumValue():
         return      
      valueElement = self.__getValueElement()
      Select(valueElement.find_element_by_tag_name('select')).select_by_visible_text(newValue)
         
   def getCancelButton(self):
      return self.__setValueKeywordModalWebElement.find_element_by_id("btn-default")
         
   def getOkButton(self):
      return self.__setValueKeywordModalWebElement.find_element_by_id("btn-confirm-set-keyword-value")
         
   def cancel(self):
      self.getCancelButton().click()
      modals.waitForClosed(self.__setValueKeywordModalWebElement)
         
   def ok(self):
      self.getOkButton().click()
      modals.waitForClosed(self.__setValueKeywordModalWebElement)


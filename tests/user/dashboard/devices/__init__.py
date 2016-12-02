from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
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
   

def waitConfigureDeviceModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "configure-manually-device-modal")))
   return ConfigureDeviceModal(browser.find_element_by_id("configure-manually-device-modal"))

def waitRemoveDeviceConfirmationModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "device-delete-modal")))
   return RemoveDeviceConfirmationModal(browser.find_element_by_id("device-delete-modal"))

   
   
class NewDeviceModal():
   """ Operations on new device modal (attached plugin selection) """
   
   def __init__(self, newDeviceModalWebElement):
       self.__newDeviceModalWebElement = newDeviceModalWebElement

   def selectAttachedPlugin(self, pluginName):
      select = WebDriverWait(self.__newDeviceModalWebElement, 10).until(Condition.visibility_of_element_located((By.ID, "associatedPluginTypeList")))
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
         
   def getConfirmButton(self):
      return self.__configureDeviceModalWebElement.find_element_by_id("btn-confirm-configure-device")
         
   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__configureDeviceModalWebElement)


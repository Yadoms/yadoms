import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
import database
import config
import yadomsServer
import dashboard
import dashboard.devices
import tools
import i18n
import time

class RenameDevice(unittest.TestCase):
   """Rename device test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openDevice(self.browser)

   def test_renameDevice(self):
      print ('=== Rename a device ===')
      deviceName = u'configurableDevice'
      newDeviceName = u'myNewName'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print ('  Open the configure device modal')
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)

      print ('  Change device name then cancel')
      configureDeviceModal.updateTextField('friendlyName', newDeviceName)
      configureDeviceModal.cancel()
      self.assertTrue(tools.waitUntil(lambda: dashboard.devices.getDeviceName(devicesTable, deviceId) == deviceName))
      
      print ('  Change device name then confirm')
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      configureDeviceModal.updateTextField('friendlyName', newDeviceName)
      configureDeviceModal.ok()
      self.assertTrue(tools.waitUntil(lambda: dashboard.devices.getDeviceName(devicesTable, deviceId) == newDeviceName))

      print ('  Check change was saved')
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, newDeviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      self.assertEqual(configureDeviceModal.getTextField('Name'), newDeviceName)
      configureDeviceModal.cancel()
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
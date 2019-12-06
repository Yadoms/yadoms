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

class RenameVirtualDevice(unittest.TestCase):
   """Rename device test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneVirtualDeviceApparentPower')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openDevice(self.browser)

   def test_renameVirtualDevice(self):
      print ('=== Rename a virtual device ===')
      deviceName = u'My virtual device'
      newDeviceName = u'myNewName'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print ('  Open the configure device modal')
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureVirtualDeviceModal = dashboard.devices.waitConfigureVirtualDeviceModal(self.browser)

      print ('  Change virtual device name then cancel')
      configureVirtualDeviceModal.updateTextField('friendlyName', newDeviceName)
      configureVirtualDeviceModal.cancel()
      self.assertTrue(tools.waitUntil(lambda: dashboard.devices.getDeviceName(devicesTable, deviceId) == deviceName))
      
      print ('  Change virtual device name then confirm')
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureVirtualDeviceModal = dashboard.devices.waitConfigureVirtualDeviceModal(self.browser)
      configureVirtualDeviceModal.updateTextField('friendlyName', newDeviceName)
      configureVirtualDeviceModal.ok()
      self.assertTrue(tools.waitUntil(lambda: dashboard.devices.getDeviceName(devicesTable, deviceId) == newDeviceName))

      print ('  Check change was saved')
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, newDeviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureVirtualDeviceModal = dashboard.devices.waitConfigureVirtualDeviceModal(self.browser)
      self.assertEqual(configureVirtualDeviceModal.getTextField('Name'), newDeviceName)
      configureVirtualDeviceModal.cancel()
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
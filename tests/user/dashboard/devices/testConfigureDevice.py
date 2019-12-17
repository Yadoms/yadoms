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

class ConfigureDevice(unittest.TestCase):
   """Configure device test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome(options=tools.ChromeOptionsHelper.get())
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openDevice(self.browser)
      

   def test_notConfigurableDevice(self):
      print ('=== Try to configure not configurable device ===')
      deviceName = u'fakeDimmableReadOnlySwitch'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print ('  Check that device is not configurable')
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      self.assertEqual(configureDeviceModal.getConfigurationFieldsCount(), 2)
      configureDeviceModal.cancel()


   def test_configurableDevice(self):
      print ('=== Configure a device ===')
      deviceName = u'configurableDevice'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print ('  Check that device is configurable')
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      self.assertEqual(configureDeviceModal.getConfigurationFieldsCount(), 5)

      print ('  Start change device configuration then cancel')
      self.assertEqual(configureDeviceModal.getTextField('CounterDivider2'), '2')
      configureDeviceModal.updateTextField('CounterDivider2', '5')
      configureDeviceModal.cancel()

      print ('  Check, it s possible to open twice the configuration windows for a normal device')
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      configureDeviceModal.cancel()
      
      print ('  Start change device configuration then confirm')
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      self.assertEqual(configureDeviceModal.getTextField('CounterDivider2'), '2')
      configureDeviceModal.updateTextField('CounterDivider2', '5')
      configureDeviceModal.ok()

      print ('  Check change was saved')
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      self.assertEqual(configureDeviceModal.getTextField('CounterDivider2'), '5')
      configureDeviceModal.cancel()

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
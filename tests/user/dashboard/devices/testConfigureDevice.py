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
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openDevice(self.browser)
      

   def test_notConfigurableDevice(self):
      print '=== Try to configure not configurable device ==='
      deviceName = u'fakeDimmableReadOnlySwitch'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Check that device is not configurable'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      assert configureDeviceModal.getConfigurationFieldsCount() == 0
      configureDeviceModal.cancel()


   def test_configurableDevice(self):
      print '=== Configure a device ==='
      deviceName = u'fakeCounter'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Check that device is configurable'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      assert configureDeviceModal.getConfigurationFieldsCount() == 1

      print '  Start change device configuration then cancel'
      assert configureDeviceModal.getTextField('plugins/dev-fakePlugin:deviceConfiguration.CounterDivider2.name') == '2'
      configureDeviceModal.updateTextField('CounterDivider', '5')
      configureDeviceModal.cancel()
      
      print '  Start change device configuration then confirm'
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      assert configureDeviceModal.getTextField('CounterDivider') == '2'
      configureDeviceModal.updateTextField('CounterDivider', '5')
      configureDeviceModal.ok()

      print '  Check change was saved'
      dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId).click()
      configureDeviceModal = dashboard.devices.waitConfigureDeviceModal(self.browser)
      assert configureDeviceModal.getTextField('CounterDivider') == '5'
      configureDeviceModal.cancel()

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
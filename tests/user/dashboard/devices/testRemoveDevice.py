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

class RemoveDevice(unittest.TestCase):
   """Remove device test"""
   
   def setUp(self):
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openDevice(self.browser)
      

   def test_cancelRemoveDevice(self):
      print '=== Cancel at confirm step when removing device ==='
      deviceName = u'fakeDimmableReadOnlySwitch'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Remove the device'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getRemoveDeviceButton(devicesTable, deviceId).click()

      print '  Cancel confirm modal'
      confirmRemoveModal = dashboard.devices.waitRemoveDeviceConfirmationModal(self.browser)
      confirmRemoveModal.cancel()

      print '  Check nothing has changed'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      

   def test_confirmRemoveDevice(self):
      print '=== Remove device ==='
      deviceName = u'fakeDimmableReadOnlySwitch'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Remove the device'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      devicesOriginalCount = dashboard.devices.countDevices(devicesTable)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      dashboard.devices.getRemoveDeviceButton(devicesTable, deviceId).click()

      print '  Confirm removing'
      confirmRemoveModal = dashboard.devices.waitRemoveDeviceConfirmationModal(self.browser)
      confirmRemoveModal.ok()

      print '  Check that device was removed from list'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      assert tools.waitUntil(lambda: dashboard.devices.countDevices(devicesTable) == (devicesOriginalCount - 1))
      time.sleep(1)
      assert dashboard.devices.countDevices(devicesTable) == (devicesOriginalCount - 1)
      assert dashboard.devices.findDeviceInTable(devicesTable, deviceName) is None
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
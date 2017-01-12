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

class CreateDevice(unittest.TestCase):
   """Manually device creation test"""
   
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
      

   def test_createDevice(self):
      print '=== Manually device creation test ==='
      deviceName = u'My device'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Create the device'
      tools.waitUntil(lambda: dashboard.devices.getCreateDeviceButton(self.browser).is_enabled())
      dashboard.devices.getCreateDeviceButton(self.browser).click()
      newDeviceModal = dashboard.devices.waitNewDeviceModal(self.browser)
      newDeviceModal.selectAttachedPlugin(attachedPluginInstance).click()
      newDeviceModal.ok()
      
      print '  Configure the device'
      editDeviceModal = dashboard.devices.waitConfigureManuallyDeviceModal(self.browser)
      editDeviceModal.setDeviceName(deviceName)
      print '  Click OK'
      editDeviceModal.ok()
      
      print '  Check created device'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deviceDatas = dashboard.devices.getDeviceDatas(devicesTable, deviceId)
      self.assertTrue(tools.waitUntil(lambda: dashboard.devices.getDeviceName(devicesTable, deviceId) == deviceName))
      self.assertEqual(dashboard.devices.getAttachedPlugin(devicesTable, deviceId), attachedPluginInstance)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
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
      deviceName = u'fakeDimmableReadOnlySwitch' # TODO : change the package.json of fakePlugin to make this device non-configurable
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Check that device is not configurable'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      assert dashboard.devices.getConfigureDeviceButton(devicesTable, deviceId) is None
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
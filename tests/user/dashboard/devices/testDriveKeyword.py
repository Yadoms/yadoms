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

class DriveKeyword(unittest.TestCase):
   """Drive keyword test"""
   
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


   def test_notDrivableKeyword(self):
      print '=== Check that a not drivable keyword doesnt have the drive button ==='
      deviceName = u'fakeOnOffReadOnlySwitch'
      keywordName = u'Switch'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Deploy device keywords'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deployButton = dashboard.devices.getDeployKeywordsButton(devicesTable, deviceId)
      deployButton.click()

      print '  Select keyword'
      tools.waitUntil(lambda: len(dashboard.devices.getKeywords(devicesTable)) == 1)
      keyword = dashboard.devices.getKeywords(devicesTable)[0]
      assert dashboard.devices.getKeywordName(keyword) == keywordName
     
      print '  Check that keyword is drivable'
      button = dashboard.devices.getCommandKeywordButton(keyword)
      assert button is None


   def test_driveOnOffKeyword(self):
      print '=== Try to drive a on/off keyword ==='
      print 'ref Issues : #162'

      deviceName = u'fakeOnOffReadWriteSwitch'
      keywordName = u'Switch'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Deploy device keywords'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deployButton = dashboard.devices.getDeployKeywordsButton(devicesTable, deviceId)
      deployButton.click()

      print '  Select keyword'
      tools.waitUntil(lambda: len(dashboard.devices.getKeywords(devicesTable)) == 1)
      keyword = dashboard.devices.getKeywords(devicesTable)[0]
      assert dashboard.devices.getKeywordName(keyword) == keywordName
     
      print '  Check that keyword is drivable'
      button = dashboard.devices.getCommandKeywordButton(keyword)
      assert button is not None

      print '  Set value to True'
      button.click()
      setKeywordValueModal = dashboard.devices.waitSetValueKeywordModal(self.browser)
      assert setKeywordValueModal.getBoolValue() == False
      setKeywordValueModal.setBoolValue(True)
      setKeywordValueModal.ok()
      assert tools.waitUntil(lambda: dashboard.devices.getKeywordBoolValue(keyword) == True)

      print '  Set value to False'
      button.click()
      setKeywordValueModal = dashboard.devices.waitSetValueKeywordModal(self.browser)
      assert setKeywordValueModal.getBoolValue() == True
      setKeywordValueModal.setBoolValue(False)
      setKeywordValueModal.ok()
      assert tools.waitUntil(lambda: dashboard.devices.getKeywordBoolValue(keyword) == False)


   def test_driveEnumKeyword(self):
      print '=== Try to drive an enum keyword ==='
      print 'ref Issues : #176'

      deviceName = u'fakeController1'
      keywordName = u'controllerValue'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Deploy device keywords'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deployButton = dashboard.devices.getDeployKeywordsButton(devicesTable, deviceId)
      deployButton.click()

      print '  Select keyword'
      tools.waitUntil(lambda: len(dashboard.devices.getKeywords(devicesTable)) == 1)
      keyword = dashboard.devices.getKeywords(devicesTable)[0]
      assert dashboard.devices.getKeywordName(keyword) == keywordName
     
      print '  Check that keyword is drivable'
      button = dashboard.devices.getCommandKeywordButton(keyword)
      assert button is not None

      print '  Set value to Run'
      button.click()
      setKeywordValueModal = dashboard.devices.waitSetValueKeywordModal(self.browser)
      setKeywordValueModal.setEnumValue('Run')
      setKeywordValueModal.ok()
      assert tools.waitUntil(lambda: dashboard.devices.getKeywordTextValue(keyword) == 'Run')

      print '  Set value to Left'
      button.click()
      setKeywordValueModal = dashboard.devices.waitSetValueKeywordModal(self.browser)
      setKeywordValueModal.setEnumValue('Left')
      setKeywordValueModal.ok()
      assert tools.waitUntil(lambda: dashboard.devices.getKeywordTextValue(keyword) == 'Left')

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
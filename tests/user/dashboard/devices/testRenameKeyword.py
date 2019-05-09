import sys
sys.path.append('.')
import unittest
from selenium import webdriver
import browser
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

class RenameKeyword(unittest.TestCase):
   """Rename keyword test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = browser.getBrowser()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openDevice(self.browser)

   def test_renameKeyword(self):
      print '=== Rename a keyword ==='
      deviceName = u'fakeSensor1'
      keywordName = u'Battery'
      newKeywordName = u'myNewKeywordName'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Deploy device keywords'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deployButton = dashboard.devices.getDeployKeywordsButton(devicesTable, deviceId)
      deployButton.click()

      print '  Change device name then cancel'
      keywords = dashboard.devices.getKeywords(devicesTable)
      
      print '    Check Battery name'
      self.assertEqual(dashboard.devices.getKeywordName(keywords[0]), keywordName)

      print '    Open modal & change keyword name & cancel'
      actionButton = dashboard.devices.getConfigureKeywordButton(keywords[0])
      actionButton.click()
      configureKeywordModal = dashboard.devices.waitConfigureKeywordModal(self.browser)
      configureKeywordModal.updateTextField('friendlyName', newKeywordName)
      configureKeywordModal.cancel()
      
      print '    Check old keyword name after cancelled'
      self.assertEqual(dashboard.devices.getKeywordName(keywords[0]), keywordName)

      print '    Open modal & change keyword name & ok'
      actionButton = dashboard.devices.getConfigureKeywordButton(keywords[0])
      actionButton.click()
      configureKeywordModal = dashboard.devices.waitConfigureKeywordModal(self.browser)
      configureKeywordModal.updateTextField('friendlyName', newKeywordName)
      configureKeywordModal.ok()
      
      print '  Check change was saved'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      keywords = dashboard.devices.getKeywords(devicesTable)
      self.assertEqual(dashboard.devices.getKeywordName(keywords[0]), newKeywordName)
      actionButton.click()
      configureKeywordModal = dashboard.devices.waitConfigureKeywordModal(self.browser)
      self.assertEqual(configureKeywordModal.getTextField('friendlyName'), newKeywordName)
      configureKeywordModal.cancel()
      
      print '    Open modal & cancel : to test if the reload of the button is working well'
      actionButton = dashboard.devices.getConfigureKeywordButton(keywords[0])
      actionButton.click()
      configureKeywordModal = dashboard.devices.waitConfigureKeywordModal(self.browser)
      configureKeywordModal.cancel()      

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
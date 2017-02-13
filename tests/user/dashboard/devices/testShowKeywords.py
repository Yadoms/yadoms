import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
from selenium.common.exceptions import NoSuchElementException
import database
import config
import yadomsServer
import dashboard
import dashboard.devices
import tools
import i18n
import time
import re
import tools

class ShowKeywords(unittest.TestCase):
   """Show keywords device test"""
   
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
      

   def test_showKeywords(self):
      print '=== Show keywords of a device ==='
      deviceName = u'fakeSensor1'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print '  Deploy device keywords'
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deployButton = dashboard.devices.getDeployKeywordsButton(devicesTable, deviceId)
      deployButton.click()

      print '  Check keyword list'
      assert tools.waitUntil(lambda: len(dashboard.devices.getKeywords(devicesTable)) == 6)
      keywords = dashboard.devices.getKeywords(devicesTable)

      print '  Wait for first acquisition'
      tools.waitUntil(lambda: dashboard.devices.getKeywordTextValue(keywords[0]))

      print '    Check Battery data'
      item = keywords[0]
      assert dashboard.devices.getKeywordName(item) == u'Battery'
      assert int(dashboard.devices.getKeywordTextValue(item)) in range(0, 100)
      assert dashboard.devices.getKeywordUnit(item) == u'%'
      assert len(dashboard.devices.getKeywordActions(item)) == 1
      assert 'btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class')

      print '    Check Current data'
      item = keywords[1]
      assert dashboard.devices.getKeywordName(item) == u'current'
      assert float(dashboard.devices.getKeywordTextValue(item))
      assert dashboard.devices.getKeywordUnit(item) == u'A'
      assert len(dashboard.devices.getKeywordActions(item)) == 1
      assert 'btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class')

      print '    Check DateTime data'
      item = keywords[2]
      assert dashboard.devices.getKeywordName(item) == u'dateTime'
      assert re.match(r'20[0-9]{2}-[A-Z][a-z]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]{6}', dashboard.devices.getKeywordTextValue(item))
      assert dashboard.devices.getKeywordUnit(item) == u''
      assert len(dashboard.devices.getKeywordActions(item)) == 1
      assert 'btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class')

      print '    Check rssi data'
      item = keywords[3]
      assert dashboard.devices.getKeywordName(item) == u'rssi'
      assert int(dashboard.devices.getKeywordTextValue(item)) in range(0, 100)
      assert dashboard.devices.getKeywordUnit(item) == u'%'
      assert len(dashboard.devices.getKeywordActions(item)) == 1
      assert 'btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class')

      print '    Check temp1 data'
      item = keywords[4]
      assert dashboard.devices.getKeywordName(item) == u'temp1'
      assert float(dashboard.devices.getKeywordTextValue(item))
      assert dashboard.devices.getKeywordUnit(item) == u'°C'
      assert len(dashboard.devices.getKeywordActions(item)) == 1
      assert 'btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class')

      print '    Check temp2 data'
      item = keywords[5]
      assert dashboard.devices.getKeywordName(item) == u'temp2'
      assert float(dashboard.devices.getKeywordTextValue(item))
      assert dashboard.devices.getKeywordUnit(item) == u'°C'
      assert len(dashboard.devices.getKeywordActions(item)) == 1
      assert 'btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class')
      
      print '   Collapse keywords'
      deployButton.click()
      try:
         devicesTable.find_element_by_class_name('device-details')
         assert False
      except NoSuchElementException:
         pass
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
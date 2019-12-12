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

class ShowKeywords(unittest.TestCase):
   """Show keywords device test"""
   
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
      

   def test_showKeywords(self):
      print ('=== Show keywords of a device ===')
      deviceName = u'fakeSensor1'
      attachedPluginInstance = u'My fakePlugin instance'
     
      print ('  Deploy device keywords')
      devicesTable = dashboard.devices.waitDevicesTable(self.browser)
      deviceId = dashboard.devices.waitDevicesTableHasDeviceNamed(self.browser, deviceName)
      deployButton = dashboard.devices.getDeployKeywordsButton(devicesTable, deviceId)
      deployButton.click()

      print ('  Check keyword list')
      self.assertTrue(tools.waitUntil(lambda: len(dashboard.devices.getKeywords(devicesTable)) == 6))
      keywords = dashboard.devices.getKeywords(devicesTable)

      print ('  Wait for first acquisition')
      tools.waitUntil(lambda: dashboard.devices.getKeywordTextValue(keywords[0]))

      print ('    Check Battery data')
      item = keywords[0]
      self.assertEqual(dashboard.devices.getKeywordName(item), u'Battery')
      self.assertTrue(int(dashboard.devices.getKeywordTextValue(item)) in range(0, 100))
      self.assertEqual(dashboard.devices.getKeywordUnit(item), u'%')
      self.assertEqual(len(dashboard.devices.getKeywordActions(item)), 1)
      self.assertTrue('btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class'))

      print ('    Check Current data')
      item = keywords[1]
      self.assertEqual(dashboard.devices.getKeywordName(item), u'current')
      self.assertTrue(float(dashboard.devices.getKeywordTextValue(item)))
      self.assertEqual(dashboard.devices.getKeywordUnit(item), u'A')
      self.assertEqual(len(dashboard.devices.getKeywordActions(item)), 1)
      self.assertTrue('btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class'))

      print ('    Check DateTime data')
      item = keywords[2]
      self.assertEqual(dashboard.devices.getKeywordName(item), u'dateTime')
      self.assertTrue(re.match(r'20[0-9]{2}-[A-Z][a-z]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]{6}', dashboard.devices.getKeywordTextValue(item)))
      self.assertEqual(dashboard.devices.getKeywordUnit(item), u'')
      self.assertEqual(len(dashboard.devices.getKeywordActions(item)), 1)
      self.assertTrue('btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class'))

      print ('    Check signalPower data')
      item = keywords[3]
      self.assertEqual(dashboard.devices.getKeywordName(item), u'signalPower')
      self.assertTrue(int(dashboard.devices.getKeywordTextValue(item)) in range(0, 100))
      self.assertEqual(dashboard.devices.getKeywordUnit(item), u'%')
      self.assertEqual(len(dashboard.devices.getKeywordActions(item)), 1)
      self.assertTrue('btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class'))

      print ('    Check temp1 data')
      item = keywords[4]
      self.assertEqual(dashboard.devices.getKeywordName(item), u'temp1')
      self.assertTrue(float(dashboard.devices.getKeywordTextValue(item)))
      self.assertEqual(dashboard.devices.getKeywordUnit(item), u'°C')
      self.assertEqual(len(dashboard.devices.getKeywordActions(item)), 1)
      self.assertTrue('btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class'))

      print ('    Check temp2 data')
      item = keywords[5]
      self.assertEqual(dashboard.devices.getKeywordName(item), u'temp2')
      self.assertTrue(float(dashboard.devices.getKeywordTextValue(item)))
      self.assertEqual(dashboard.devices.getKeywordUnit(item), u'°C')
      self.assertEqual(len(dashboard.devices.getKeywordActions(item)), 1)
      self.assertTrue('btn-configure' in dashboard.devices.getKeywordActions(item)[0].get_attribute('class'))
      
      print ('   Collapse keywords')
      deployButton.click()
      with self.assertRaises(NoSuchElementException):
         devicesTable.find_element_by_class_name('device-details')
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
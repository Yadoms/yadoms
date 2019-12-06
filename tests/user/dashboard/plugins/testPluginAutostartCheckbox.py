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
import dashboard.plugins
import tools

class AutostartCheckboxPlugin(unittest.TestCase):
   """Plugin autostart checkbox test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneFakePlugin')
      config.deploy("nominal")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)
      

   def test_autostartCheckbox(self):
      print ('=== Autostart Checkbox test ===')
      print ('ref Issues : #182')
      pluginNumber = 0
     
      print ('  Verify initial state')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      self.assertTrue(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))
      
      print ('  Uncheck the box')
      dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber).click()
      tools.waitUntil(lambda: dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber) is False)

      print ('  Reload page and check box state')
      dashboard.openSummary(self.browser)
      dashboard.openPlugin(self.browser)
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber) is False)
      
      print ('  Check the box')
      dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber).click()
      tools.waitUntil(lambda: dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber) is True)

      print ('  Reload page and check box state')
      dashboard.openPlugin(self.browser)
      tools.waitUntil(lambda: dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber) is True)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
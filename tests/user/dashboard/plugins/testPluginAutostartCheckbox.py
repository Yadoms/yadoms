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
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)
      

   def test_autostartCheckbox(self):
      print '=== Autostart Checkbox test ==='
      print 'ref Issues : #182'
      pluginNumber = 0
     
      print '  Verify initial state'
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      self.assertTrue(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))
      
      print '  Uncheck the box'
      dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber).click()
      self.assertFalse(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))

      print '  Reload page and check box state'
      dashboard.openSummary(self.browser)
      dashboard.openPlugin(self.browser)
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      self.assertFalse(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))
      
      print '  Check the box'
      dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber).click()
      self.assertTrue(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))

      print '  Reload page and check box state'
      dashboard.openPlugin(self.browser)
      self.assertTrue(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
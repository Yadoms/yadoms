import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import database
import scripts
import yadomsServer
import dashboard
import dashboard.automation
import tools

class StartStopPlugin(unittest.TestCase):
   """Start/Stop plugin test"""
   
   def setUp(self):
      database.deploy('OneFakePlugin')
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      
   def test_startStopPlugin(self):
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)
      pluginNumber = 0
      
      # Get plugins table
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      startStopButton = dashboard.plugins.getPluginStartStopButton(pluginsTable, pluginNumber)
      
      self.assertEqual(dashboard.plugins.getPluginState(pluginsTable, pluginNumber), dashboard.plugins.PluginState.Running)

      # Stop plugin
      tools.waitUntil(lambda: startStopButton.is_enabled())
      startStopButton.click()
      tools.waitUntil(lambda: dashboard.plugins.getPluginState(pluginsTable, pluginNumber) is dashboard.plugins.PluginState.Stopped)

      # Start plugin
      tools.waitUntil(lambda: startStopButton.is_enabled())
      startStopButton.click()
      tools.waitUntil(lambda: dashboard.plugins.getPluginState(pluginsTable, pluginNumber) is dashboard.plugins.PluginState.Running)
            
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
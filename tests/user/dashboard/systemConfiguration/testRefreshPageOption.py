import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import database
import config
import scripts
import yadomsServer
import dashboard.systemConfiguration
import tools

class RefreshPageOption(unittest.TestCase):
   """RefreshPage option test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.new()
      config.deploy("nominal")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      
   def test_refreshPageOptionDefaultValue(self):
      print '=== RefreshPage option default value check ==='

      print 'Open systemConfiguration dashboard'
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      
      print 'Active advanced parameters'
      advancedParameterSection = dashboard.systemConfiguration.enableAdvancedParametersSection(self.browser)

      print 'Check RefreshPage option default value'
      self.assertTrue(dashboard.systemConfiguration.getRefreshPageOptionState(advancedParameterSection), True)

      
   def test_refreshPageOption(self):
      print '=== RefreshPage option test ==='

      print 'Create 2 more pages (for a total of 3 pages)'
      #TODO

      print 'Open systemConfiguration dashboard'
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      
      print 'Active RefreshPage option'      
      advancedParameterSection = dashboard.systemConfiguration.enableAdvancedParametersSection(self.browser)
      dashboard.systemConfiguration.enableRefreshPageOption(advancedParameterSection)

      import time
      time.sleep(3)


#|||
#      startStopButton = dashboard.plugins.getPluginStartStopButton(pluginsTable, pluginNumber)
      
#      self.assertEqual(dashboard.plugins.getPluginState(pluginsTable, pluginNumber), dashboard.plugins.PluginState.Running)

#      print 'Stop plugin'
#      self.assertTrue(tools.waitUntil(lambda: startStopButton.is_enabled()))
#      startStopButton.click()
#      self.assertTrue(tools.waitUntil(lambda: dashboard.plugins.getPluginState(pluginsTable, pluginNumber) is dashboard.plugins.PluginState.Stopped))

#      print 'Start plugin'
#      self.assertTrue(tools.waitUntil(lambda: startStopButton.is_enabled()))
#      startStopButton.click()
#      self.assertTrue(tools.waitUntil(lambda: dashboard.plugins.getPluginState(pluginsTable, pluginNumber) is dashboard.plugins.PluginState.Running))
            
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
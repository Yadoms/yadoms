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

class ConfigurePlugin(unittest.TestCase):
   """Configure plugin test"""
   
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
      dashboard.openPlugin(self.browser)
      

   def test_configurePlugin(self):
      print ('=== Configure plugin test ===')
      pluginNumber = 0
     
      print ('  Configure the first plugin')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginConfigureButton(pluginsTable, pluginNumber).is_enabled())
      dashboard.plugins.getPluginConfigureButton(pluginsTable, pluginNumber).click()
      
      print ('  Modify plugin name')
      pluginNewName = "This is the new plugin name"
      configurePluginModal = dashboard.plugins.waitConfigurePluginModal(self.browser)
      configurePluginModal.setPluginName(pluginNewName)
      configurePluginModal.ok()
      
      print ('  Check modified plugin')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      pluginDatas = dashboard.plugins.getPluginDatas(pluginsTable, pluginNumber)
      self.assertTrue(tools.waitUntil(lambda: dashboard.plugins.getPluginName(pluginsTable, pluginNumber) == pluginNewName))
      self.assertEqual(dashboard.plugins.getPluginState(pluginsTable, pluginNumber), dashboard.plugins.PluginState.Running)
      self.assertTrue(dashboard.plugins.getPluginAutoStartState(pluginsTable, pluginNumber))
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
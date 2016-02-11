import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
import database
import yadomsServer
import dashboard
import dashboard.plugins
import tools

class RemovePlugin(unittest.TestCase):
   """Remove plugin test"""
   
   def setUp(self):
      database.deploy('OneFakePlugin')
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)
      

   def test_configurePlugin(self):
      pluginNumber = 0

      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginRemoveButton(pluginsTable, pluginNumber).is_enabled())
      removeButton = dashboard.plugins.getPluginRemoveButton(pluginsTable, pluginNumber)
      
      self.assertEqual(dashboard.plugins.getPluginState(pluginsTable, pluginNumber), dashboard.plugins.PluginState.Running)
      
      # Remove plugin
      removeButton.click()
      confirmationModal = dashboard.plugins.waitRemovePluginConfirmationModal(self.browser)
      confirmationModal.getOkButton().click()
      
      # Table should be updated
      self.assertTrue(tools.waitUntil(lambda: dashboard.plugins.getPluginNumberInTable(self.browser, pluginsTable) == 0, 5))
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
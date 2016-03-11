import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
import database
import scripts
import yadomsServer
import dashboard
import dashboard.plugins
import notification
import i18n
import tools
import os.path

class CreatePlugin(unittest.TestCase):
   """Create plugin test"""
   
   def setUp(self):
      database.new()
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      

   def checkCreateOkPlugin(self, pluginInstanceName, pluginType):
      # - notification
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["configure-plugin"]["pluginSuccessfullyCreated"])
      
      # - in web client
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)

      pluginNumber = 0
      
      self.assertEqual(len(dashboard.plugins.getPluginDatas(pluginsTable, pluginNumber)), 5)
      self.assertEqual(dashboard.plugins.getPluginName(pluginsTable, pluginNumber), pluginInstanceName)
      self.assertEqual(dashboard.plugins.getPluginType(pluginsTable, pluginNumber).lstrip(), pluginType)
      self.assertTrue(dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber))

      buttons = dashboard.plugins.getPluginButtons(pluginsTable, pluginNumber)
      self.assertEqual(len(buttons), 3)
      WebDriverWait(self.browser, 10).until(lambda driver: \
         "btn" in dashboard.plugins.getPluginStartStopButton(pluginsTable, pluginNumber).get_attribute("class") and \
         "btn-startStop" in dashboard.plugins.getPluginStartStopButton(pluginsTable, pluginNumber).get_attribute("class") and \
         "btn-warning" in dashboard.plugins.getPluginStartStopButton(pluginsTable, pluginNumber).get_attribute("class"))
      self.assertTrue( \
         "btn" in dashboard.plugins.getPluginConfigureButton(pluginsTable, pluginNumber).get_attribute("class") and \
         "btn-configure" in dashboard.plugins.getPluginConfigureButton(pluginsTable, pluginNumber).get_attribute("class") and \
         "btn-primary" in dashboard.plugins.getPluginConfigureButton(pluginsTable, pluginNumber).get_attribute("class"))
      self.assertTrue( \
         "btn" in dashboard.plugins.getPluginRemoveButton(pluginsTable, pluginNumber).get_attribute("class") and \
         "btn-delete" in dashboard.plugins.getPluginRemoveButton(pluginsTable, pluginNumber).get_attribute("class") and \
         "btn-danger" in dashboard.plugins.getPluginRemoveButton(pluginsTable, pluginNumber).get_attribute("class"))

      WebDriverWait(self.browser, 20).until(lambda driver: dashboard.plugins.getPluginState(pluginsTable, pluginNumber) == dashboard.plugins.PluginState.Running)
            
            
   def test_createOkPlugin(self):
      """Nominal test of plugin creation"""
      self.doTest(
         "My fakePlugin instance",
         i18n.getPlugin("fakePlugin")["name"],
         lambda pluginInstanceName, pluginType: self.checkCreateOkPlugin(pluginInstanceName, pluginType))

         
      
   def doTest(self, pluginInstanceName, pluginType, checkPluginFct):
      # Open plugin dashboard
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)

      # Create new plugin
      tools.waitUntil(lambda: dashboard.plugins.getCreatePluginButton(self.browser).is_enabled())
      dashboard.plugins.getCreatePluginButton(self.browser).click()
      newPluginModal = dashboard.plugins.waitNewPluginModal(self.browser)
      newPluginModal.selectPlugin(pluginType).click()
      newPluginModal.getConfirmButton().click()

      # - Plugin configuration
      editPluginModal = dashboard.plugins.waitConfigurePluginModal(self.browser)
      editPluginModal.replacePluginName(pluginInstanceName)
         

      # Click OK
      editPluginModal.getConfirmButton().click()
      
      
      # Check created plugin
      checkPluginFct(pluginInstanceName, pluginType)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
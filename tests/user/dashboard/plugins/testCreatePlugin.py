import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
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
      self.assertEqual(dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber), True)

      buttons = dashboard.plugins.getRuleButtons(pluginsTable, pluginNumber)
      self.assertEqual(len(buttons), 3)
      self.assertEqual(dashboard.plugins.getRuleStartStopButton(pluginsTable, pluginNumber).get_attribute("class"), "btn btn-enableDisable btn-warning")
      self.assertEqual(dashboard.plugins.getRuleEditButton(pluginsTable, pluginNumber).get_attribute("class"), "btn btn-edit btn-primary")
      self.assertEqual(dashboard.plugins.getRuleRemoveButton(pluginsTable, pluginNumber).get_attribute("class"), "btn btn-delete btn-danger")

      self.assertEqual(dashboard.plugins.getRuleState(pluginsTable, pluginNumber), dashboard.plugins.PluginState.Running)
      
      
      # - on disk (corresponding script file)
      self.assertTrue(scripts.checkLocalRuleCodeById(1, ruleCode))
      self.assertTrue(tools.waitUntil(lambda: scripts.checkLocalRuleLogById(1, ruleLog)))
            
            
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
      dashboard.plugins.getCreatePluginButton(self.browser).click()
      newPluginModal = dashboard.plugins.waitNewPluginModal(self.browser)
      newPluginModal.selectPlugin(pluginType).click()
      newPluginModal.getConfirmButton(self.browser).click()

      # - Plugin configuration
      editPluginModal = dashboard.plugins.waitConfigurePluginModal(self.browser)
      editPluginModal.replacePluginName(pluginInstanceName)
         

      # Click OK
      editPluginModal.getConfirmButton(self.browser).click()
      
      
      # Check created plugin
      checkPluginFct(pluginInstanceName, pluginType)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
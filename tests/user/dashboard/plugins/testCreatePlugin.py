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
      print '  Check notification'
      notification.waitText(self.browser, notification.Type.Success, i18n.get()["modals"]["configure-plugin"]["pluginSuccessfullyCreated"])
      
      print '  Check plugins table'
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)

      pluginNumber = 0
      
      self.assertEqual(len(dashboard.plugins.getPluginDatas(pluginsTable, pluginNumber)), 5)
      self.assertEqual(dashboard.plugins.getPluginName(pluginsTable, pluginNumber), pluginInstanceName)
      self.assertEqual(dashboard.plugins.getPluginType(pluginsTable, pluginNumber).lstrip(), pluginType)
      self.assertTrue(dashboard.plugins.getPluginAutoStart(pluginsTable, pluginNumber))

      buttons = dashboard.plugins.getPluginButtons(pluginsTable, pluginNumber)
      self.assertEqual(len(buttons), 4)

      startStopButton = dashboard.plugins.getPluginStartStopButton(pluginsTable, pluginNumber)
      self.assertEqual(startStopButton, buttons[0])
      self.assertIn("btn-warning", startStopButton.get_attribute("class"))

      configureButton = dashboard.plugins.getPluginConfigureButton(pluginsTable, pluginNumber)
      self.assertEqual(configureButton, buttons[1])
      self.assertIn("btn-primary", configureButton.get_attribute("class"))

      extraCommandButton = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      self.assertEqual(extraCommandButton, buttons[2])
      self.assertIn("btn-success", extraCommandButton.get_attribute("class"))

      removeButton = dashboard.plugins.getPluginRemoveButton(pluginsTable, pluginNumber)
      self.assertEqual(removeButton, buttons[3])
      self.assertIn("btn-danger", removeButton.get_attribute("class"))

      WebDriverWait(self.browser, 20).until(lambda driver: dashboard.plugins.getPluginState(pluginsTable, pluginNumber) == dashboard.plugins.PluginState.Running)
            
            
   def test_createOkPlugin(self):
      print '=== Nominal test of plugin creation ==='
      self.doTest(
         "My fakePlugin instance",
         i18n.getPlugin("fakePlugin")["name"],
         lambda pluginInstanceName, pluginType: self.checkCreateOkPlugin(pluginInstanceName, pluginType))

         
      
   def doTest(self, pluginInstanceName, pluginType, checkPluginFct):
      print 'Open plugin dashboard'
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)

      print 'Create new plugin'
      tools.waitUntil(lambda: dashboard.plugins.getCreatePluginButton(self.browser).is_enabled())
      dashboard.plugins.getCreatePluginButton(self.browser).click()
      newPluginModal = dashboard.plugins.waitNewPluginModal(self.browser)
      newPluginModal.selectPlugin(pluginType).click()
      newPluginModal.ok()

      print 'Plugin configuration'
      editPluginModal = dashboard.plugins.waitConfigurePluginModal(self.browser)
      editPluginModal.setPluginName(pluginInstanceName)
         

      print 'Click OK'
      editPluginModal.ok()
      
      
      print 'Check created plugin'
      checkPluginFct(pluginInstanceName, pluginType)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
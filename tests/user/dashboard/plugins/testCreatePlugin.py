import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
import database
import config
import scripts
import yadomsServer
import dashboard.plugins

class CreatePlugin(unittest.TestCase):
   """Create plugin test"""
   
   def setUp(self):
      database.new()
      config.deploy("withDeveloperMode")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
            
            
   def test_createOkPlugin(self):
      print '=== Nominal test of plugin creation ==='

      pluginInstanceName = "My fakePlugin instance"
      pluginType = "dev-fakePlugin"

      print 'Create created plugin'
      dashboard.plugins.createPluginSequence(self.browser, pluginInstanceName, pluginType, lambda pluginInstanceName: dashboard.plugins.basicFillConfigurationSequence(self.browser, pluginInstanceName))

      print 'Check created plugin'
      dashboard.plugins.checkCreatedPluginSequence(self, pluginInstanceName, pluginType, True, True)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
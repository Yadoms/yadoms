import sys
sys.path.append('.')
import unittest
from selenium import webdriver
import database
import config
import scripts
import yadomsServer
import dashboard.plugins
import tools

class CreatePlugin(unittest.TestCase):
   """Create systemInformation plugin test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.new()
      config.deploy("nominal")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome(options=tools.ChromeOptionsHelper.get())
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
            
            
   def test_createSystemInformationPlugin(self):
      print ('=== Test of systemInformation plugin creation ===')

      pluginInstanceName = "My systemInformation instance"
      pluginType = "SystemInformation"

      print ('Create created plugin')
      dashboard.plugins.createPluginSequence(self.browser, pluginInstanceName, pluginType, lambda pluginInstanceName: dashboard.plugins.basicFillConfigurationSequence(self.browser, pluginInstanceName))

      print ('Check created plugin')
      dashboard.plugins.checkCreatedPluginSequence(self, pluginInstanceName, pluginType, False, False)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
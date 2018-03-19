import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
import database
import config
import i18n
import notification
import yadomsServer
import dashboard
import dashboard.plugins
import tools

class ExtraQuery(unittest.TestCase):
   """Extra queries test"""
   
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
      

   def test_simpleQuery(self):
      print '=== Simple query test ==='
      pluginNumber = 0
     
      print '  Select query'
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-simpleCommand']").click()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["simpleCommand"]["name"]))
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
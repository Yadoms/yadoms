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
import notification
import scripts
import yadomsServer
import dashboard.systemConfiguration
import tools
import mainPage
import i18n

class AdvancedParameterSection(unittest.TestCase):
   """Advanced parameter section test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.new()
      config.deploy("nominal")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      
   def test_testAdvancedParameterSectionReset(self):
      print '=== Advanced parameter section default state check (cold start), save value, restore value and reset to default ==='

      print 'Open systemConfiguration dashboard'
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)

      print 'Check Advanced parameter section default state (cold start)'
      self.assertFalse(dashboard.systemConfiguration.isAdvancedParametersSectionActive(self.browser))
      
      print 'Enable Advanced parameter section'      
      dashboard.systemConfiguration.enableAdvancedParametersSection(self.browser)
      dashboard.systemConfiguration.applySystemConfiguration(self.browser)
      dashboard.close(self.browser)

      print 'Check that advanced parameter section state was saved'
      self.browser.refresh()
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      self.assertTrue(dashboard.systemConfiguration.isAdvancedParametersSectionActive(self.browser))

      print 'Reset to default value'
      dashboard.systemConfiguration.resetToDefaultSystemConfiguration(self.browser)
      # Reseting to default values make page reload, wait for page reloaded
      yadomsServer.waitPageLoaded(self.browser)

      print 'Re-open systemConfiguration dashboard and check state was restored'
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      self.assertFalse(dashboard.systemConfiguration.isAdvancedParametersSectionActive(self.browser))

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
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
import modals
import scripts
import yadomsServer
import dashboard.systemConfiguration
import tools
import mainPage
import i18n

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
      
      
   def test_refreshPageOptionSaveRestoreReset(self):
      print ('=== RefreshPage option default value check (cold start), save value, restore value and reset to default ===')

      print ('Open systemConfiguration dashboard and active advanced parameters')
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      panel = dashboard.systemConfiguration.getPanel(self.browser)
      advancedParameterSection = panel.enableAdvancedParametersSection()

      print ('Check RefreshPage option default value (cold start)')
      self.assertTrue(panel.getRefreshPageOptionState(advancedParameterSection))

      print ('Change RefreshPage option value')
      panel.setRefreshPageOption(advancedParameterSection, False)
      panel.apply()
      
      print ('Close dashboard')
      dashboard.close(self.browser)

      print ('Re-open systemConfiguration dashboard and active advanced parameters')
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      panel = dashboard.systemConfiguration.getPanel(self.browser)
      advancedParameterSection = panel.enableAdvancedParametersSection()

      print ('Check RefreshPage option was saved')
      self.assertFalse(panel.getRefreshPageOptionState(advancedParameterSection))

      print ('Reset to default value')
      panel.resetToDefaultValues()
      modals.waitOkCancelModal(self.browser).ok()
      # Reseting to default values make page reload, wait for page reloaded
      yadomsServer.waitPageLoaded(self.browser)

      print ('Re-open systemConfiguration dashboard and active advanced parameters')
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      panel = dashboard.systemConfiguration.getPanel(self.browser)
      advancedParameterSection = panel.enableAdvancedParametersSection()

      print ('Check default value was restored')
      self.assertTrue(panel.getRefreshPageOptionState(advancedParameterSection))

   def test_refreshPageOptionEnable(self):
      print ('=== RefreshPage option (enabled) test ===')

      print ('Open systemConfiguration dashboard')
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      panel = dashboard.systemConfiguration.getPanel(self.browser)
      
      print ('Enable RefreshPage option')
      advancedParameterSection = panel.enableAdvancedParametersSection()
      panel.setRefreshPageOption(advancedParameterSection, True)
      panel.apply()
      dashboard.close(self.browser)

      print ('Create 3 more pages (for a total of 4 pages)')
      mainPage.addPage(self.browser, "Page 1")
      mainPage.addPage(self.browser, "Page 2")
      mainPage.addPage(self.browser, "Page 3")

      print ('Select page 2 and refresh')
      mainPage.getPagesMenuBar(self.browser).selectPage("Page 2")
      self.browser.refresh()
      self.assertEqual(mainPage.getPagesMenuBar(self.browser).getCurrentPage().getName(), "Page 2")

      print ('Select page 3 and refresh')
      mainPage.getPagesMenuBar(self.browser).selectPage("Page 3")
      self.browser.refresh()
      self.assertEqual(mainPage.getPagesMenuBar(self.browser).getCurrentPage().getName(), "Page 3")

      print ('Select page 1 and refresh')
      mainPage.getPagesMenuBar(self.browser).selectPage("Page 1")
      self.browser.refresh()
      self.assertEqual(mainPage.getPagesMenuBar(self.browser).getCurrentPage().getName(), "Page 1")

      
   def test_refreshPageOptionDisable(self):
      print ('=== RefreshPage option (disabled) test ===')

      print ('Open systemConfiguration dashboard')
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)
      panel = dashboard.systemConfiguration.getPanel(self.browser)
      
      print ('Disable RefreshPage option')
      advancedParameterSection = panel.enableAdvancedParametersSection()
      panel.setRefreshPageOption(advancedParameterSection, False)
      panel.apply()
      dashboard.close(self.browser)

      print ('Create 3 more pages (for a total of 4 pages)')
      mainPage.addPage(self.browser, "Page 1")
      mainPage.addPage(self.browser, "Page 2")
      mainPage.addPage(self.browser, "Page 3")

      welcomePageTitle = i18n.get()["initialization"]["homePage"]

      print ('Select page 2 and refresh')
      mainPage.getPagesMenuBar(self.browser).selectPage("Page 2")
      self.browser.refresh()
      self.assertEqual(mainPage.getPagesMenuBar(self.browser).getCurrentPage().getName(), welcomePageTitle)

      print ('Select page 3 and refresh')
      mainPage.getPagesMenuBar(self.browser).selectPage("Page 3")
      self.browser.refresh()
      self.assertEqual(mainPage.getPagesMenuBar(self.browser).getCurrentPage().getName(), welcomePageTitle)

      print ('Select page 1 and refresh')
      mainPage.getPagesMenuBar(self.browser).selectPage("Page 1")
      self.browser.refresh()
      self.assertEqual(mainPage.getPagesMenuBar(self.browser).getCurrentPage().getName(), welcomePageTitle)

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
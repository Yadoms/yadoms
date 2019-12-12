import sys
sys.path.append('.')
import unittest
import database
import config
import scripts
import yadomsServer
import dashboard
from selenium import webdriver
import tools

class NavigationAccrossPages(unittest.TestCase):
   """Test navigation accros dashboard pages"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.new()
      config.deploy("nominal")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome(options=tools.ChromeOptionsHelper.get())
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)

   def click(self, menuEntries, pageIndex):
      self.assertTrue(menuEntries[pageIndex].is_enabled())
      print ('click on {menuEntries[pageIndex].text} page (index {str(pageIndex)}')
      menuEntries[pageIndex].click()

   def test_rightPagesDisplayed(self):
      print ('=== Test navigation accros dashboard pages ===')
      print ('ref Issues : #172, #174')

      expectedPageTitles = [ \
         u'dashboard-summary', \
         u'dashboard-system-configuration', \
         u'dashboard-plugins', \
         u'dashboard-devices', \
         u'dashboard-automation-center', \
         u'dashboard-recipients', \
         u'dashboard-install-update', \
         u'dashboard-maintenance', \
         u'dashboard-about']

      expectedButtons = [ \
         u'btn-dashboard-summary', \
         u'btn-dashboard-system-configuration', \
         u'btn-dashboard-plugins', \
         u'btn-dashboard-devices', \
         u'btn-dashboard-automatisation', \
         u'btn-dashboard-recipients', \
         u'btn-dashboard-install-update', \
         u'btn-dashboard-maintenance', \
         u'btn-dashboard-about']
      
      print ('  Enter dashboard')
      db = dashboard.open(self.browser)
   
      dashboardSubWindow = db.find_element_by_xpath(".//div[@id='main-dashboard-sub-window-content']")
      dashboardButtons = db.find_element_by_xpath(".//ul[@id='dashboard-btns']")
      menuEntries = dashboardButtons.find_elements_by_xpath("./child::*")
      self.assertEqual(len(menuEntries), len(expectedPageTitles))

      print ('   Display all dashboard pages sequentially')

      def checkPage(dashboardSubWindow, expectedPageTitles, pageIndex):
         page = dashboardSubWindow.find_element_by_xpath(".//div[@id='" + expectedPageTitles[pageIndex] + "']")
         self.assertTrue(tools.waitUntil(lambda: page.is_displayed()))

      def checkActiveButton(dashboardButtons, expectedButtons, pageIndex):
         activeButtons = dashboardButtons.find_elements_by_xpath(".//li[@class='active']")
         if len(activeButtons) != 1:
            return False
         if activeButtons[0].get_attribute("id") != expectedButtons[pageIndex]:
            return False
         return True

      for page in range(0, len(menuEntries)):
         self.click(menuEntries, page)
         checkPage(dashboardSubWindow, expectedPageTitles, page)
         self.assertTrue(tools.waitUntil(lambda: checkActiveButton(dashboardButtons, expectedButtons, page)))


   def test_quickNavigation(self):
      print ('=== Test quick navigation accross dashboard pages ===')
      print ('At test end, only one page must be displayed')
      print ('ref Issues : #172')

      print ('  Enter dashboard')
      db = dashboard.open(self.browser)
   
      dashboardButtons = db.find_element_by_id("dashboard-btns")
      menuEntries = dashboardButtons.find_elements_by_xpath("./child::*")

      # Do several tests because #172 is not systematic
      for testId in range(0, 10):

         for page in range(0, len(menuEntries)):
            self.click(menuEntries, page)

         self.assertTrue(tools.waitUntil(lambda: len(db.find_elements_by_xpath(".//div[@id='main-dashboard-sub-window-content']/div")) == 1))

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
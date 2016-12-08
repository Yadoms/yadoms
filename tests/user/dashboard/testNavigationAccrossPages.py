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
      database.new()
      config.deploy("nominal")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)

   def checkMenuEntry(self, entry, expectedEntryId):
      self.assertEqual(entry.get_attribute("id"), expectedEntryId)
      self.assertEqual(entry.is_displayed(), True)
      self.assertEqual(entry.is_enabled(), True)
      self.assertEqual(entry.is_selected(), False)
      
   def test_navigationAccrossPages(self):
      print '=== Test navigation accros dashboard pages ==='
      print ' In some case, passing from a page to another'
      print ' make bad refresh (previous page content is not hidden.'
      print ' The aim of this test is to reproduce this problem.'

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
      
      print '  Enter dashboard'
      db = dashboard.open(self.browser)
   
      dashboard_boutons = db.find_element_by_id("dashboard-btns")
      menuEntries = dashboard_boutons.find_elements_by_xpath("./child::*")
      assert len(menuEntries) == len(expectedPageTitles)

      print '   Display all dashboard pages sequentially'
      def click(menuEntries, pageIndex):
         print 'click on ' + menuEntries[pageIndex].text + ' page (index ' + str(pageIndex) + ') ==>'
         menuEntries[pageIndex].click()

      # Note that the navigation is not perfect, but considered as acceptable for actual Yadoms version
      # See https://github.com/Yadoms/yadoms/issues/172 for more information
      def getDisplayedPages(dashboardSubWindow):
         displayedPages = []
         for page in dashboardSubWindow.find_elements_by_xpath("div"):
            if 'hidden' not in page.get_attribute('class'):
               displayedPages.append(page)
         return displayedPages

      def checkPage(dashboardSubWindow, expectedPageTitles):
         displayedPages = getDisplayedPages(dashboardSubWindow)
         assert len(displayedPages) == 1
         pageTitle = displayedPages[0].get_attribute('id')
         if pageTitle == expectedPageTitles[page]:
            print '   [OK] Get expected page (' + expectedPageTitles[page] + ')'
            return True
         print '    [FAIL] Displayed page is |' + pageTitle + '|, should be |' + expectedPageTitles[page] + '|'
         return False

      dashboardSubWindow = db.find_element_by_id("main-dashboard-sub-window-content")
      for page in range(0, len(menuEntries)):
         assert tools.doUntil(lambda : click(menuEntries, page), lambda: checkPage(dashboardSubWindow, expectedPageTitles), 3)
            

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
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
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)

   def click(self, menuEntries, pageIndex):
      print 'click on ' + menuEntries[pageIndex].text + ' page (index ' + str(pageIndex) + ')'
      menuEntries[pageIndex].click()

   def getDisplayedPages(self, dashboardSubWindow):
      displayedPages = []
      for page in dashboardSubWindow.find_elements_by_xpath("div"):
         pageClasses = page.get_attribute('class')
         if pageClasses is not None and 'hidden' not in pageClasses:
            displayedPages.append(page)
      return displayedPages

      
   def test_rightPagesDisplayed(self):
      print '=== Test navigation accros dashboard pages ==='
      print 'ref Issues : #172, #174'

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
      # Note that the navigation is not perfect, but considered as acceptable for actual Yadoms version
      # See https://github.com/Yadoms/yadoms/issues/172 for more information

      def checkPage(dashboardSubWindow, expectedPageTitles, pageIndex):
         displayedPages = self.getDisplayedPages(dashboardSubWindow)
         assert len(displayedPages) == 1
         pageTitle = displayedPages[0].get_attribute('id')
         if pageTitle == expectedPageTitles[pageIndex]:
            print '   [OK] Get expected page (' + expectedPageTitles[pageIndex] + ')'
            return True
         print '    [WARNING] Displayed page is ' + pageTitle + ', should be ' + expectedPageTitles[pageIndex]
         return False

      dashboardSubWindow = db.find_element_by_id("main-dashboard-sub-window-content")
      for page in range(0, len(menuEntries)):
         assert tools.doUntil(lambda : self.click(menuEntries, page), lambda: checkPage(dashboardSubWindow, expectedPageTitles, page), 3)


   def test_quickNavigation(self):
      print '=== Test quick navigation accros dashboard pages ==='
      print 'ref Issues : #172'

      print '  Enter dashboard'
      db = dashboard.open(self.browser)
   
      dashboard_boutons = db.find_element_by_id("dashboard-btns")
      menuEntries = dashboard_boutons.find_elements_by_xpath("./child::*")

      for page in range(0, len(menuEntries)):
         self.click(menuEntries, page)

      nbDisplayedPages = len(self.getDisplayedPages(db.find_element_by_id("main-dashboard-sub-window-content")))
      if nbDisplayedPages != 1:
         print '   [FAIL] ' + str(nbDisplayedPages) + ' pages displayed, expected 1'
         assert False

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
import sys
sys.path.append('.')
import unittest
import database
import config
import scripts
import yadomsServer
import dashboard
from selenium import webdriver

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
      
      print '  Enter dashboard'
      db = dashboard.open(self.browser)
   
      dashboard_boutons = db.find_element_by_id("dashboard-btns")
      menuEntries = dashboard_boutons.find_elements_by_xpath("./child::*")
      nbPages = len(menuEntries)

      print '   Display all dashboard pages sequentially'
      for page in range(0, nbPages):
         menuEntries[page].click()
            
      print '   Check that only last page is displayed'
      displayedContent = db.find_element_by_id("main-dashboard-sub-window-content")
      assert len(displayedContent.find_elements_by_xpath("div")) == 1
      assert displayedContent.find_elements_by_xpath("div")[0].get_attribute('id') == 'dashboard-about'
            

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
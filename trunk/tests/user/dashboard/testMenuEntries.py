import unittest
from selenium import webdriver
import tools

class MenuEntries(unittest.TestCase):
   """Check entries of the dashboard main menu"""
   
   def setUp(self):
      self.browser = webdriver.Firefox()

   def checkMenuEntry(self, entry, expectedEntryId):
      assert entry.get_attribute("id") == expectedEntryId
      assert entry.is_displayed() is True
      assert entry.is_enabled() is True
      assert entry.is_selected() is False
      
   def test_checkEntries(self):
      browser = self.browser
      browser.get("http://127.0.0.1:8080")
      self.assertIn("Yadoms", browser.title)
      
      # Enter dashboard
      tools.enterDashboard(browser)
   
      # Check summary page
      dashboard_boutons = browser.find_element_by_id("dashboard-btns")
      menuEntries = dashboard_boutons.find_elements_by_xpath("./child::*")
      assert len(menuEntries) is 8
      self.checkMenuEntry(menuEntries[0], "btn-dashboard-summary")
      self.checkMenuEntry(menuEntries[1], "btn-dashboard-system-configuration")
      self.checkMenuEntry(menuEntries[2], "btn-dashboard-plugins")
      self.checkMenuEntry(menuEntries[3], "btn-dashboard-devices")
      self.checkMenuEntry(menuEntries[4], "btn-dashboard-automatisation")
      self.checkMenuEntry(menuEntries[5], "btn-dashboard-recipients")
      self.checkMenuEntry(menuEntries[6], "btn-dashboard-install-update")
      self.checkMenuEntry(menuEntries[7], "btn-dashboard-about")
      
   def tearDown(self):
      self.browser.close()

if __name__ == "__main__":
   unittest.main()
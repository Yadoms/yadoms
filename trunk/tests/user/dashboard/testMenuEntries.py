import unittest
import yadomsServer
import dashboard
from selenium import webdriver

class MenuEntries(unittest.TestCase):
   """Check entries of the dashboard main menu"""
   
   def setUp(self):
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)

   def checkMenuEntry(self, entry, expectedEntryId):
      self.assertEqual(entry.get_attribute("id"), expectedEntryId)
      self.assertEqual(entry.is_displayed(), True)
      self.assertEqual(entry.is_enabled(), True)
      self.assertEqual(entry.is_selected(), False)
      
   def test_checkEntries(self):
      self.assertIn("Yadoms", self.browser.title)
      
      # Enter dashboard
      dashboard.open(self.browser)
   
      # Check summary page
      dashboard_boutons = self.browser.find_element_by_id("dashboard-btns")
      menuEntries = dashboard_boutons.find_elements_by_xpath("./child::*")
      self.assertEqual(len(menuEntries), 8)
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
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
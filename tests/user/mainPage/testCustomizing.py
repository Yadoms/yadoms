import sys
sys.path.append('.')
import unittest
from selenium import webdriver
import database
import config
import scripts
import yadomsServer
import mainPage.widgets
import time

class Customizing(unittest.TestCase):
   """Customizing test"""
   
   def setUp(self):
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
            
            
   def test_customizing(self):
      print '=== Entering/Exiting customization test ==='
      
      print 'Enter customizing mode'
      mainPage.widgets.enterCustomizingMode(self.browser)
      
      print 'Exit customizing mode'
      mainPage.widgets.exitCustomizingMode(self.browser)
            
            
   def test_uncustomizingAfterModal(self):
      print '=== Test exit customization just after closing a modal ==='
      
      # Exiting custom mode can be sometimes problematic, so test 10 times
      # to be sure it works well
      for i in range (0, 10):
         print 'Enter customizing mode'
         mainPage.widgets.enterCustomizingMode(self.browser)
         
         print 'Open add widget modal'
         mainPage.widgets.getAddWidgetButton(self.browser).click()
         newWidgetModal = mainPage.widgets.waitNewWidgetModal(self.browser)
         print 'Cancel add widget modal'
         newWidgetModal.cancel()
         
         print 'Immediately exit customizing mode'
         mainPage.widgets.exitCustomizingMode(self.browser)
      
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
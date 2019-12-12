import sys
sys.path.append('.')
import unittest
from selenium import webdriver
import database
import config
import scripts
import yadomsServer
import mainPage
import time
import tools

class AddPage(unittest.TestCase):
   """App page test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.new()
      config.deploy("nominal")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome(options=tools.ChromeOptionsHelper.get())
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
            
            
   def test_cancelAddPage(self):
      print ('=== Entering/Exiting customization test ===')
      
      print ('Enter customizing mode')
      mainPage.enterCustomizingMode(self.browser)
         
      print ('Open add page modal')
      mainPage.getPagesMenuBar(self.browser).getAddPageButton().click()
      addPageModal = mainPage.waitAddPageModal(self.browser)
      print ('Cancel add page modal')
      addPageModal.cancel()
      
      print ('Exit customizing mode')
      mainPage.exitCustomizingMode(self.browser)
            
            
   def test_addPage(self):
      print ('=== Add a page test ===')
      newPageName = "New page"
      
      print ('Enter customizing mode')
      mainPage.enterCustomizingMode(self.browser)
         
      print ('Open add page modal')
      pagesMenuBar = mainPage.getPagesMenuBar(self.browser)
      pagesMenuBar.getAddPageButton().click()
      addPageModal = mainPage.waitAddPageModal(self.browser)

      print ('Create the page')
      addPageModal.enterPageName(newPageName)
      addPageModal.ok()

      print ('Check page was created (customizing mode)')
      self.assertEqual(pagesMenuBar.getPagesCount(), 2)
      self.assertEqual(pagesMenuBar.getPage(1).getName(), newPageName)      
      
      print ('Exit customizing mode')
      mainPage.exitCustomizingMode(self.browser)
      
      print ('Check page was created (normal mode)')
      pagesMenuBar = mainPage.getPagesMenuBar(self.browser)
      self.assertEqual(pagesMenuBar.getPagesCount(), 2)
      self.assertEqual(pagesMenuBar.getPage(1).getName(), newPageName)

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
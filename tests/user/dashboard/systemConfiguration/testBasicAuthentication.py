import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
import configurationPanel
import database
import config
import notification
import scripts
import yadomsServer
import dashboard.systemConfiguration
import tools
import mainPage
import i18n

class BasicAuthentication(unittest.TestCase):
   """Basic authentication test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.new()
      config.deploy("nominal")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)

      
   def test_testSetBasicAuthentication(self):
      print '=== Set a basic authentication ==='

      print 'Open systemConfiguration dashboard'
      dashboard.open(self.browser)
      dashboard.openSystemConfiguration(self.browser)

      print 'Check that basic authentication is disabled (cold start)'
      panel = dashboard.systemConfiguration.getPanel(self.browser)
      self.assertFalse(panel.isBasicAuthenticationSectionActive())
      
      print 'Enable basic authentication section'      
      panel.enableBasicAuthenticationSection()
      
      print 'Check that Apply is not possible without password'
      panel.apply()
      passwordField = panel.getBasicAuthenticationPasswordField()
      self.assertTrue(tools.waitUntil(lambda: "has-error" in passwordField.get_attribute("class")))
      self.assertEqual(passwordField.find_element_by_xpath(".//div[@class='help-block']//ul[@role='alert']//li").text, i18n.get()["configuration"]["validationForm"]["required"])
      
      print 'Check that verify password is needed'
      panel.setBasicAuthenticationPassword("yadoms1234");
      panel.apply()
      verifyPasswordField = panel.getBasicAuthenticationVerifyPasswordField()
      self.assertTrue(tools.waitUntil(lambda: "has-warning" in verifyPasswordField.get_attribute("class")))
      self.assertEqual(verifyPasswordField.find_element_by_xpath(".//div[@class='help-block']//ul[@role='alert']//li").text, i18n.get()["modals"]["dashboard"]["sub-windows"]["system-configuration"]["configuration-items"]["basicAuthenticationPassword2"]["matchToErrorMessage"])

      print 'Check that password fields must be the same'
      panel.setBasicAuthenticationVerifyPassword("yadoms123");
      panel.apply()
      verifyPasswordField = panel.getBasicAuthenticationVerifyPasswordField()
      self.assertTrue(tools.waitUntil(lambda: "has-warning" in verifyPasswordField.get_attribute("class")))
      self.assertEqual(verifyPasswordField.find_element_by_xpath(".//div[@class='help-block']//ul[@role='alert']//li").text, i18n.get()["modals"]["dashboard"]["sub-windows"]["system-configuration"]["configuration-items"]["basicAuthenticationPassword2"]["matchToErrorMessage"])

      print 'Check that password fields must be the same'
      panel.setBasicAuthenticationVerifyPassword("yadoms1234");
      self.assertTrue(tools.waitUntil(lambda: "has-warning" not in verifyPasswordField.get_attribute("class")))

      print 'Valid new authentication'
      panel.apply()

      # Applying these modification make Yadoms client to be refreshed (by Yadoms design)
      # It's probably because this that we can't access alert dialog (Selenium alert API doesn't work)
      
      print 'Check that requests without credentials are refused'
      self.assertTrue(tools.waitUntil(lambda: yadomsServer.waitServerStarted(clientCredentials = None, timemoutSeconds = 1) == False))

      print 'Check that it works with credentials'
      self.assertTrue(yadomsServer.waitServerStarted(clientCredentials = { "user" : "admin", "password" : "yadoms1234" }))

      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
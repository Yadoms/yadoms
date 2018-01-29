import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition

   
def getShowDashboardButton(browser):
   return browser.find_element_by_id("btn-show-dashboard")
      
      
def get(browser):
   """ Return the dashboard WebElement """
   return browser.find_element_by_id("main-dashboard-modal")


def open(browser):
   """Open the dashboard and wait until displayed"""
   
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "btn-show-dashboard")))
   getShowDashboardButton(browser).click()
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "main-dashboard-modal")))
   return get(browser)


def close(browser):
   """Close the dashboard"""
   
   browser.find_element_by_xpath(".//div[@id='main-dashboard-modal']//div[@class='modal-footer']//button[@data-i18n='common.close']").click();
   WebDriverWait(browser, 10).until(Condition.invisibility_of_element_located((By.ID, "main-dashboard-modal")))


def openPage(browser, pageElementId, elementIdToWait):
   """Open a specific dashboard page"""
   
   # Note that the navigation is not perfect, but considered as acceptable for actual Yadoms version
   # See testNavigationAccrossPages.py and https://github.com/Yadoms/yadoms/issues/172 for more information
   # So the ideal code should be :
   #
   #browser.find_element_by_id(pageElementId).click()
   #WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, elementIdToWait)))
   #
   # But here we don't want to be blocked here, so this is a workaround :
   button = browser.find_element_by_id(pageElementId)
   retries = 10
   while(retries > 0):
      button.click()
      foundElements = browser.find_elements_by_xpath(".//div[@id='main-dashboard-sub-window-content']//*[@id='" + elementIdToWait + "']")
      if len(foundElements) and foundElements[0].is_displayed():
         return
      retries = retries - 1
   assert False

   
def openSummary(browser):
   """Open the summary page of dashboard"""
   
   openPage(browser, "btn-dashboard-summary", "dashboard-summary")

   
def openSystemConfiguration(browser):
   """Open the system configuration page of dashboard"""
   
   openPage(browser, "btn-dashboard-system-configuration", "dashboard-system-configuration")

   
def openAutomation(browser):
   """Open the automation page of dashboard"""
   
   openPage(browser, "btn-dashboard-automatisation", "automation-rule-list")

   
def openPlugin(browser):
   """Open the plugin page of dashboard"""
   
   openPage(browser, "btn-dashboard-plugins", "plugin-instance-list")

   
def openDevice(browser):
   """Open the device page of dashboard"""
   
   openPage(browser, "btn-dashboard-devices", "device-list")
   
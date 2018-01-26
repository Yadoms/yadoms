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
   
   browser.find_element_by_id(pageElementId).click()
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, elementIdToWait)))

   
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
   
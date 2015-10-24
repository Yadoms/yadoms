﻿import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition


def isShowDashboardButtonAvailable(browser):
   """Check if the dashboard button is available"""
   if (not getShowDashboardButton(browser).is_displayed()):
      # Sometimes first click on drop-down button is lost, so retry...
      getDropDownButton(browser).click()
      return False;
   return True;

   
def getDropDownButton(browser):
   return browser.find_element_by_id("dropDownButton")


def getShowDashboardButton(browser):
   return browser.find_element_by_id("btn-show-dashboard")
      
      
def get(browser):
   """ Return the dashboard WebElement """
   return browser.find_element_by_id("main-dashboard-modal")


def open(browser):
   """Open the dashboard and wait until displayed"""
   
   WebDriverWait(browser, 10).until(Condition.visibility_of(getDropDownButton(browser)))
   getDropDownButton(browser).click()
   WebDriverWait(browser, 10).until(lambda browser: isShowDashboardButtonAvailable(browser))
   getShowDashboardButton(browser).click()
   WebDriverWait(browser, 10).until(Condition.visibility_of(get(browser)))
   return get(browser)

   
def openAutomation(browser):
   """Open the automation page of dashboard"""
   
   automationItem = browser.find_element_by_id("btn-dashboard-automatisation")
   automationItem.click()

   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "automation-rule-list")))
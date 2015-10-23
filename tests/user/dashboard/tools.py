import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition


def enterDashboard(browser):
   """Enter the dashboard and wait until displayed"""
   
   browser.find_element_by_id("dropDownButton").click()
   browser.find_element_by_id("btn-show-dashboard").click()

   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "main-dashboard-modal")))

def selectAutomation(browser):
   """Select the automation item in dashboard"""
   
   browser.find_element_by_id("btn-dashboard-automatisation").click()

   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "automation-rule-list")))

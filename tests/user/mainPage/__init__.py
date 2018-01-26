from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import tools
import i18n
import modals




def getPagesMenuBar(browser):
   return PageMenuBar(browser.find_element_by_xpath(".//div[@id='pageMenu']"))
   
def waitAddPageModal(browser):
   modal = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//div[@id='modify-page-modal']")))
   if modal.find_element_by_xpath(".//h4[@class='modal-title']").text == i18n.get()["modals"]["modify-page"]["createNewPageTitle"]:
      return AddPageModal(modal)
   assert False


class PageMenuBar():
   """ Operations on the pages menu bar """
   
   def __init__(self, barWebElement):
      self.__bar = barWebElement
   
   def getAddPageButton(self):
      return self.__bar.find_element_by_xpath(".//i[@class='fa fa-plus']")

   def getPagesCount(self):
      return len(self.__bar.find_elements_by_xpath(".//li[contains(@class, 'tabPagePills')]"))

   def getPage(self, index):
      return PageMenuBarTab(self.__bar.find_element_by_xpath(".//li[contains(@page-id, '" + str(index + 1) + "')]"))


class PageMenuBarTab():
   """ Operations on a tab of the pages menu bar """
   
   def __init__(self, tabWebElement):
      self.__tab = tabWebElement
   
   def getName(self):
      return self.__tab.find_element_by_xpath(".//a[@data-toggle='tab']//span").text


class AddPageModal():
   """ Operations on add page modal """
   
   def __init__(self, modalWebElement):
      self.__modal = modalWebElement

   def enterPageName(self, pageName):
      item = self.__modal.find_element_by_xpath(".//input[@id='modify-page-text']")
      item.send_keys(Keys.CONTROL + 'a')
      item.send_keys(Keys.DELETE)
      item.send_keys(pageName)

   def getConfirmButton(self):
      return self.__modal.find_element_by_xpath(".//button[@id='btn-confirm-modify-page']")
         
   def getCancelButton(self):
      return self.__modal.find_element_by_xpath(".//button[@data-i18n='common.cancel']")
      
   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__modal)
      
   def cancel(self):
      self.getCancelButton().click()
      modals.waitForClosed(self.__modal)

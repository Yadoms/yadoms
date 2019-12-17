from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import tools
import i18n
import modals
import re


def getCustomizeButton(browser):
   return browser.find_element_by_xpath(".//div[@class='navbar-right']//a[@id='customizeButton']")

def isCustomizing(browser):
   pageTabsElement = browser.find_element_by_class_name('page-tabs')
   customizableElement = pageTabsElement.find_element_by_class_name('customization-item')
   return customizableElement.is_displayed()

def enterCustomizingMode(browser):
   if not isCustomizing(browser):
      getCustomizeButton(browser).click()
      assert tools.waitUntil(lambda: isCustomizing(browser))

def exitCustomizingMode(browser):
   if isCustomizing(browser):
      getCustomizeButton(browser).click()
      assert tools.waitUntil(lambda: not isCustomizing(browser))



def getAddWidgetButton(browser):
   return browser.find_element_by_id("btn-add-widget")


def waitNewWidgetModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "new-pickup-modal")))
   modal = browser.find_element_by_id("new-pickup-modal")
   if modal.find_element_by_id("myModalLabel") and modal.find_element_by_id("myModalLabel").get_attribute("data-i18n") == "modals.add-widget.title":
      return NewWidgetModal(browser.find_element_by_id("new-pickup-modal"))
   assert False   
   
def waitConfigureWidgetModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "configure-widget-modal")))
   return ConfigureWidgetModal(browser.find_element_by_id("configure-widget-modal"))

def getCurrentPage(browser):
   pagesContainer = browser.find_element_by_id("tabContainer")
   return pagesContainer.find_element_by_css_selector('div.widgetPage.active')
   
def getWidgetGrid(browser, page):
   return page.find_element_by_class_name("grid")
   
def listWidgetsOnCurrentPage(browser):
   widgetContainer = getWidgetGrid(getCurrentPage(browser), getCurrentPage(browser)).find_elements_by_class_name("widget")
   widgets = []
   for widget in widgetContainer:
      widgets.append(Widget(widget))
   return widgets
   
def findWidgetOnCurrentPage(browser, widgetType, widgetTitle):
   widgets = listWidgetsOnCurrentPage(browser)
   for widget in widgets:
      if (widget.getType() == widgetType and widget.getTitle() == widgetTitle):
         return widget

   # Not found
   return None  



class NewWidgetModal():
   """ Operations on new widget modal (widget selection) """
   
   def __init__(self, newWidgetModalWebElement):
      self.__modal = modals.PickupSelectorModal(newWidgetModalWebElement)

   def selectWidget(self, expectedWidgetName):
      item = self.__modal.selectItem(expectedWidgetName)
      if item is not None:
         return item
      print ('selectWidget : Nothing to select, {expectedWidgetName} not found')
      assert False

   def getConfirmButton(self):
      return self.__modal.getConfirmButton()
         
   def getCancelButton(self):
      return self.__modal.getCancelButton()
      
   def clickAdd(self):
      return self.__modal.clickAdd()
      
   def cancel(self):
      return self.__modal.cancel()



class ConfigureWidgetModal():
   """ Operations on widget configuration modal """
   
   def __init__(self, configureWidgetModalWebElement):
      self.__configureWidgetModalWebElement = configureWidgetModalWebElement
      self.__configurationPanel = ConfigurationPanel(self.__configureWidgetModalWebElement)
      
   def getConfigurationPanel(self):
      return self.__configurationPanel    

   def getWidgetTitle(self):
      return self.__configurationPanel.getTextItemByName('modals.configure-widget.titleName.name')
      
   def setWidgetTitle(self, newTitle):
      titleField = self.getWidgetTitle()
      titleField.send_keys(Keys.CONTROL + "a")
      titleField.send_keys(Keys.DELETE)
      titleField.send_keys(newTitle)
         
   def getConfirmButton(self):
      return self.__configureWidgetModalWebElement.find_element_by_id("btn-confirm-configure-widget")

   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__configureWidgetModalWebElement)
      
      
class Widget():
   """ Class representing a displayed widget """
   
   def __init__(self, widgetWebElement):
      self.__widgetWebElement = widgetWebElement 

   def getType(self):
      classes = self.__widgetWebElement.find_element_by_class_name('panel-widget').get_attribute('class').split()
      for classItem in classes:
         if "widget-" in classItem:
            return re.search('widget-(.*)', classItem).group(1)
      assert False
      
   def getTitle(self):
      return self.__widgetWebElement.find_element_by_class_name('panel-widget-title').text
      




def getPagesMenuBar(browser):
   return PageMenuBar(browser.find_element_by_xpath(".//div[@id='pageMenu']"))
   
def waitAddPageModal(browser):
   modal = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.XPATH, ".//div[@id='modify-page-modal']")))
   if modal.find_element_by_xpath(".//h4[@class='modal-title']").text == i18n.get()["modals"]["modify-page"]["createNewPageTitle"]:
      return AddPageModal(modal)
   assert False

def addPage(browser, pageName):
   """ High-level function to add a page """
   customizeMode = isCustomizing(browser)
   if not customizeMode:
      enterCustomizingMode(browser)
   getPagesMenuBar(browser).getAddPageButton().click()
   addPageModal = waitAddPageModal(browser)
   addPageModal.enterPageName(pageName)
   addPageModal.ok()
   if not customizeMode:
      exitCustomizingMode(browser)


class PageMenuBar():
   """ Operations on the pages menu bar """
   
   def __init__(self, barWebElement):
      self.__bar = barWebElement
   
   def getAddPageButton(self):
      return self.__bar.find_element_by_xpath(".//i[@class='fa fa-plus']")

   def getPagesCount(self):
      return len(self.__bar.find_elements_by_xpath(".//li[contains(@class, 'tabPagePills')]"))

   def findPage(self, name):
      for indexPage in range(self.getPagesCount()):
         page = self.getPage(indexPage)
         if page.getName() == name:
            return page
      return None

   def selectPage(self, name):
      self.findPage(name).select()

   def getPage(self, index):
      return PageMenuBarTab(self.__bar.find_element_by_xpath(".//li[contains(@page-id, '" + str(index + 1) + "')]"))

   def getCurrentPage(self):
      return PageMenuBarTab(self.__bar.find_element_by_xpath(".//li[@class='tabPagePills ui-droppable active']"))


class PageMenuBarTab():
   """ Operations on a tab of the pages menu bar """
   
   def __init__(self, tabWebElement):
      self.__tab = tabWebElement
   
   def getName(self):
      return self.__tab.find_element_by_xpath(".//a[@data-toggle='tab']//span").text

   def select(self):
      self.__tab.find_element_by_xpath(".//a[@data-toggle='tab']").click()


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

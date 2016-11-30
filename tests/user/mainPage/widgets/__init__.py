from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import tools
import re
import modals

   
def getCustomizeButton(browser):
   return browser.find_element_by_id("customizeButton")


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
      print "selectWidget : Nothing to select, ", expectedWidgetName, " not found"
      assert False

   def getConfirmButton(self):
      return self.__modal.getConfirmButton()
         
   def getCancelButton(self):
      return self.__modal.getCancelButton()
      
   def ok(self):
      return self.__modal.ok()
      
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
      return self.__configurationPanel.getItemByName('modals.configure-widget.titleName.name')
      
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
      
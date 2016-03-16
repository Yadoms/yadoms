from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import tools
import re

   
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
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "new-widget-modal")))
   return NewWidgetModal(browser.find_element_by_id("new-widget-modal"))
   
   
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
       self.__newWidgetModalWebElement = newWidgetModalWebElement

   def selectWidget(self, expectedWidgetName):
      widgetList = WebDriverWait(self.__newWidgetModalWebElement, 10).until(Condition.visibility_of_element_located((By.ID, "ul-add-widget")))
      for item in widgetList.find_elements_by_tag_name('button'):
         if item.get_attribute('widget-type') == expectedWidgetName:
            return item
      print "selectWidget : Nothing to select, ", expectedWidgetName, " not found"
      assert False
      
   def waitForClosed(self):
      assert tools.waitUntil(lambda: 'display: none;' in self.__newWidgetModalWebElement.get_attribute('style'))

   def getConfirmButton(self):
      WebDriverWait(self.__newWidgetModalWebElement, 10).until(Condition.visibility_of_element_located((By.ID, "btn-confirm-add-widget")))
      return self.__newWidgetModalWebElement.find_element_by_id("btn-confirm-add-widget")
         
   def getCloseButton(self):
      buttons = self.__newWidgetModalWebElement.find_elements_by_tag_name('button')
      for button in buttons:
         if button.get_attribute('data-i18n') is not None and 'common.close' in button.get_attribute('data-i18n'):
            return button
      assert False



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
      
   def waitForClosed(self):
      assert tools.waitUntil(lambda: 'display: none;' in self.__configureWidgetModalWebElement.get_attribute('style'))

   def ok(self):
      self.getConfirmButton().click()
      self.waitForClosed()
      
      
class Widget():
   """ Class representing a displayed widget """
   
   def __init__(self, widgetWebElement):
      self.__widgetWebElement = widgetWebElement 

   def getType(self):
      widgetBody = self.__widgetWebElement.find_element_by_class_name('panel-widget-body')
      linkElement = widgetBody.find_element_by_tag_name('link')
      hrefValue = linkElement.get_attribute('href')
      return re.search('widgets/(.*)/widget.css', hrefValue).group(1)
      
   def getTitle(self):
      return self.__widgetWebElement.find_element_by_class_name('panel-widget-title').text
      
   def getPosition(self):
      return [int(self.__widgetWebElement.get_attribute('data-gs-x')), int(self.__widgetWebElement.get_attribute('data-gs-y'))]

   def getSize(self):
      return [int(self.__widgetWebElement.get_attribute('data-gs-width')), int(self.__widgetWebElement.get_attribute('data-gs-height'))]
      
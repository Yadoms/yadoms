from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import modals
import i18n
import tools


""" Operations on plugins dashboard page """

def getCreatePluginButton(browser):
   return browser.find_element_by_id("btn-add-new-plugin")
   
def waitNewPluginModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "new-plugin-modal")))
   return NewPluginModal(browser.find_element_by_id("new-plugin-modal"))
   
      
def waitPluginsTable(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "plugin-instance-list")))
   return browser.find_element_by_id("plugin-instance-list")

def waitPluginsTableHasNPlugins(browser, pluginsNumberExpected):
   pluginsTable = waitPluginsTable(browser)
   assert tools.waitUntil(lambda: getPluginNumberInTable(browser, pluginsTable) == pluginsNumberExpected)
   return pluginsTable

def getPluginNumberInTable(browser, pluginsTable):
   return len(pluginsTable.find_elements_by_tag_name("tr")) - 1
   
def getPluginDatas(pluginsTable, pluginNumber):
   plugin = pluginsTable.find_elements_by_tag_name("tr")[pluginNumber + 1]
   return plugin.find_elements_by_tag_name("td")

def getPluginName(pluginsTable, pluginNumber):
   return getPluginDatas(pluginsTable, pluginNumber)[0].text

def getPluginType(pluginsTable, pluginNumber):
   return getPluginDatas(pluginsTable, pluginNumber)[1].text
   
def getPluginAutoStart(pluginsTable, pluginNumber):
   return getPluginDatas(pluginsTable, pluginNumber)[2].find_element_by_tag_name("input").is_selected()
   
def getPluginButtons(pluginsTable, pluginNumber):
   pluginsActionsButtonsCell = getPluginDatas(pluginsTable, pluginNumber)[3]
   buttonsGroup = pluginsActionsButtonsCell.find_element_by_class_name("btn-group")
   return buttonsGroup.find_elements_by_tag_name("button")
   
def getPluginButton(pluginsTable, pluginNumber, index):
   buttons = getPluginButtons(pluginsTable, pluginNumber)
   return buttons[index]
   
def getPluginStartStopButton(pluginsTable, pluginNumber):
   buttons = getPluginButtons(pluginsTable, pluginNumber)
   for button in buttons:
      if "btn-startStop" in button.get_attribute("class"):
         return button
   assert False, "Start/Stop button not found"
   
def getPluginConfigureButton(pluginsTable, pluginNumber):
   buttons = getPluginButtons(pluginsTable, pluginNumber)
   for button in buttons:
      if "btn-configure" in button.get_attribute("class"):
         return button
   assert False, "Configure button not found"
   
def getPluginExtraCommandButton(pluginsTable, pluginNumber):
   buttons = getPluginButtons(pluginsTable, pluginNumber)
   for button in buttons:
      if "btn-extraCommand" in button.get_attribute("class"):
         return button
   assert False, "Extra command button not found"
   
def getPluginRemoveButton(pluginsTable, pluginNumber):
   buttons = getPluginButtons(pluginsTable, pluginNumber)
   for button in buttons:
      if "btn-delete" in button.get_attribute("class"):
         return button
   assert False, "Delete button not found"
   

def waitConfigurePluginModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "configure-plugin-modal")))
   return ConfigurePluginModal(browser.find_element_by_id("configure-plugin-modal"))

def waitRemovePluginConfirmationModal(browser):
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "confirmation-modal")))
   return RemovePluginConfirmationModal(browser.find_element_by_id("confirmation-modal"))
   
   
   
class PluginState:
   Error, Stopped, Running, Custom, Unknown = range(5)
   
def getPluginState(pluginsTable, pluginNumber):
   pluginStateCell = getPluginDatas(pluginsTable, pluginNumber)[4]
   labelStatusClasses = pluginStateCell.find_element_by_class_name("label-status").get_attribute("class")
   labelStatusText = pluginStateCell.find_element_by_class_name("label-status").text
   if "label-danger" in labelStatusClasses:
      return PluginState.Error
   if "label-warning" in labelStatusClasses:
      if labelStatusText == i18n.get()["modals"]["dashboard"]["sub-windows"]["plugins"]["stopped"]:
         return PluginState.Stopped
      else:
         return PluginState.Custom
   if "label-success" in labelStatusClasses:
      return PluginState.Running
   assert False   

   
   
class NewPluginModal():
   """ Operations on new plugin modal (plugin selection) """
   
   def __init__(self, newPluginModalWebElement):
       self.__newPluginModalWebElement = newPluginModalWebElement

   def selectPlugin(self, expectedPluginName):
      select = WebDriverWait(self.__newPluginModalWebElement, 10).until(Condition.visibility_of_element_located((By.ID, "pluginTypeList")))
      for option in select.find_elements_by_tag_name('option'):
         if option.text == expectedPluginName:
            return option
      print "selectPlugin : Nothing to select, ", expectedPluginName, " not found"
      assert False
         
   def getConfirmButton(self):
      return self.__newPluginModalWebElement.find_element_by_id("btn-confirm-add-plugin")
         
   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__newPluginModalWebElement)
            
            
            

class RemovePluginConfirmationModal(modals.RemoveObjectConfirmationModal):
   """ Operations on delete plugin confirmation modal """
   pass
   



class ConfigurePluginModal():
   """ Operations on plugin configuration modal """
   
   def __init__(self, configurePluginModalWebElement):
      self.__configurePluginModalWebElement = configurePluginModalWebElement

   def getPluginName(self):
      return ConfigurationPanel(self.__configurePluginModalWebElement).getItemByName("modals.configure-plugin.name-configuration.name")
      
   def setPluginName(self, newName):
      tools.waitReadyForInput(self.getPluginName())
      nameField = self.getPluginName()
      nameField.send_keys(Keys.CONTROL + "a")
      nameField.send_keys(Keys.DELETE)
      nameField.send_keys(newName)
         
   def getConfirmButton(self):
      return self.__configurePluginModalWebElement.find_element_by_id("btn-confirm-configure-plugin")
         
   def ok(self):
      self.getConfirmButton().click()
      modals.waitForClosed(self.__configurePluginModalWebElement)
      

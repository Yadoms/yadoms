import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
import database
import config
import configurationPanel
import i18n
import modals
import os
import notification
import yadomsServer
import dashboard
import dashboard.plugins
import tools

class ExtraQuery(unittest.TestCase):
   """Extra queries test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
      
      # Open plugins dashboard
      dashboard.open(self.browser)
      dashboard.openPlugin(self.browser)


   class ExtraQueryParameterModal():
      """ Operations on extra query modal """
   
      def __init__(self, modalWebElement):
          self.__modalWebElement = modalWebElement

      def __panel(self):
         return configurationPanel.ConfigurationPanel(self.__modalWebElement)

      def __fieldValueElement(self, fieldName):
         field = self.__panel().findField(fieldName)
         return field.find_element_by_xpath(".//div[contains(@class, 'control-label')]/*[contains(@class, 'form-control')]")
         
      def __getConfirmButton(self):
         return self.__modalWebElement.find_element_by_xpath(".//button[@id='btn-confirm-plugin-extra-queries-configure']")

      def ok(self):
         self.__getConfirmButton().click()
         modals.waitForClosed(self.__modalWebElement)         

      def setTextField(self, fieldName, fieldValue):
         fieldValueElement = self.__fieldValueElement(fieldName)
         fieldValueElement.send_keys(Keys.CONTROL + 'a')
         fieldValueElement.send_keys(Keys.DELETE)
         fieldValueElement.send_keys(fieldValue) 

      def setEnumField(self, fieldName, enumIndex):
         fieldValueElement = self.__fieldValueElement(fieldName)
         Select(fieldValueElement).select_by_index(enumIndex)  

      def setFileField(self, fieldName, path):
         fieldValueElement = self.__fieldValueElement(fieldName)
         fieldValueElement.send_keys(path) 

   
   def waitExtraQueryParameterModal(self):
      WebDriverWait(self.browser, 10).until(Condition.visibility_of_element_located((By.ID, 'plugin-extra-queries-configure-modal')))
      modals.waitForOpened(self.browser.find_element_by_id('plugin-extra-queries-configure-modal'))
      return self.ExtraQueryParameterModal(self.browser.find_element_by_id('plugin-extra-queries-configure-modal'))
      

   def test_simpleQuery(self):
      print ('=== Simple query test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-simpleCommand']").click()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["simpleCommand"]["name"]))
      

   def test_textDataQuery(self):
      print ('=== Query with text data test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-textDataCommand']").click()

      modal = self.waitExtraQueryParameterModal()
      modal.setTextField("plugins.dev-fakePlugin:extraQueries.textDataCommand.commandData.testValue", "test data")
      modal.ok()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["textDataCommand"]["name"]))
      

   def test_numericDataQuery(self):
      print ('=== Query with numeric data test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-numericDataCommand']").click()

      modal = self.waitExtraQueryParameterModal()
      modal.setTextField("plugins.dev-fakePlugin:extraQueries.numericDataCommand.commandData.testValue", "123")
      modal.ok()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["numericDataCommand"]["name"]))
      

   def test_dataBindingQuery(self):
      print ('=== Query with data binding test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-dataBindingCommand']").click()

      modal = self.waitExtraQueryParameterModal()
      modal.setEnumField("plugins.dev-fakePlugin:extraQueries.dataBindingCommand.commandData.networkInterface", 0)
      modal.ok()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["dataBindingCommand"]["name"]))
      

   def test_dataBindingPluginCommandQuery(self):
      print ('=== Query with data binding with plugin test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-dataBindingPluginCommand']").click()

      modal = self.waitExtraQueryParameterModal()
      modal.setEnumField("plugins.dev-fakePlugin:extraQueries.dataBindingPluginCommand.commandData.dynamicSection", 0)
      modal.ok()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["dataBindingPluginCommand"]["name"]))
      

   def test_changePluginStateMessageQuery(self):
      print ('=== Query of change plugin state test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-changePluginStateMessage']").click()

      modal = self.waitExtraQueryParameterModal()
      modal.setTextField("plugins.dev-fakePlugin:extraQueries.changePluginStateMessage.commandData.newStateMessage", "message associated to plugin state")
      modal.ok()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["changePluginStateMessage"]["name"]))
      

   def test_asyncEQwithProgressionQuery(self):
      print ('=== Query with progress following test ===')
      pluginNumber = 0
     
      print ('  Select query')
      pluginsTable = dashboard.plugins.waitPluginsTableHasNPlugins(self.browser, 1)
      tools.waitUntil(lambda: dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber).is_enabled())
      button = dashboard.plugins.getPluginExtraCommandButton(pluginsTable, pluginNumber)
      button.click()
      
      select = button.find_element_by_xpath(".//following-sibling::ul[@class='dropdown-menu dropdown-extraqueries']")
      select.find_element_by_xpath(".//li/a[@class='btn-extraQuery-asyncEQwithProgression']").click()

      modal = self.waitExtraQueryParameterModal()
      modal.setFileField("plugins.dev-fakePlugin:extraQueries.asyncEQwithProgression.commandData.fileContent", __file__)
      modal.ok()

      notification.waitText(self.browser,
         notification.Type.Success,
         i18n.get()["core"]["plugins"]["extraquery"]["success"].replace("{{extraquery}}", i18n.getPlugin("dev-fakePlugin")["extraQueries"]["asyncEQwithProgression"]["name"]),
         20)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
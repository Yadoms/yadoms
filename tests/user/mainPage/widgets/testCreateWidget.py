import sys
sys.path.append('.')
import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
import database
import config
import scripts
import yadomsServer
import mainPage
import i18n
import tools
import os.path
import notification
import time

class CreateWidget(unittest.TestCase):
   """Create widget test"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      database.deploy('OneFakePlugin')
      config.deploy("withDeveloperMode")
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Chrome(options=tools.ChromeOptionsHelper.get())
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
                        
   def test_createSwitchWidget(self):
      print ('=== Nominal test of switch widget creation ===')
      self.doTest(
         "switch",
         "My switch widget",
         lambda editWidgetModal, widgetTitle: self.configureSwitchWidget(editWidgetModal, widgetTitle, 'fakeOnOffReadWriteSwitch', 'Switch'),
         lambda widgetType, widgetTitle: self.checkCreateWidget(widgetType, widgetTitle, [0, 0], [1, 1]))
      
   def configureSwitchWidget(self, editWidgetModal, widgetTitle, device, keyword):
      print ('  Change widget title')
      editWidgetModal.setWidgetTitle(widgetTitle)
      print ('  Select device')
      selects = editWidgetModal.getConfigurationPanel().getItemsByName('widgets.switch:configurationSchema.device.name')
      Select(selects[0]).select_by_visible_text(device)
      Select(selects[1]).select_by_visible_text(keyword)
      print ('  Confirm')
      editWidgetModal.ok()

   def checkCreateWidget(self, widgetType, widgetTitle, widgetExpectedPosition, widgetExpectedSize):
      print ('  Check no notification occurs')
      self.assertTrue(notification.noNotification(self.browser))
      print ('  Check widget present on main page')
      self.assertTrue(tools.waitUntil(lambda: mainPage.findWidgetOnCurrentPage(self.browser, widgetType, widgetTitle) is not None))
         
   def test_createGaugeWidget(self):
      print ('=== Nominal test of gauge widget creation ===')
      self.doTest(
         "gauge",
         "My gauge widget",
         lambda editWidgetModal, widgetTitle: self.configureGaugeWidget(editWidgetModal, widgetTitle, 'fakeSensor2', 'temp1'),
         lambda widgetType, widgetTitle: self.checkCreateWidget(widgetType, widgetTitle, [0, 0], [2, 2]))
      
   def configureGaugeWidget(self, editWidgetModal, widgetTitle, device, keyword):
      print ('  Change widget title')
      editWidgetModal.setWidgetTitle(widgetTitle)
      print ('  Select device')
      selects = editWidgetModal.getConfigurationPanel().getItemsByName('widgets.gauge:configurationSchema.device.name')
      Select(selects[0]).select_by_visible_text(device)
      Select(selects[1]).select_by_visible_text(keyword)
      print ('  Confirm')
      editWidgetModal.ok()

   def test_createChartWidgetWithOneDevice(self):
      print ('=== Nominal test of chart widget creation - one device ===')
      self.doTest(
         "chart",
         "My chart widget",
         lambda editWidgetModal, widgetTitle: self.configureChartWidgetOneDevice(editWidgetModal, widgetTitle, 'fakeSensor2', 'temp1'),
         lambda widgetType, widgetTitle: self.checkCreateWidget(widgetType, widgetTitle, [0, 0], [2, 2]))
      
   def configureChartWidgetOneDevice(self, editWidgetModal, widgetTitle, device, keyword):
      print ('  Change widget title')
      editWidgetModal.setWidgetTitle(widgetTitle)
      
      print ('  Select device')
      editWidgetModal.getConfigurationPanel().getItemButton().click()
      selects = editWidgetModal.getConfigurationPanel().getItemListByNameAndPosition('widgets.chart:configurationSchema.devices.item.content.source.name', 1)
      
      Select(selects[0]).select_by_visible_text(device)
      Select(selects[1]).select_by_visible_text(keyword)
      print ('  Confirm')
      editWidgetModal.ok()
      
   def test_createChartWidgetWithTwoDevices(self):
      print ('=== Nominal test of chart widget creation - two devices - item button ===')
      self.doTest(
         "chart",
         "My chart widget",
         lambda editWidgetModal, widgetTitle: self.configureChartWidgetTwoDevices(editWidgetModal, widgetTitle, 'fakeSensor2', 'temp1', 'fakeSensor1', 'temp2'),
         lambda widgetType, widgetTitle: self.checkCreateWidget(widgetType, widgetTitle, [0, 0], [2, 2]))
      
   def configureChartWidgetTwoDevices(self, editWidgetModal, widgetTitle, device1, keyword1, device2, keyword2):
      print ('  Change widget title')
      editWidgetModal.setWidgetTitle(widgetTitle)
      
      chemin = 'widgets.chart:configurationSchema.devices.item.content.source.name'
      
      print ('  Select first device')
      editWidgetModal.getConfigurationPanel().getItemButton().click()
      selects = editWidgetModal.getConfigurationPanel().getItemListByNameAndPosition(chemin, 1)
      
      Select(selects[0]).select_by_visible_text(device1)
      Select(selects[1]).select_by_visible_text(keyword1)
      
      print ('  Select second device')
      editWidgetModal.getConfigurationPanel().getItemButton().click()
      selects = editWidgetModal.getConfigurationPanel().getItemListByNameAndPosition(chemin, 2)
      
      Select(selects[0]).select_by_visible_text(device2)
      Select(selects[1]).select_by_visible_text(keyword2)    
      
      print ('  Confirm')
      editWidgetModal.ok()
      
   def test_createChartWidgetWithTwoDevicesWithDuplicateButton(self):
      print ('=== Nominal test of chart widget creation - two devices - duplicate button ===')
      self.doTest(
         "chart",
         "My chart widget",
         lambda editWidgetModal, widgetTitle: self.configureChartWidgetTwoDevicesWithDuplicateButton(editWidgetModal, widgetTitle, 'fakeSensor2', 'temp1', 'fakeSensor1', 'temp2'),
         lambda widgetType, widgetTitle: self.checkCreateWidget(widgetType, widgetTitle, [0, 0], [2, 2]))
      
   def configureChartWidgetTwoDevicesWithDuplicateButton(self, editWidgetModal, widgetTitle, device1, keyword1, device2, keyword2):
      print ('  Change widget title')
      editWidgetModal.setWidgetTitle(widgetTitle)
      chemin = 'widgets.chart:configurationSchema.devices.item.content.source.name'
      
      print ('  Select first device')
      editWidgetModal.getConfigurationPanel().getItemButton().click()
      selects = editWidgetModal.getConfigurationPanel().getItemListByNameAndPosition(chemin, 1)
      Select(selects[0]).select_by_visible_text(device1)
      Select(selects[1]).select_by_visible_text(keyword1)
      
      print ('  Select second device')
      editWidgetModal.getConfigurationPanel().getDuplicateButton().click()
      
      selects = editWidgetModal.getConfigurationPanel().getItemListByNameAndPosition(chemin, 2)
      Select(selects[0]).select_by_visible_text(device2)
      Select(selects[1]).select_by_visible_text(keyword2)      
      
      print ('  Confirm')
      editWidgetModal.ok()      
      
   def doTest(self, widgetType, widgetTitle, configureWidgetFct, checkFct):
      print ('Enter customizing mode')
      mainPage.enterCustomizingMode(self.browser)

      print ('Open add widget modal')
      mainPage.getAddWidgetButton(self.browser).click()
      newWidgetModal = mainPage.waitNewWidgetModal(self.browser)
      
      print ('Select widget')
      newWidgetModal.selectWidget(widgetType).click()
      newWidgetModal.clickAdd()

      print ('Configure widget')
      configureWidgetFct(mainPage.waitConfigureWidgetModal(self.browser), widgetTitle)
      
      print ('Exit customizing mode')
      mainPage.exitCustomizingMode(self.browser)
      
      print ('Check created widget')
      checkFct(widgetType, widgetTitle)
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
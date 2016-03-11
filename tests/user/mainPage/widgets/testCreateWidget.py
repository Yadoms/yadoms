import unittest
from selenium import webdriver
from selenium.webdriver.support.ui import Select
import database
import scripts
import yadomsServer
import mainPage.widgets
import i18n
import tools
import os.path

class CreateWidget(unittest.TestCase):
   """Create widget test"""
   
   def setUp(self):
      database.deploy('OneFakePlugin')
      scripts.deleteAll()
      self.serverProcess = yadomsServer.start()
      self.browser = webdriver.Firefox()
      self.browser.implicitly_wait(10)
      yadomsServer.openClient(self.browser)
            
            
   def test_createWidget(self):
      """Nominal test of widget creation"""
      self.doTest(
         "switch",
         "My switch widget",
         lambda editWidgetModal, widgetTitle: self.configureSwitchWidget(editWidgetModal, widgetTitle, 'fakeOnOffReadWriteSwitch', 'Switch'),
         lambda widgetType, widgetTitle: self.checkCreateWidget(widgetType, widgetTitle, [0, 0], [1, 1]))
      
         
   def configureSwitchWidget(self, editWidgetModal, widgetTitle, device, keyword):
      editWidgetModal.setWidgetTitle(widgetTitle)
      editWidgetModal.getConfigurationPanel().getAddObjectButton('widgets/switch:configurationSchema.devices.name').click()
      selects = editWidgetModal.getConfigurationPanel().getItemsByName('widgets/switch:configurationSchema.devices.item.content.source.name')
      Select(selects[0]).select_by_visible_text(device)
      Select(selects[1]).select_by_visible_text(keyword)
      editWidgetModal.ok()


   def checkCreateWidget(self, widgetType, widgetTitle, widgetExpectedPosition, widgetExpectedSize):
      widget = mainPage.widgets.findWidgetOnCurrentPage(self.browser, widgetType, widgetTitle)
      self.assertIsNotNone(widget)
      self.assertEqual(widget.getPosition(), widgetExpectedPosition)
      self.assertEqual(widget.getSize(), widgetExpectedSize)

         
      
   def doTest(self, widgetType, widgetTitle, configureWidgetFct, checkFct):
      # Enter customizing mode
      mainPage.widgets.enterCustomizingMode(self.browser)

      # Open add widget modal
      mainPage.widgets.getAddWidgetButton(self.browser).click()
      newWidgetModal = mainPage.widgets.waitNewWidgetModal(self.browser)
      
      # Select widget
      newWidgetModal.selectWidget(widgetType).click()
      newWidgetModal.getConfirmButton().click()

      # - Widget configuration
      configureWidgetFct(mainPage.widgets.waitConfigureWidgetModal(self.browser), widgetTitle)
      
      # Exit customizing mode
      mainPage.widgets.exitCustomizingMode(self.browser)
      
      
      # Check created widget
      checkFct(widgetType, widgetTitle)
      
      
   def tearDown(self):
      self.browser.close()
      yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
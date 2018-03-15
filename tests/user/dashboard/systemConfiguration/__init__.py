from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.keys import Keys
from configurationPanel import ConfigurationPanel
import modals
import i18n
import tools
import dashboard
import notification
from configurationPanel import ConfigurationPanel


""" Operations on system configuration dashboard page """

def getPanel(browser):
   return SystemConfigurationPanel(WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "dashboard-system-configuration"))))

def enableAdvancedParametersSection(browser): # TODO virer
   section = getPanel(browser).enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name", True)
   return ConfigurationPanel(section)

def disableAdvancedParametersSection(browser): # TODO virer
   section = getPanel(browser).enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name", False)

def isAdvancedParametersSectionActive(browser): # TODO virer
   return getPanel(browser).isOptionalSectionEnabled("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name")

def enableBasicAuthenticationSection(browser): # TODO virer
   section = getPanel(browser).enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthentication.name", True)
   return ConfigurationPanel(section)

def isBasicAuthenticationSectionActive(browser): # TODO virer
   return getPanel(browser).isOptionalSectionEnabled("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthentication.name")

def getRefreshPageOptionState(advancedParameterSection): # TODO virer
   refreshPageCheckbox = advancedParameterSection.getCheckboxItemByName("modals.dashboard.sub-windows.system-configuration.configuration-items.refreshPage.name")
   return True if refreshPageCheckbox.get_attribute('checked') is not None else False

def setRefreshPageOption(advancedParameterSection, enable): # TODO virer
   refreshPageCheckbox = advancedParameterSection.getCheckboxItemByName("modals.dashboard.sub-windows.system-configuration.configuration-items.refreshPage.name")
   isChecked = True if refreshPageCheckbox.get_attribute('checked') is not None else False
   if isChecked != enable:
      refreshPageCheckbox.click()

def applySystemConfiguration(browser): # TODO virer
   panel = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "dashboard-system-configuration")))
   panel.find_element_by_xpath(".//button[@id='btn-confirm-configure-system']").click()
   notification.waitText(browser, notification.Type.Success, i18n.get()["modals"]["dashboard"]["sub-windows"]["system-configuration"]["configurationSaved"])
   assert tools.waitUntil(lambda: notification.noNotification(browser))

def resetToDefaultSystemConfiguration(browser): # TODO virer
   panel = WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "dashboard-system-configuration")))
   panel.find_element_by_xpath(".//button[@id='resetConfiguration']").click()
   confirmationModal = modals.waitOkCancelModal(browser)
   confirmationModal.ok()


class SystemConfigurationPanel(ConfigurationPanel):
   def __init__(self, panelWebElement):
      super(SystemConfigurationPanel, self).__init__(panelWebElement)
      self._applyButton = super(SystemConfigurationPanel, self).getButton("common.apply")

   def enableAdvancedParametersSection(self):
      section = super(SystemConfigurationPanel, self).enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name", True)
      return ConfigurationPanel(section)

   def disableAdvancedParametersSection(self):
      section = super(SystemConfigurationPanel, self).enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name", False)

   def isAdvancedParametersSectionActive(self):
      return super(SystemConfigurationPanel, self).isOptionalSectionEnabled("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name")

   def enableBasicAuthenticationSection(self):
      section = super(SystemConfigurationPanel, self).enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthentication.name", True)
      return ConfigurationPanel(section)

   def isBasicAuthenticationSectionActive(self):
      return super(SystemConfigurationPanel, self).isOptionalSectionEnabled("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthentication.name")

   def getBasicAuthenticationPasswordField(self):
      return super(SystemConfigurationPanel, self).findField("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthenticationPassword.name")

   def getBasicAuthenticationPassword(self):
      return super(SystemConfigurationPanel, self).getPasswordItemByName("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthenticationPassword.name")

   def setBasicAuthenticationPassword(self, newValue):
      tools.waitReadyForInput(self.getBasicAuthenticationPassword())
      field = self.getBasicAuthenticationPassword()
      field.send_keys(Keys.CONTROL + "a")
      field.send_keys(Keys.DELETE)
      field.send_keys(newValue)

   def getBasicAuthenticationVerifyPasswordField(self):
      return super(SystemConfigurationPanel, self).findField("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthenticationPassword2.name")

   def getBasicAuthenticationVerifyPassword(self):
      return super(SystemConfigurationPanel, self).getPasswordItemByName("modals.dashboard.sub-windows.system-configuration.configuration-items.basicAuthenticationPassword2.name")

   def setBasicAuthenticationVerifyPassword(self, newValue):
      tools.waitReadyForInput(self.getBasicAuthenticationVerifyPassword())
      field = self.getBasicAuthenticationVerifyPassword()
      field.send_keys(Keys.CONTROL + "a")
      field.send_keys(Keys.DELETE)
      field.send_keys(newValue)

   def apply(self):
      self._applyButton.click()

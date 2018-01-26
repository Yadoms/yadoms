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

def enableAdvancedParametersSection(browser):
   panel = ConfigurationPanel(WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "dashboard-system-configuration"))))
   section = panel.enableOptionalSection("modals.dashboard.sub-windows.system-configuration.configuration-items.advancedParameters.name", True)
   return ConfigurationPanel(section)

def getRefreshPageOptionState(advancedParameterSection):
   refreshPageCheckbox = advancedParameterSection.getCheckboxItemByName("modals.dashboard.sub-windows.system-configuration.configuration-items.refreshPage.name")
   return True if refreshPageCheckbox.get_attribute('checked') is not None else False

def setRefreshPageOption(advancedParameterSection, enable):
   refreshPageCheckbox = advancedParameterSection.getCheckboxItemByName("modals.dashboard.sub-windows.system-configuration.configuration-items.refreshPage.name")
   isChecked = True if refreshPageCheckbox.get_attribute('checked') is not None else False
   if isChecked != enable:
      refreshPageCheckbox.click()

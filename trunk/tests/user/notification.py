from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait


   
class Type:
   Success, Information, Warning, Error = range(4)
   
   
def isLastNotification(browser, expectedType, expectedText):
   notificationContainer = browser.find_element_by_id("noty_bottomRight_layout_container")

   if notificationContainer.find_element_by_class_name("noty_type_success"):
      type = Type.Success
   elif notificationContainer.find_element_by_class_name("noty_type_information"):
      type = Type.Information
   elif notificationContainer.find_element_by_class_name("noty_type_warning"):
      type = Type.Warning
   elif notificationContainer.find_element_by_class_name("noty_type_error"):
      type = Type.Error
   else:
      assert False
      
   if (expectedType is not type):
      return False

   text = notificationContainer.find_element_by_class_name("noty_text")
   if (expectedText is not text.text):
      return False
   return True


def wait(browser, expectedType, expectedText):
   """ Wait for an expected notification """
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "noty_bottomRight_layout_container")))
   WebDriverWait(browser, 10).until(lambda driver: isLastNotification(browser, expectedType, expectedText) != True)

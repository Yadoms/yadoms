from selenium.webdriver.support import expected_conditions as Condition
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.common.exceptions import NoSuchElementException
import tools

   
class Type:
   Success, Information, Warning, Error = range(4)
   
   
class Notification:
   def __init__(self, type, text):
      self.type = type
      self.text = text
   
   def getType(self):
      return self.type
   
   def getText(self):
      return self.text
      
   def __eq__(self, other):
      """Operator == overload"""
      return self.type == other.type and self.text == other.text
      
   def Contains(self, other):
      """Similar to == operator but expected text must only be contained in notification text"""
      return self.type == other.type and other.text in self.text
      
      
def readType(notificationElement):
   divElement = notificationElement.find_element_by_tag_name("div")
   if "noty_type_success" in divElement.get_attribute("class"):
      return Type.Success
   elif "noty_type_information" in divElement.get_attribute("class"):
      return Type.Information
   elif "noty_type_warning" in divElement.get_attribute("class"):
      return Type.Warning
   elif "noty_type_error" in divElement.get_attribute("class"):
      return Type.Error
   else:
      assert False
      
      
def readText(notificationElement):
   textContainerElement = notificationElement.find_element_by_tag_name("div").find_element_by_tag_name("div").find_element_by_tag_name("span")
   return textContainerElement.text
      
   
def getCurrentNotifications(browser):
   """Return a list containing currently displayed notifications"""
   notifications = []
   
   try:
      notificationContainer = browser.find_element_by_id("noty_bottomRight_layout_container")
   except NoSuchElementException:
      return notifications
      
   if not notificationContainer.is_displayed():
      return notifications
      
   for notificationElement in notificationContainer.find_elements_by_tag_name("li"):
      notifications.append(Notification(readType(notificationElement), readText(notificationElement)))
   return notifications
   
   
def isNotification(browser, expectedType, expectedText):
   for notification in getCurrentNotifications(browser):
      if Notification(expectedType, expectedText) == notification:
         return True
   return False
   
   
def isNotificationContainingText(browser, expectedType, expectedSubText):
   for notification in getCurrentNotifications(browser):
      if notification.Contains(Notification(expectedType, expectedSubText)):
         return True
   return False


def wait(browser, expectedType, expectedText):
   """ Wait for an expected notification """
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "noty_bottomRight_layout_container")))
   WebDriverWait(browser, 10).until(lambda driver: isNotification(browser, expectedType, expectedText))


def waitIn(browser, expectedType, expectedSubText):
   """ Wait for a notification contining expected text"""
   WebDriverWait(browser, 10).until(Condition.visibility_of_element_located((By.ID, "noty_bottomRight_layout_container")))
   WebDriverWait(browser, 10).until(lambda driver: isNotificationContainingText(browser, expectedType, expectedSubText))
   
   
def noNotification(browser, timeout = 5):
   """ Check that no notification is pushed for the timeout"""
   return tools.waitUntil(lambda: len(getCurrentNotifications(browser)) != 0, timeout) == False

from selenium.common.exceptions import NoSuchElementException, StaleElementReferenceException
import tools
import time

   
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
      
      
def __readType(notificationElement):
   notificationElementClasses = notificationElement.find_element_by_tag_name("div").get_attribute("class")
   if "noty_type_success" in notificationElementClasses:
      return Type.Success
   elif "noty_type_information" in notificationElementClasses:
      return Type.Information
   elif "noty_type_warning" in notificationElementClasses:
      return Type.Warning
   elif "noty_type_error" in notificationElementClasses:
      return Type.Error
   else:
      assert False
      
      
def __readText(notificationElement):
   textContainerElement = notificationElement.find_element_by_tag_name("div").find_element_by_tag_name("div").find_element_by_tag_name("span")
   return textContainerElement.text
      
   
def getCurrentNotifications(browser):
   """Return a list containing currently displayed notifications"""
   
   try:
      notificationContainer = browser.find_element_by_id("noty_bottomRight_layout_container")
   except NoSuchElementException:
      return []
      
   if not notificationContainer.is_displayed():
      return []
      
   notifications = []
   for notificationElement in notificationContainer.find_elements_by_tag_name("li"):
      try:
         notifications.append(Notification(__readType(notificationElement), __readText(notificationElement)))
      except StaleElementReferenceException:
         # Ignore gone notifications
         pass
   return notifications
   
   
def isNotification(browser, expectedType):
   for notification in getCurrentNotifications(browser):
      try:
         if expectedType == notification.getType():
            return True
      except StaleElementReferenceException:
         # Ignore gone notifications
         pass
   return False
   
   
def isNotificationWithText(browser, expectedType, expectedText):
   for notification in getCurrentNotifications(browser):
      try:
         if Notification(expectedType, expectedText) == notification:
            return True
      except StaleElementReferenceException:
         # Ignore gone notifications
         pass
   return False

   
def isNotificationContainingText(browser, expectedType, expectedSubText):
   for notification in getCurrentNotifications(browser):
      try:
         if notification.Contains(Notification(expectedType, expectedSubText)):
            return True
      except StaleElementReferenceException:
         # Ignore gone notifications
         pass
   return False


def wait(browser, expectedType):
   """ Wait for an expected notification """
   assert tools.waitUntil(lambda: isNotification(browser, expectedType))


def waitText(browser, expectedType, expectedText):
   """ Wait for an expected notification """
   assert tools.waitUntil(lambda: isNotificationWithText(browser, expectedType, expectedText))


def waitSubText(browser, expectedType, expectedSubText):
   """ Wait for a notification containing expected text"""
   assert tools.waitUntil(lambda: isNotificationContainingText(browser, expectedType, expectedSubText))
   
   
def noNotification(browser):
   """ Check that no notification is pushed for the timeout (set by implicitly_wait) """
   return len(getCurrentNotifications(browser)) == 0

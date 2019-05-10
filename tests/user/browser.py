from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait

headless = False
chromeBin = ''

# Method which provide the browser
# The aim is to centralize the browser configuration here (headless,...)
def getBrowser():
      options = webdriver.ChromeOptions()
      if chromeBin:
         options.setBinary(chromeBin)
      if headless:
         options.add_argument('headless')
         options.add_argument('window-size=1200x600')
      return webdriver.Chrome(chrome_options=options)

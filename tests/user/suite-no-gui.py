#!/usr/bin/env python
import unittest
import HTMLTestRunner
import StringIO
from pyvirtualdisplay import Display 
 
class Test_Suite(unittest.TestCase):

   def test_main(self):
      display = Display(visible=0, size=(1024, 768))
      display.start()

      loader = unittest.TestLoader()
         
      suite = loader.discover('.')

      with open("report/index.html", 'w') as outFile:
         runner = HTMLTestRunner.HTMLTestRunner(
            stream=outFile,
            title='Yadoms user tests report',
            description='Tests covering Yadoms usage from the web client'
         )
         runner.run(suite)
         
      display.stop()
      

if __name__ == "__main__":
   unittest.main()
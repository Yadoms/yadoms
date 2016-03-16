#!/usr/bin/env python
import unittest
import StringIO
import xmlrunner
from pyvirtualdisplay import Display 
 
class Test_Suite(unittest.TestCase):

   def test_main(self):
      display = Display(visible=0, size=(1024, 768))
      display.start()

      loader = unittest.TestLoader()
      suite = loader.discover('.')

      with open('report/output.xml', 'wb') as outFile:
         runner = xmlrunner.XMLTestRunner(output=outFile)
         runner.run(suite)
         
      display.stop()
      

if __name__ == "__main__":
   unittest.main()
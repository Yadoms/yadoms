#!/usr/bin/env python
import sys
sys.path.append('.')
import unittest
import StringIO
import xmlrunner
import platform
from pyvirtualdisplay import Display
import tools

 
class Test_Suite(unittest.TestCase):

   def test_main(self):
      if (platform.system() == "Windows"):
         raise Exception('No-GUI tests are not available under Windows');

      tools.deleteContext()
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
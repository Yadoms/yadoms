#!/usr/bin/env python
import sys
sys.path.append('.')
import unittest
import xmlrunner
import platform
from pyvirtualdisplay import Display
import tools
import shutil
import os


if __name__ == "__main__":
   tools.ChromeOptionsHelper.setHeadless()
   tools.deleteContext()
   suite = unittest.TestLoader().discover('.')

   shutil.rmtree('report_xml', True)
   os.makedirs('report_xml', exist_ok=True)
   with open('report_xml/report.xml', 'wb') as outFile:
      runner = xmlrunner.XMLTestRunner(output=outFile)
      runner.run(suite)

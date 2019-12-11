import sys
sys.path.append('.')
import unittest
import HtmlTestRunner
import tools
import shutil
 

if __name__ == "__main__":
   tools.deleteContext()         
   loader = unittest.TestLoader()         
   suite = loader.discover('.')

   shutil.rmtree('report')
   runner = HtmlTestRunner.HTMLTestRunner(output = 'report',
                                          combine_reports = True,
                                          report_name = 'index',
                                          add_timestamp = False,
                                          report_title ='Yadoms user tests report')
   runner.run(suite)

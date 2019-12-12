import sys
sys.path.append('.')
import unittest
import HtmlTestRunner
import tools
import shutil
import os


if __name__ == "__main__":
   tools.ChromeOptionsHelper.setHeadless()
   tools.deleteContext()
   suite = unittest.TestLoader().discover('.')

   shutil.rmtree('report_html', True)
   os.makedirs('report_html', exist_ok=True)
   runner = HtmlTestRunner.HTMLTestRunner(output = 'report_html',
                                          combine_reports = True,
                                          report_name = 'index',
                                          add_timestamp = False,
                                          report_title ='Yadoms user tests report')
   runner.run(suite)

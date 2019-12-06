import sys
sys.path.append('.')
import unittest
import HtmlTestRunner

try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO

import tools
 
 #TODO gros ménage à faire
# class Test_Suite(unittest.TestCase):

#    def test_main(self):
#       tools.deleteContext()      
#       loader = unittest.TestLoader()         
#       suite = loader.discover('.')

      # with open("report/index.html", 'w') as outFile:
      #    runner = HtmlTestRunner.HTMLTestRunner(
      #       stream=outFile,
      #       title='Yadoms user tests report',
      #       description='Tests covering Yadoms usage from the web client'
      #    )
      #    runner.run(suite)


if __name__ == "__main__":
   tools.deleteContext()         
   loader = unittest.TestLoader()         
   suite = loader.discover('.')
   # runner = unittest.TextTestRunner()
   # runner.run(suite)
   # unittest.main(testRunner=HtmlTestRunner.HTMLTestRunner(output='report'), testLoader=loader)
   runner = HtmlTestRunner.HTMLTestRunner(output='report')
   runner.run(suite)
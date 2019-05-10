import sys
sys.path.append('.')
import unittest
import HTMLTestRunner
import StringIO
import tools
import sys, getopt 
import browser
import argparse

class Test_Suite(unittest.TestCase):

   def test_main(self):
      tools.deleteContext()      
      loader = unittest.TestLoader()         
      suite = loader.discover('.')

      with open("report/index.html", 'w') as outFile:
         runner = HTMLTestRunner.HTMLTestRunner(
            stream=outFile,
            title='Yadoms user tests report',
            description='Tests covering Yadoms usage from the web client'
         )
         runner.run(suite)


def main(argv):
   browser.headless = False
   
   parser = argparse.ArgumentParser()
   parser.add_argument('--headless', action='store_true')
   parser.add_argument('--chrome', action='store')
   parser.add_argument('unittest_args', nargs='*')
   
   args = parser.parse_args()
   # TODO: Go do something with args.input and args.filename
   browser.headless = args.headless
   # Now set the sys.argv to the unittest_args (leaving sys.argv[0] alone)
   sys.argv[1:] = args.unittest_args
   print 'Running headless mode : ' + str(args.headless)
   print 'Running chrome : ' + str(args.chrome)
   unittest.main()

if __name__ == "__main__":
   main(sys.argv[1:])   
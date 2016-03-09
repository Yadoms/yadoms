import unittest
import HTMLTestRunner
import StringIO

import dashboard.testMenuEntries   # TODO virer
 
 
class Test_Suite(unittest.TestCase):

   def test_main(self):
      loader = unittest.TestLoader()
         
      suite = loader.discover('.')
      # suite = unittest.TestSuite()
      # suite.addTests([
         # unittest.defaultTestLoader.loadTestsFromTestCase(dashboard.testMenuEntries.MenuEntries)
         # ])

      with open("report.html", 'w') as outFile:
         runner = HTMLTestRunner.HTMLTestRunner(
            stream=outFile,
            title='Yadoms user tests',
            description='Tests covering Yadoms usage from the web client'
         )
         runner.run(suite)


if __name__ == "__main__":
   unittest.main()
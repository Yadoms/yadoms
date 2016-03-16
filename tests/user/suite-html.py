import unittest
import HTMLTestRunner
import StringIO
 
 
class Test_Suite(unittest.TestCase):

   def test_main(self):
      loader = unittest.TestLoader()
         
      suite = loader.discover('.')

      with open("report/index.html", 'w') as outFile:
         runner = HTMLTestRunner.HTMLTestRunner(
            stream=outFile,
            title='Yadoms user tests report',
            description='Tests covering Yadoms usage from the web client'
         )
         runner.run(suite)


if __name__ == "__main__":
   unittest.main()
This file explains how to execute user tests


==========================
Dependencies and tools
==========================
These user tests are based on the Selenium tool (http://www.seleniumhq.org/), used with the Python implementation.
- Python 2.7 must be installed.

Install Python modules :

1) Easy method
In a console, type :
python -m pip install selenium psutil
   
2) Alternative method
If first method doesn't work (behind proxy for example), you can download modules (from the https://pypi.python.org/pypi site) as .whl files, and install manually.
For each module, type in a console :
python -m pip install {Path_to_downloaded_file}\{Module_to_install}.whl



==========================
Run a test
==========================
First you need to be into the root of user tests :
   cd {Yadoms_trunk_path}\tests\user
   
   
To run all tests :
   python -m unittest discover
   
To run tests of a specific module :
   python -m dashboard.automation.testCreateRule
   

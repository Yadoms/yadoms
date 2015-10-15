This file explains how to execute user tests


==========================
Dependancies and tools
==========================
These user tests are based on the Selenium tool (http://www.seleniumhq.org/), used with the Python implementation.
- First download the whl file at https://pypi.python.org/pypi/selenium
- Install it :
   - Got to {Your_Python_Install_Path}/Scripts
   - Type :
      python -m pip install {Path_to_downloaded_file}\selenium-x.yy.z-py2-none-any.whl


==========================
Run a test
==========================
- Start the server (yadoms executable)
- Go the test path
- Run it :
   python {my_test}.py
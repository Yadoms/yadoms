This file explains how to execute user tests


==========================
Dependancies and tools
==========================
These user tests are based on the Selenium tool (http://www.seleniumhq.org/), used with the Python implementation.
- Python 2.7 must be installed.

1) Download Python modules :
Be careful to select version of following modules corresponding to your Python installation (Python version, 32/64 bits, OS...)
- Selenium : Download the whl file from https://pypi.python.org/pypi/selenium
- psutil : Download the whl file from https://pypi.python.org/pypi/psutil

2) Install Python modules
- In a console, and for each module, type :
python -m pip install {Path_to_downloaded_file}\{Module_to_install}.whl



==========================
Run a test
==========================
- Start the server (yadoms executable)
- Always start test from the {Yadoms_trunk_path}\tests\user
- Run it :
   python -m dashboard.automation.testCreateRule
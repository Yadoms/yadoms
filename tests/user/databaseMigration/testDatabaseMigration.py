import sys
sys.path.append('.')
import unittest
import glob
import re
import os
import database
import config
import scripts
import yadomsServer
import dashboard
from selenium import webdriver
import tools

class DatabaseMigration(unittest.TestCase):
   """Test database migration"""
   """Just inject a database of wanted version, and check if web client can connect to server (if database fails to migrate, web server doesn't start)"""
   
   def setUp(self):
      yadomsServer.ensureStopped()
      config.deploy("nominal")
      scripts.deleteAll()
      self.browser = webdriver.Chrome(options=tools.ChromeOptionsHelper.get())
      self.browser.implicitly_wait(10)
      self.serverProcess = None


   def checkMigrationOk(self):
      self.serverProcess = yadomsServer.start(["logLevel=information"])
      yadomsServer.openClient(self.browser)


   def test_noExistingDatabase(self):
      database.new()
      self.checkMigrationOk()


   def test_migration(self):
      print ('=== Test migration ===')
      dbFiles = glob.glob(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'database_*.db3'))
      for dbFile in dbFiles:
         dbVersion = re.search('.*database_(.*)\.db3', dbFile).group(1).replace('_', '.')
         print ('Test migration from database version {dbVersion}...')
         if self.serverProcess:
            yadomsServer.stop(self.serverProcess)
         database.deployFromPath(dbFile)
         self.checkMigrationOk()


      
   def tearDown(self):
      self.browser.close()
      if self.serverProcess:
         yadomsServer.stop(self.serverProcess)

if __name__ == "__main__":
   unittest.main()
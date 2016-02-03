import time

def yMain(yApi):
   while(True):
      print 'location = ', yApi.getInfo(yApi.kLatitude)
      time.sleep(5)

import time

def yMain(yApi):
   while True:
      print ('Yadoms server OS = {yApi.getInfo(yApi.kYadomsServerOS)}')
      time.sleep(5)

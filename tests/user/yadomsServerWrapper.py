# This wrapper is used to start Yadoms in the same console window
# (not the console of the test)
# Using this, the test can communicate to the wrapper, asking it
# to stop gracefuly Yadoms
#

import subprocess, time, signal, os, sys

cmdLine = ' '.join(sys.argv[1:])
print 'Start process : "' + cmdLine + '"...'

print sys.argv[1], "started"
process = subprocess.Popen(cmdLine)

sys.stdin.readline()
print "Stopping Yadoms..."

try:
   os.kill(signal.CTRL_C_EVENT, 0)
   process.wait()
except KeyboardInterrupt:
   pass

print "Yadoms stopped"

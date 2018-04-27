#pragma once

#ifdef __cplusplus
extern "C" {
#endif
   
   int mosquitto_start(int argc, char *argv[]);

   void mosquitto_stop();
#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
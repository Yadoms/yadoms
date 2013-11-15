#pragma once


class CPluginInstance
{
public:
   CPluginInstance(IHardwarePlugin& plugin) : m_Plugin(plugin)
   {
   }

   void Start()
   {
      m_Plugin->Start();
      m_pThread = new boost::thread(DoWork);
   }

   void Stop()
   {
      // Stop the thread
      m_pThread->interrupt();
      m_pThread->join();
      delete m_pThread;
      m_pThread = NULL;

      // Call plugin stop method to free ressources
      m_Plugin->Stop();
   }

   void DoWork()
   {
      // TODO : we can set protections here (restart plugin if it crashes, force to stop it...)
      try 
      {
         // Call plugin main loop
         m_Plugin->DoWork();
      } 
      catch(boost::thread_interrupted&)
      {
         // Thread is stopped
      }
   }

protected:
   IHardwarePlugin& m_pPlugin;
   boost::thread* m_pThread;
};

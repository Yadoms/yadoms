#pragma once


//--------------------------------------------------------------
/// \brief	Base class for threads
//--------------------------------------------------------------
class CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief		Constructor
   /// \param [in]	threadName : the thread name
   /// \param [in]	bRethrowDoWorkExceptions : if true, all exception catch by thsi class are rethrown, if false nothing else is done with exception
   /// \return    	void
   //--------------------------------------------------------------
   CThreadBase(const std::string & threadName = "", const bool bRethrowDoWorkExceptions = false);

   //--------------------------------------------------------------
   /// \brief			Destructor
   /// \return    	void
   //--------------------------------------------------------------
   virtual ~CThreadBase(void);

   //--------------------------------------------------------------
   /// \brief			Enumeration for all thread status
   //--------------------------------------------------------------
   enum EStatus
   {
      kStarting,        /// \brief  Thread is starting
      kRunning,         /// \brief	Thread is running
      kStopping,        /// \brief	Thread is requested to stop
      kStopped          /// \brief	Thread is stopped
   };

public:
   //--------------------------------------------------------------
   /// \brief			Start the thread
   /// \return    	void
   //--------------------------------------------------------------
   virtual void start();
   //--------------------------------------------------------------
   /// \brief			Ask the thread to stop and returns (non blocking)
   /// \return    	void
   //--------------------------------------------------------------
   virtual void requestToStop();

   //--------------------------------------------------------------
   /// \brief			Stop the thread (blocking)
   /// \return    	true if thread was successfully stopped, false if timeout
   //--------------------------------------------------------------
   virtual bool stop();

   //--------------------------------------------------------------
   /// \brief			Wait for end of the the thread (blocking)
   /// \par seconds  Timeout. If 0, wait indefinitely
   /// \return    	true if thread was stopped, false if timeout
   //--------------------------------------------------------------
   virtual bool waitForStop(int seconds = 0);

   //--------------------------------------------------------------
   /// \brief			Get the thread status
   /// \return    	One of the EStatus enum values
   //--------------------------------------------------------------
   EStatus getStatus() const;

   //--------------------------------------------------------------
   /// \brief		Set the thread name
   /// \param [in]	threadName : the thread name
   //--------------------------------------------------------------
   void setName(const std::string & name)
   { m_threadName = name; }

   //--------------------------------------------------------------
   /// \brief		Get the thread name
   /// \return    	The thread name
   //--------------------------------------------------------------
   const std::string& getName() const
   { return m_threadName; }


   //--------------------------------------------------------------
   /// \brief     Change the stop time out
   /// \return    The stop timeout (in seconds)
   //--------------------------------------------------------------
   void setStopTimeoutSeconds(int seconds)
   { m_stopTimeoutSeconds = seconds; }

   //--------------------------------------------------------------
   /// \brief		Get the stop time out
   /// \return    	The stop timeout (in seconds)
   //--------------------------------------------------------------
   int getStopTimeoutSeconds() const
   { return m_stopTimeoutSeconds;}   

protected:
   //--------------------------------------------------------------
   /// \brief			The main thread work
   //--------------------------------------------------------------
   virtual void doWork() = 0;

   //--------------------------------------------------------------
   /// \brief			Change the thread status
   /// \param [in]	newStatus : the new thread status
   /// \return    	void
   //--------------------------------------------------------------
   void changeStatus(const EStatus & newStatus);

private:  
   //--------------------------------------------------------------
   /// \brief			Method which manage the virtual doWork method
   /// \return    	void
   //--------------------------------------------------------------
   void doWorkInternal();

private:
   //--------------------------------------------------------------
   /// \brief			Thread name
   //--------------------------------------------------------------
   std::string m_threadName;

   //--------------------------------------------------------------
   /// \brief			Pointer (shared_ptr) to the thread
   //--------------------------------------------------------------
   boost::shared_ptr<boost::thread> m_thread;

   //--------------------------------------------------------------
   /// \brief			Current thread status
   //--------------------------------------------------------------
   EStatus m_threadStatus;

   //--------------------------------------------------------------
   /// \brief			Manage the status change callback
   //--------------------------------------------------------------
   boost::signals2::signal<void(const EStatus & newStatus)> m_statusChanged;

   //--------------------------------------------------------------
   /// \brief			Default timeout when stop requested before thread effectively stop.
   ///               If thread is not stopped at timeout, thread is killed.
   //--------------------------------------------------------------
   int m_stopTimeoutSeconds;

   //--------------------------------------------------------------
   /// \brief			Default timeout when stop requested before thread effectively stop.
   ///               If thread is not stopped at timeout, thread is killed.
   //--------------------------------------------------------------
   static const int DefaultStopTimeoutSeconds;

   //--------------------------------------------------------------
   /// \brief			Indicate if exception of DoWork are rethrown or not
   //--------------------------------------------------------------
   bool m_bRethrowDoWorkExceptions;
};


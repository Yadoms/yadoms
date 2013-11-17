#pragma once

#include <string>
#include "boost/thread.hpp"
#include <boost/signals2.hpp>

//--------------------------------------------------------------
/// \brief	Base class for threads
//--------------------------------------------------------------
class CThreadBase
{
public:
	//--------------------------------------------------------------
	/// \brief		Constructor
	/// \return    	void
	//--------------------------------------------------------------
	CThreadBase();

	//--------------------------------------------------------------
	/// \brief		Constructor
	/// \param [in]	threadName : the thread name
	/// \return    	void
	//--------------------------------------------------------------
	CThreadBase(const std::string & threadName);
	
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
		kRunning,         /// \brief	Thread is running
		kStopRequested,   /// \brief	Thread is requested to stop
		kStopped          /// \brief	Thread is stopped
	};

public:
	//--------------------------------------------------------------
	/// \brief			Start the thread
	/// \return    	void
	//--------------------------------------------------------------
	void start();
	
	//--------------------------------------------------------------
	/// \brief			Ask the thread to stop and returns (non blocking)
	/// \return    	void
	//--------------------------------------------------------------
	void requestToStop();

	//--------------------------------------------------------------
	/// \brief			Stop the thread (blocking)
	/// \return    	void
	//--------------------------------------------------------------
	void stop();

	//--------------------------------------------------------------
	/// \brief			Get the thread status
	/// \return    	One of the EStatus enum values
	//--------------------------------------------------------------
	const EStatus getStatus();
	
	//--------------------------------------------------------------
	/// \brief		Set the thread name
	/// \param [in]	threadName : the thread name
	//--------------------------------------------------------------
	void setName(const std::string & name);

	//--------------------------------------------------------------
	/// \brief		Get the thread name
	/// \return    	The thread name
	//--------------------------------------------------------------
	const std::string getName();

	//--------------------------------------------------------------
	/// \brief			Register a callback function for status change event
	/// \param [in]	pFunc : function pointer for callback
	/// \param [in]	pInstance : instance which own function as member
	/// \return    	void
	//--------------------------------------------------------------
	void registerStatusChanged(void *pFunc(const EStatus & newStatus), void * pInstance);

protected:
	//--------------------------------------------------------------
	/// \brief			The main plugin work
	/// \return    	void
	//--------------------------------------------------------------
	virtual void doWork() =0;

private:
	//--------------------------------------------------------------
	/// \brief			Change the thread status
	/// \param [in]	newStatus : the new thread status
	/// \return    	void
	//--------------------------------------------------------------
	void changeStatus(const EStatus & newStatus);

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
};


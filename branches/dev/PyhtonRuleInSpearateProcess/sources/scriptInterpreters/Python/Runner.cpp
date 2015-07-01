#include "stdafx.h"
#include "Runner.h"
#include "ScriptFile.h"
#include <shared/Log.h>
#include "PythonException.hpp"
#include <shared/DataContainer.h>


CRunner::CRunner(const std::string& scriptPath, const std::string& interpreterPath,
   boost::shared_ptr<IPythonExecutable> executable, const shared::CDataContainer& scriptConfiguration)
   :m_scriptFile(boost::make_shared<CScriptFile>(scriptPath)), m_scriptConfiguration(scriptConfiguration), m_interpreterPath(interpreterPath), m_executable(executable)
{
}

CRunner::~CRunner()
{
   interrupt();
}

void CRunner::run(shared::script::yScriptApi::IYScriptApi& context, boost::shared_ptr<shared::script::ILogger> scriptLogger)
{
   m_lastError.clear();

   try
   {
      scriptLogger->out() << "#### START ####" << std::endl;

      // TODO locker m_process pendant l'appel au start
      m_process = m_executable->startModule(m_scriptFile->module(), m_scriptFile->abslouteParentPath(), scriptLogger);

      int returnCode = m_process->wait();
      if (returnCode != 0)
         throw CPythonException("Script returned with error " + boost::lexical_cast<std::string>(returnCode));

      scriptLogger->out() << "#### END ####" << std::endl;


      //TODO
      //// Create sub-interpreter (needed for thread context)
      //CSubInterpreter interpreter;

      //context.log("#### START ####");

      //// Load script file
      //CScriptLoader loader(m_scriptPath, m_interpreterPath);
      //loader.load();

      //// Create the context (instance of yScriptApi)
      //CPythonObject pyContext(SWIG_NewPointerObj(&context, SWIG_TypeQuery("shared::script::yScriptApi::IYScriptApi *"), 0));
      //CPythonObject tuple(PyTuple_New(1));
      //Py_XINCREF(*pyContext); // Increment pyContext reference count, because PyTuple_SetItem steals the reference
      //if (PyTuple_SetItem(*tuple, 0, *pyContext))
      //   throw CPythonException("Unable to create context");

      //// Redirect console
      //CConsoleRedirector stdoutRedirector(tuple);

      //// Run the script
      //CPythonObject pyReturnValue(PyObject_CallObject(*loader.yMain(), *tuple));

      //// If errno = 4 (interrupt function call), it's because of stopping Yadoms (Python is catching CTRL-C),
      //// so don't throw a CPythonException, or rule will be set to error state, and not started again at next Yadoms startup.
      //if (pyReturnValue.isNull())
      //{
      //   //TODO
      //   //if (!isPythonError(PyExc_IOError, 4))
      //   //   throw CPythonException("Script yMain function returned with error");

      //   // We are here because because receive a CTRL-C (Python catch CTRL-C as the main application...).
      //   // So Yadoms will end, just wait that thread stop is required
      //   context.log("#### END ####");
      //   YADOMS_LOG(information) << m_scriptPath << " : script exited";
      //   boost::this_thread::sleep_for(boost::chrono::seconds(10000)); // Should throw boost::thread_interrupted
      //   YADOMS_LOG(error) << m_scriptPath << " : timeout waiting for stop rule thread";
      //}

      //context.log("#### END ####");
      YADOMS_LOG(information) << m_scriptFile->pathName() << " : script exited";
   }
   catch(CPythonException& e)
   {
      scriptLogger->error() << m_scriptFile->pathName() << " : error running script, " << e.what();
      scriptLogger->out() << "#### END ####";
      m_lastError = e.what();
   }
}

void CRunner::interrupt()
{
   try
   {
      if (!!m_process)
         Poco::Process::requestTermination(m_process->id());
   }
   catch (Poco::SystemException&)
   {
      // Nothing to do. This exception occurs when process is already stopped
   }
}

bool CRunner::isOk() const
{
   return m_lastError.empty();
}

std::string CRunner::error() const
{
   return m_lastError;
}

//TODO
//bool CRunner::isPythonError(PyObject* pyException, int pyErrorCode) const
//{
//   if (!PyErr_Occurred())
//      return false;
//   
//   if (!PyErr_ExceptionMatches(pyException))
//      return false;
//
//   PyObject* pyErrorType = NULL;
//   PyObject* pyErrorMessage = NULL;
//   PyObject* pyTraceBack = NULL;
//   PyErr_Fetch(&pyErrorType, &pyErrorMessage, &pyTraceBack);
//
//   // First item of the pyErrorMessage tuple is errno
//   CPythonBorrowedObject item(PyTuple_GetItem(pyErrorMessage, 0));
//   long errCode = PyInt_AsLong(*item);
//
//   PyErr_Restore(pyErrorType, pyErrorMessage, pyTraceBack);
//
//   return errCode == pyErrorCode;
//}
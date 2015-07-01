#pragma once
#include "IInitializer.h"
#include "PythonLibInclude.h"
//TODO classe à virer
//--------------------------------------------------------------
/// \brief	Python initializer to support RAII idiom
//--------------------------------------------------------------
class CInitializer : public IInitializer
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CInitializer();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CInitializer();

   //--------------------------------------------------------------
   /// \brief	The global Python thread state
   //--------------------------------------------------------------
   PyThreadState* m_globalPythonThreadState;
};




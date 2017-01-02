#pragma once

//--------------------------------------------------------------
/// \brief	yScriptApi context accessor, used by script to interact with Yadoms
//--------------------------------------------------------------
class IContextAccessor
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IContextAccessor() {}

   //--------------------------------------------------------------
   /// \brief	Get the context accessor ID (unique on full system)
   /// \return The context accessor ID
   //--------------------------------------------------------------
   virtual std::string id() const = 0;
};

#pragma once

//--------------------------------------------------------------
/// \brief	Python script stopped event data
//--------------------------------------------------------------
class IEventScriptStopped
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IEventScriptStopped()
   {
   }

   //--------------------------------------------------------------
   /// \brief	Get the script ID
   /// \return The script ID
   //--------------------------------------------------------------
   virtual int scriptId() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the script error (empty if no error)
   /// \return The script error
   //--------------------------------------------------------------
   virtual const std::string& error() const = 0;
};

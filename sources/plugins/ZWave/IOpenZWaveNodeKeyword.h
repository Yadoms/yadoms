#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <value_classes/ValueID.h>
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/typeInfo/ITypeInfo.h>
#include "OpenZWaveCommandClass.h"

//--------------------------------------------------------------
/// \brief	    Interface for keywords
//--------------------------------------------------------------
class IOpenZWaveNodeKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IOpenZWaveNodeKeyword()
   {
   }
   
   //--------------------------------------------------------------
   /// \brief	    Send a command 
   /// \param [in]   commandData    The command content
   /// \return	      true if command has been sent (not applied, just sent), else false
   //--------------------------------------------------------------
   virtual bool sendCommand(const std::string & commandData) = 0;

   //--------------------------------------------------------------
   /// \brief	      get the last value of a keyword (as string)
   //--------------------------------------------------------------   
   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue() = 0;

   //--------------------------------------------------------------
   /// \brief	    Update the keyword value 
   /// \param [in]   value    The new keyword value
   //--------------------------------------------------------------
   virtual void updateValue(OpenZWave::ValueID & value) = 0;

   //--------------------------------------------------------------
   /// \brief	    Update the keyword value 
   /// \param [in]   value    The new keyword value
   //--------------------------------------------------------------
   virtual shared::CDataContainerSharedPtr serialize() = 0;

   //--------------------------------------------------------------
   /// \brief	   Get the type information
   /// \return    the type information about keyword
   //--------------------------------------------------------------
   virtual boost::shared_ptr<shared::plugin::yPluginApi::typeInfo::ITypeInfo> & getTypeInformation() = 0;

   //--------------------------------------------------------------
   /// \brief	  Provide the keyword index (order index of keyword)
   /// \return    the keyword index
   //--------------------------------------------------------------
   virtual int getIndex() = 0;

   //--------------------------------------------------------------
   /// \brief	  Provide the keyword index (order index of keyword)
   /// \return    the keyword index
   //--------------------------------------------------------------
   virtual ECommandClass getCommandClass() = 0;
};
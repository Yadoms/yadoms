//
// CHardwarePluginQualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "IHardwarePluginQualifier.h"

//--------------------------------------------------------------
/// \brief	this class is used to qualify a plugin
//--------------------------------------------------------------
class CHardwarePluginQualifier : public IHardwarePluginQualifier
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CHardwarePluginQualifier();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginQualifier();

   // IHardwarePluginQualifier implementation
   virtual void signalLoad(const boost::shared_ptr<const CHardwarePluginFactory> plugin);
   virtual void signalUnload(const boost::shared_ptr<const CHardwarePluginFactory> plugin);
   virtual void signalCrash(const boost::shared_ptr<const CHardwarePluginFactory> plugin, const std::string& exceptionName);
   virtual bool isSafe(const boost::shared_ptr<const CHardwarePluginFactory> plugin);
   virtual int getQualityLevel(const boost::shared_ptr<const CHardwarePluginFactory> plugin);
   // [END] IHardwarePluginQualifier implementation
};

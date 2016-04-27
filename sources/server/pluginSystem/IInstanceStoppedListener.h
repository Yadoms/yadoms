#pragma once

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Listener when an instance is stopped
   //--------------------------------------------------------------
   class IInstanceStoppedListener
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IInstanceStoppedListener() {}

      //--------------------------------------------------------------
      /// \brief	Called when instance is stopped
      /// \param[in] instanceId  ID of the stopped instance
      //--------------------------------------------------------------
      virtual void onStopped(int instanceId) = 0;
   };
} // namespace pluginSystem


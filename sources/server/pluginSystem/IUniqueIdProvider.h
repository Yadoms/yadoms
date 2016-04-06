#pragma once

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Unique ID provider interface
   //--------------------------------------------------------------
   class IUniqueIdProvider
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IUniqueIdProvider() {}

      //--------------------------------------------------------------
      /// \brief	Get the context unique ID
      /// \return the context unique ID
      //--------------------------------------------------------------
      virtual std::string id() const = 0;
   };
} // namespace pluginSystem


#pragma once
#include "IFactory.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	IFactory implementation
   //--------------------------------------------------------------
   class CFactory : public IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CFactory();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CFactory();

      // IFactory Implementation
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> createInformation(const boost::filesystem::path& libraryPath) const;
      // [END] IFactory Implementation
   };

} // namespace pluginSystem

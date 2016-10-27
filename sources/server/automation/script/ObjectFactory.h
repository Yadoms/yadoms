//
// Object factory
//
#pragma once
#include "IObjectFactory.h"


namespace automation
{
   namespace script
   {
      //-----------------------------------------------------------------------------
      /// \class              Main object factory
      //-----------------------------------------------------------------------------
      class ObjectFactory : public IObjectFactory
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~ObjectFactory();

         // IObjectFactory implementation
         boost::shared_ptr<IAutoLocation> getAutoLocationService() override;
         boost::shared_ptr<ILocation> getLocation() override;
         boost::shared_ptr<IDayLight> getDayLight() override;
         // [END] IObjectFactory implementation

      private:
         boost::shared_ptr<IAutoLocation> m_autoLocationService;
         boost::shared_ptr<ILocation> m_location;
         boost::shared_ptr<IDayLight> m_dayLight;
      };
   }
} // namespace automation::script



//
// Object factory
//
#pragma once
#include "IObjectFactory.h"


namespace automation { namespace script
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
      virtual boost::shared_ptr<automation::script::IAutoLocation> getAutoLocationService();
      virtual boost::shared_ptr<automation::script::ILocation> getLocation();
      virtual boost::shared_ptr<automation::script::IDayLight> getDayLight();
      // [END] IObjectFactory implementation

   private:
      boost::shared_ptr<automation::script::IAutoLocation> m_autoLocationService;
      boost::shared_ptr<automation::script::ILocation> m_location;
      boost::shared_ptr<automation::script::IDayLight> m_dayLight;
   };

} } // namespace automation::script
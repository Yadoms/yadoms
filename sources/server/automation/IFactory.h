#pragma once
#include "IInstance.h"
#include <server/database/entities/Entities.h>
#include "database/IDataProvider.h"
#include "dataAccessLayer/IDataAccessLayer.h"
#include "IQualifier.h"
#include "IInstanceStoppedListener.h"

namespace automation
{
   namespace script
   {
      //--------------------------------------------------------------
      /// \brief	Interpreters factory
      //--------------------------------------------------------------
      class IFactory
      {
      public:
         //--------------------------------------------------------------
         /// \brief			Map of all valid interpreters (key are interpreter file names, values are interpreter information)
         //--------------------------------------------------------------
         typedef std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>> AvailableInterpretersMap;

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~IFactory()
         {
         }

         //--------------------------------------------------------------
         /// \brief	                     Find all installed interpreters
         /// \return                      All installed interpreters found (included not supported)
         //--------------------------------------------------------------
         virtual AvailableInterpretersMap findAvailableInterpreters() const = 0;

         //--------------------------------------------------------------
         /// \brief	                     Create a interpreter instance
         /// \param [in]   instanceData               the interpreter instance data
         /// \param [in]   dataProvider               the database accessor
         /// \param [in]   dataAccessLayer            the data access layer
         /// \param [in]   instanceStoppedListener    Listener to call when instance stopped
         /// \return                      The interpreter instance
         //--------------------------------------------------------------
         virtual boost::shared_ptr<IInstance> createInstance(boost::shared_ptr<database::IDataProvider> dataProvider,
                                                             boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                             boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener) const = 0;

         //--------------------------------------------------------------
         /// \brief	                  Get the interpreter instance data path
         /// \param [in]	instanceId  The interpreter instance id
         /// \return                   The interpreter instance data path
         //--------------------------------------------------------------
         virtual boost::filesystem::path interpreterDataPath(int instanceId) const = 0;
      };
   }
} // namespace automation::script



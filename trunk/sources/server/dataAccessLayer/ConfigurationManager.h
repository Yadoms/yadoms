#pragma once
#include "IConfigurationManager.h"
#include "database/IConfigurationRequester.h"

namespace dataAccessLayer {

   class CConfigurationManager : public IConfigurationManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                                Constructor
      /// \param [in] configurationRequester    The configuration requester
      /// \param [in] notifier                  The notifier
      //--------------------------------------------------------------
      CConfigurationManager(boost::shared_ptr< database::IConfigurationRequester > configurationRequester);
   
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CConfigurationManager();

      // IConfigurationManager implementation
      virtual void create(database::entities::CConfiguration& configurationToCreate);
      virtual bool exists(const std::string & section, const std::string & name);
      virtual boost::shared_ptr<database::entities::CConfiguration> getConfiguration(const std::string & section, const std::string & name);
      virtual std::vector<boost::shared_ptr<database::entities::CConfiguration> > getConfigurations(const std::string & section);
      virtual std::vector<boost::shared_ptr<database::entities::CConfiguration> > getConfigurations();
      virtual void updateConfiguration(database::entities::CConfiguration& configurationToUpdate);
      virtual void removeConfiguration(database::entities::CConfiguration& configurationToRemove);
      // [END] - IConfigurationManager implementation
      
   protected:
      //--------------------------------------------------------------
      /// \brief  Post an update configuration notification for the [section,name] data
      //--------------------------------------------------------------
      void postUpdateNotification(const std::string & section, const std::string & name);

   private:
      //--------------------------------------------------------------
      /// \brief           The real data requester
      //--------------------------------------------------------------
      boost::shared_ptr< database::IConfigurationRequester > m_configurationRequester;
   };
 
} //namespace dataAccessLayer 

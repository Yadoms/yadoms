#pragma once

#include "IRestService.h"
#include "update/UpdateManager.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace web { namespace rest { namespace service {

   //-----------------------------------------------------------------------------
   /// \brief  REST handler class for update and check for update
   //-----------------------------------------------------------------------------      
   class CUpdate : public IRestService
   {
   public:
      //-----------------------------------------------------------------------------
      /// \brief  Constructor
      /// \param [in] updateManager   The update manager
      //-----------------------------------------------------------------------------      
      CUpdate(boost::shared_ptr<update::CUpdateManager> updateManager);

      //-----------------------------------------------------------------------------
      /// \brief  Destructor
      //-----------------------------------------------------------------------------      
      virtual ~CUpdate();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      //-----------------------------------------------------------------------------
      /// \brief  Get the REST keyword
      /// \return The REST keyword
      //-----------------------------------------------------------------------------      
      static const std::string & getRestKeyword();
   private:
      //-----------------------------------------------------
      ///\brief The operation type to do
      //-----------------------------------------------------
      DECLARE_ENUM_HEADER(EWhatToDo, 
         ((Update)(0))
         ((Check)(1))
      )

      //-----------------------------------------------------------------------------
      /// \brief  Do an update or check for update for Yadoms
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer forYadoms(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      //-----------------------------------------------------------------------------
      /// \brief  Do an update or check for update for a plugin
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer forPlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      //-----------------------------------------------------------------------------
      /// \brief  Get the list of all available plugins from the server
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer availablePlugins(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      //-----------------------------------------------------------------------------
      /// \brief  Update a plugin to another version (upgrade or downgrade)
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer updatePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      //-----------------------------------------------------------------------------
      /// \brief  Install a new plugin
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer installPlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      //-----------------------------------------------------------------------------
      /// \brief  Remove a plugin
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer removePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

      //-----------------------------------------------------------------------------
      /// \brief  Do an update or check for update for a widget
      /// \param [in]   parameters        The url parameters
      /// \param [in]   requestContent    The url content
      /// \return the request result
      //-----------------------------------------------------------------------------         
      shared::CDataContainer forWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
   
      //-----------------------------------------------------------------------------
      /// \brief  Post an update or check for update
      /// \param [in]   source        The object source to update
      /// \param [in]   whatToDo      The object source to update
      /// \return the request result
      //-----------------------------------------------------------------------------  
      shared::CDataContainer postRequest(boost::shared_ptr<update::source::IUpdateSource> source, const std::string whatToDo);

      //-----------------------------------------------------------------------------
      /// \brief  The update manager
      //-----------------------------------------------------------------------------         
      boost::shared_ptr<update::CUpdateManager> m_updateManager;

      //-----------------------------------------------------------------------------
      /// \brief  The rest keyword
      //-----------------------------------------------------------------------------         
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 

#pragma once

#include "IRestService.h"
#include "update/UpdateManager.h"
#include <shared/enumeration/EnumHelpers.hpp>
#include "update/worker/IUpdateChecker.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
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
            CUpdate(boost::shared_ptr<update::CUpdateManager> updateManager,
                    boost::shared_ptr<update::worker::IUpdateChecker> updateChecker);

            //-----------------------------------------------------------------------------
            /// \brief  Destructor
            //-----------------------------------------------------------------------------      
            virtual ~CUpdate();


            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            //-----------------------------------------------------------------------------
            /// \brief  Get the REST keyword
            /// \return The REST keyword
            //-----------------------------------------------------------------------------      
            static const std::string& getRestKeyword();
         private:
            //-----------------------------------------------------
            ///\brief The operation type to do
            //-----------------------------------------------------
            DECLARE_ENUM_HEADER(EWhatToDo,
               ((Update)(0))
               ((Check)(1))
            )

            //-----------------------------------------------------------------------------
            /// \brief  List all available updates
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer availableUpdates(const std::vector<std::string>& parameters,
                                                    const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  List all available versions of Yadoms
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer availableYadomsVersions(const std::vector<std::string>& parameters,
                                                           const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update Yadoms to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer updateYadoms(const std::vector<std::string>& parameters,
                                                const std::string& requestContent) const;


            //-----------------------------------------------------------------------------
            /// \brief  Get the list of all available widgets from the server
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer availableWidgets(const std::vector<std::string>& parameters,
                                                    const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update a widget to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------   
            shared::CDataContainer updateWidget(const std::vector<std::string>& parameters,
                                                const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Install a new widget
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------    
            shared::CDataContainer installWidget(const std::vector<std::string>& parameters,
                                                 const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Remove a widget
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------   
            shared::CDataContainer removeWidget(const std::vector<std::string>& parameters,
                                                const std::string& requestContent) const;


            //-----------------------------------------------------------------------------
            /// \brief  Get the list of all available plugins from the server
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer availablePlugins(const std::vector<std::string>& parameters,
                                                    const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update a plugin to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer updatePlugin(const std::vector<std::string>& parameters,
                                                const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Install a new plugin
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer installPlugin(const std::vector<std::string>& parameters,
                                                 const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Remove a plugin
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer removePlugin(const std::vector<std::string>& parameters,
                                                const std::string& requestContent) const;


            //-----------------------------------------------------------------------------
            /// \brief  Get the list of all available scriptInterpreters from the server
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer availableScriptInterpreters(const std::vector<std::string>& parameters,
                                                               const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update a scriptInterpreter to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer updateScriptInterpreter(const std::vector<std::string>& parameters,
                                                           const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Install a new scriptInterpreter
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer installScriptInterpreter(const std::vector<std::string>& parameters,
                                                            const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Remove a scriptInterpreter
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            shared::CDataContainer removeScriptInterpreter(const std::vector<std::string>& parameters,
                                                           const std::string& requestContent) const;


            //-----------------------------------------------------------------------------
            /// \brief  The dependancies
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<update::CUpdateManager> m_updateManager;
            boost::shared_ptr<update::worker::IUpdateChecker> m_updateChecker;

            //-----------------------------------------------------------------------------
            /// \brief  The rest keyword
            //-----------------------------------------------------------------------------         
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 

#pragma once
#include "IRestService.h"
#include "update/IUpdateManager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         //-----------------------------------------------------------------------------
         /// \brief  REST handler class for update and check for update
         //-----------------------------------------------------------------------------      
         class CUpdate final : public IRestService
         {
         public:
            //-----------------------------------------------------------------------------
            /// \brief  Constructor
            /// \param [in] updateManager   The update manager
            //-----------------------------------------------------------------------------      
            explicit CUpdate(boost::shared_ptr<update::IUpdateManager> updateManager);
            ~CUpdate() override = default;


            // IRestService implementation
            void configurePocoDispatcher(CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestAccessPoint>>> accessPoints() override;
            // [END] IRestService implementation

            //-----------------------------------------------------------------------------
            /// \brief  Get the REST keyword
            /// \return The REST keyword
            //-----------------------------------------------------------------------------      
            static const std::string& getRestKeyword();
         private:
            boost::shared_ptr<shared::serialization::IDataSerializable> scanForUpdates(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> availableUpdates(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update Yadoms to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> updateYadoms(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update a widget to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------   
            boost::shared_ptr<shared::serialization::IDataSerializable> updateWidget(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Install a new widget
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------    
            boost::shared_ptr<shared::serialization::IDataSerializable> installWidget(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Remove a widget
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------   
            boost::shared_ptr<shared::serialization::IDataSerializable> removeWidget(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update a plugin to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> updatePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Install a new plugin
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> installPlugin(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Remove a plugin
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> removePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Update a scriptInterpreter to another version (upgrade or downgrade)
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> updateScriptInterpreter(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Install a new scriptInterpreter
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> installScriptInterpreter(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;

            //-----------------------------------------------------------------------------
            /// \brief  Remove a scriptInterpreter
            /// \param [in]   parameters        The url parameters
            /// \param [in]   requestContent    The url content
            /// \return the request result
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<shared::serialization::IDataSerializable> removeScriptInterpreter(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const;


            //-----------------------------------------------------------------------------
            /// \brief  The dependancies
            //-----------------------------------------------------------------------------         
            boost::shared_ptr<update::IUpdateManager> m_updateManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestAccessPoint>>> m_accessPoints;

            //-----------------------------------------------------------------------------
            /// \brief  The rest keyword
            //-----------------------------------------------------------------------------         
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 

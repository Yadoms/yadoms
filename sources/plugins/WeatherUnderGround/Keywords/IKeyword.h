#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The KeyWord Interface interface
//--------------------------------------------------------------
class IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IKeyword()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                      initialize the keyword (Declaration)
   /// \param[in] api                Yadoms APi context
   //--------------------------------------------------------------
   virtual void initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                           shared::CDataContainer details) const = 0;

   //--------------------------------------------------------------
   /// \brief	                      Get the IHistorizable variable
   /// \return                       Historizable object
   //--------------------------------------------------------------
   virtual boost::shared_ptr<yApi::historization::IHistorizable> getHistorizable() const = 0;
};


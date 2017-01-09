#pragma once
#include <shared/script/yInterpreterApi/IInformation.h>
#include <interpreter_IPC/interpreterInit.pb.h>

namespace interpreter_cpp_api
{
   class CInformation :public shared::script::yInterpreterApi::IInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    buffer : Protobuf buffer
      //--------------------------------------------------------------
      explicit CInformation(boost::shared_ptr<const interpreter_IPC::toInterpreter::Information> buffer);

      virtual ~CInformation();

      // shared::script::yInterpreterApi::IInformation implementation
      const std::string& getType() const override;
      const std::string& getName() const override;
      const std::string& getDescription() const override;
      const std::string& getVersion() const override;
      const std::string& getAuthor() const override;
      const std::string& getUrl() const override;
      const std::string& getCredits() const override;
      bool isSupportedOnThisPlatform() const override;
      boost::shared_ptr<const shared::CDataContainer> getPackage() const override;
      const boost::filesystem::path& getPath() const override;
      // [END] shared::script::yInterpreterApi::IInformation implementation

   private:
      boost::shared_ptr<const interpreter_IPC::toInterpreter::Information> m_buffer;
      const boost::filesystem::path m_path;
   };
} // namespace interpreter_cpp_api	



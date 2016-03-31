#include "stdafx.h"
#include "NativeExecutableLoader.h"

namespace pluginSystem
{

CNativeExecutableLoader::CNativeExecutableLoader()
{
}

CNativeExecutableLoader::~CNativeExecutableLoader()
{
}

boost::shared_ptr<shared::process::IRunner> CNativeExecutableLoader::createRunner(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                                  boost::shared_ptr<shared::process::ILogger> logger,
                                                                                  const std::string& apiContextId,
                                                                                  boost::shared_ptr<IInstanceStateHandler> stopNotifier) const
{

}

} // namespace pluginSystem

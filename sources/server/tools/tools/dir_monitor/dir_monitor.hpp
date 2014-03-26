// 
// Copyright (c) 2008, 2009 Boris Schaeling <boris@highscore.de> 
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 
#pragma once

#include "basic_dir_monitor.hpp" 
#include <tools/dir_monitor/basic_dir_monitor_service.hpp>

namespace boost { 
namespace asio { 

typedef basic_dir_monitor<basic_dir_monitor_service<> > dir_monitor; 

} 
} 


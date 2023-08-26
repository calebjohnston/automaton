//
//  api.h
//
//  Created by Caleb Johnston on 8/26/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <deque>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

#include "model.h"

namespace Auto {

#pragma game model state

Result test_api_1(Command& cmd);
Result test_api_2(Command& cmd);
Result test_api_3(Command& cmd);
Result api_file_list(Command& cmd);
Result api_file_info(Command& cmd);
Result api_copy_file(Command& cmd);
Result api_delete_file(Command& cmd);
Result api_uninstall_program(Command& cmd);
Result api_install_program(Command& cmd);
Result api_not_implemented(Command& cmd);

#pragma transformation functions

Result install_program(Kernel& kernel, const std::string& name);
Result install_daemon(Kernel& kernel, const std::string& name);
Result uninstall_program(Kernel& kernel, const std::string& name);
Result uninstall_daemon(Kernel& kernel, const std::string& name);
Result delete_file(Kernel& kernel, const std::string& name);

}	// namespace Auto

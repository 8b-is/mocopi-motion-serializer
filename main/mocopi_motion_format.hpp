/*
* Copyright (C) 2025 Sony Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef mocopi_motion_format_hpp
#define mocopi_motion_format_hpp

#include <stdio.h>
#include <string>
#include <cstdio>
#include "./box.hpp"
#include "./enums.hpp"
#include "./const.hpp"

#if __APPLE__
#import <TargetConditionals.h>
#if TARGET_OS_MAC
#include <sys/types.h>
#endif
#endif

namespace mocopi_motion_format {

class MocopiMotionFormat{
public:
  Box* box_header;
  Box* box_sender_difinition;
  Box* box_data;
  
  MocopiMotionFormat(Box* box_header, Box* box_sender_difinition, Box* box_data);
  MocopiMotionFormat();
  MocopiMotionFormat(std::string sender_ip, unsigned short sender_port);
  bool IsSkeletonDefinition();
  bool IsFrameData();
  void DeleteDeta();
private:
  void SetUp(std::string sender_ip, unsigned short sender_port);
};

}

#endif /* mocopi_motion_format_hpp */

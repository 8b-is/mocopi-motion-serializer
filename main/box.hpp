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

#ifndef box_hpp
#define box_hpp

#include <stdio.h>
#include <string>
#include <any>
#include <vector>
#include "./enums.hpp"
#include "./const.hpp"

#if __APPLE__
#import <TargetConditionals.h>
#if TARGET_OS_MAC
#include <sys/types.h>
#endif
#endif

namespace mocopi_motion_format{

class Box{
public:
  int data_size;
  BoxType box_type;
  std::string box_type_4cc;
  std::any data;
  std::vector<Box*> children;
  int data_num; // Number of data elements for array data
  
  float* float_array_data = nullptr;
  uint8_t* uint8_t_array_data = nullptr;
  std::string string_data = "";
  
  Box();
  Box(BoxType box_type, std::any data, int data_size);
  Box(BoxType box_type, std::any data, int data_size, int data_num);
  Box(BoxType box_type, std::vector<Box*> children);
  Box(BoxType box_type, const float* quaternion, const float* vector3);
  Box(BoxType box_type, std::string data);
  Box(BoxType box_type, uint8_t hour, uint8_t min, uint8_t sec, uint8_t frame, uint8_t frame_rate = 0, bool drop_frame = false);
  int GetBoxSize();
  void DeleteData();
  
  static unsigned long long IpAddressToULong(std::string ip_address);
  static std::string ULongToIpAddress(unsigned long long ip_address);
};

}

#endif /* box_hpp */

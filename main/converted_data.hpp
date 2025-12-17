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

#ifndef converted_data_hpp
#define converted_data_hpp

#include <stdio.h>
#include <thread>
#include "./mocopi_motion_format.hpp"
#include "./mocopi_motion_format_utility.hpp"

namespace mocopi_motion_format {

/// Class for temporarily storing converted data to avoid memory deallocation
class ConvertedData{
public:
  void SetData(std::byte* converted_bytes);
  void SetData(
               int*   joint_ids,
               int*   parent_joint_ids,
               float* rotations_x,
               float* rotations_y,
               float* rotations_z,
               float* rotations_w,
               float* positions_x,
               float* positions_y,
               float* positions_z
               );
  void SetData(
               int*   joint_ids,
               float* rotations_x,
               float* rotations_y,
               float* rotations_z,
               float* rotations_w,
               float* positions_x,
               float* positions_y,
               float* positions_z
               );
  
  bool SetConvertedData(
                        std::byte* converted_bytes,
                        int*   joint_ids,
                        int*   parent_joint_ids,
                        float* rotations_x,
                        float* rotations_y,
                        float* rotations_z,
                        float* rotations_w,
                        float* positions_x,
                        float* positions_y,
                        float* positions_z
                        );
  
private:
  int*        joint_ids         = nullptr;
  int*        parent_joint_ids  = nullptr;
  float*      rotations_x       = nullptr;
  float*      rotations_y       = nullptr;
  float*      rotations_z       = nullptr;
  float*      rotations_w       = nullptr;
  float*      positions_x       = nullptr;
  float*      positions_y       = nullptr;
  float*      positions_z       = nullptr;
  std::byte*  converted_bytes   = nullptr;
  
  void DeleteData();
};

}

#endif /* converted_data_hpp */

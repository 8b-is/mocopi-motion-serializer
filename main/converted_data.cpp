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

#include "converted_data.hpp"

namespace mocopi_motion_format {

void ConvertedData::SetData(std::byte *converted_bytes)
{
  SetConvertedData(
                   converted_bytes,
                   nullptr, nullptr,
                   nullptr, nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr
                   );
}

void ConvertedData::SetData(
                            int *joint_ids, int *parent_joint_ids,
                            float *rotations_x,
                            float *rotations_y,
                            float *rotations_z,
                            float *rotations_w,
                            float *positions_x,
                            float *positions_y,
                            float *positions_z)
{
  SetConvertedData(
                   nullptr,
                   joint_ids,
                   parent_joint_ids,
                   rotations_x,
                   rotations_y,
                   rotations_z,
                   rotations_w,
                   positions_x,
                   positions_y,
                   positions_z
                   );
}

void ConvertedData::SetData(
                            int *joint_ids,
                            float *rotations_x,
                            float *rotations_y,
                            float *rotations_z,
                            float *rotations_w,
                            float *positions_x,
                            float *positions_y,
                            float *positions_z)
{
  SetConvertedData(
                   nullptr,
                   joint_ids, nullptr,
                   rotations_x, rotations_y,
                   rotations_z,
                   rotations_w,
                   positions_x,
                   positions_y,
                   positions_z
                   );
}

bool ConvertedData::SetConvertedData(
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
                                     )
{  
  DeleteData();
  this->converted_bytes = converted_bytes;
  this->joint_ids = joint_ids;
  this->parent_joint_ids = parent_joint_ids;
  this->rotations_x = rotations_x;
  this->rotations_y = rotations_y;
  this->rotations_z = rotations_z;
  this->rotations_w = rotations_w;
  this->positions_x = positions_x;
  this->positions_y = positions_y;
  this->positions_z = positions_z;

  return true;
}

void ConvertedData::DeleteData()
{
  // Properly release dynamically allocated memory to prevent memory leaks
  if(joint_ids != nullptr){
    delete [] joint_ids;
    joint_ids = nullptr;
  }
  
  if(parent_joint_ids != nullptr){
    delete [] parent_joint_ids;
    parent_joint_ids = nullptr;
  }
  
  if(rotations_x != nullptr){
    delete [] rotations_x;
    rotations_x = nullptr;
  }
  
  if(rotations_y != nullptr){
    delete [] rotations_y;
    rotations_y = nullptr;
  }
  
  if(rotations_z != nullptr){
    delete [] rotations_z;
    rotations_z = nullptr;
  }
  
  if(rotations_w != nullptr){
    delete [] rotations_w;
    rotations_w = nullptr;
  }
  
  if(positions_x != nullptr){
    delete [] positions_x;
    positions_x = nullptr;
  }
  
  if(positions_y != nullptr){
    delete [] positions_y;
    positions_y = nullptr;
  }
  
  if(positions_z != nullptr){
    delete [] positions_z;
    positions_z = nullptr;
  }
  
  if(converted_bytes != nullptr){
    delete [] converted_bytes;
    converted_bytes = nullptr;
  }
}

}

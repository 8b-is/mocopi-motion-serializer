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

#ifndef mocopi_motion_format_manager_hpp
#define mocopi_motion_format_manager_hpp

#include <stdio.h>
#include <string>

#include <mutex>

#include "./mocopi_motion_format.hpp"
#include "./mocopi_motion_format_utility.hpp"
#include "./converted_data.hpp"
#include "./logger.hpp"

namespace mocopi_motion_format{

class MocopiMotionFormatManager{
public:
  MocopiMotionFormatManager();
  ~MocopiMotionFormatManager();
  
  bool ConvertSkeletonDefinitionToBytes(
                                        char* sender_ip,
                                        int sender_port,
                                        int data_size,
                                        int* joint_ids,
                                        int* parent_joint_ids,
                                        float* rotations_x,
                                        float* rotations_y,
                                        float* rotations_z,
                                        float* rotations_w,
                                        float* positions_x,
                                        float* positions_y,
                                        float* positions_z,
                                        int& converted_bytes_size,
                                        std::byte*& converted_bytes
                                        );
  bool ConvertFrameDataToBytes(
                               char* sender_ip,
                               int sender_port,
                               int frame_number,
                               float timestamp,
                               double utc_time,
                               uint8_t timecode_hour,
                               uint8_t timecode_min,
                               uint8_t timecode_sec,
                               uint8_t timecode_frame,
                               uint8_t timecode_frame_rate,
                               bool    timecode_drop_frame,
                               int data_size,
                               int* joint_ids,
                               float* rotations_x,
                               float* rotations_y,
                               float* rotations_z,
                               float* rotations_w,
                               float* positions_x,
                               float* positions_y,
                               float* positions_z,
                               int& converted_bytes_size,
                               std::byte*& converted_bytes
                               );
  
  // NOTE: sensor_address and sensor_name support only single-byte characters
  bool ConvertSensorDataToBytes(
                                char* sender_ip,
                                int sender_port,
                                double utc_time,
                                char* sensor_address,
                                char* sensor_name,
                                int sensor_joint_id,
                                float rotation_x,
                                float rotation_y,
                                float rotation_z,
                                float rotation_w,
                                float velocity_x,
                                float velocity_y,
                                float velocity_z,
                                int& converted_bytes_size,
                                std::byte*& converted_bytes
                                );
  
  bool IsMmfBytes(int bytes_size, std::byte* bytes);
  bool IsSkeletonDefinitionBytes(int bytes_size, std::byte* bytes);
  bool IsFrameDataBytes(int bytessize, std::byte* bytes);
  bool IsSensorDataBytes(int bytessize, std::byte* bytes);
  
  bool ConvertBytesToSkeletonDefinition(
                                        int bytes_size,
                                        std::byte* bytes,
                                        unsigned long long& sender_ip,
                                        int& sender_port,
                                        int& data_size,
                                        int*& joint_ids,
                                        int*& parent_joint_ids,
                                        float*& rotations_x,
                                        float*& rotations_y,
                                        float*& rotations_z,
                                        float*& rotations_w,
                                        float*& positions_x,
                                        float*& positions_y,
                                        float*& positions_z
                                        );
  bool ConvertBytesToFrameData(
                               int bytes_size,
                               std::byte* bytes,
                               unsigned long long& sender_ip,
                               int& sender_port,
                               int& frame_number,
                               float& timestamp,
                               double& utc_time,
                               uint8_t& timecode_hour,
                               uint8_t& timecode_min,
                               uint8_t& timecode_sec,
                               uint8_t& timecode_frame,
                               uint8_t& timecode_frame_rate,
                               bool&    timecode_drop_frame,
                               int& data_size,
                               int*& joint_ids,
                               float*& rotations_x,
                               float*& rotations_y,
                               float*& rotations_z,
                               float*& rotations_w,
                               float*& positions_x,
                               float*& positions_y,
                               float*& positions_z
                               );
  
  bool ConvertBytesToSensorData(
                                int bytes_size,
                                std::byte* bytes,
                                unsigned long long& sender_ip,
                                int& sender_port,
                                double& utc_time,
                                char*& sensor_address,
                                char*& sensor_name,
                                int& sensor_joint_id,
                                float& rotation_x,
                                float& rotation_y,
                                float& rotation_z,
                                float& rotation_w,
                                float& velocity_x,
                                float& velocity_y,
                                float& velocity_z
                                );
  
  static MocopiMotionFormatManager* GetInstance() {
    if(instance == nullptr){
      instance = new MocopiMotionFormatManager();
    }
    return instance;
  }
  
private:
  std::mutex convert_mtx;
  
  static MocopiMotionFormatManager* instance;
};

}

#endif /* mocopi_motion_format_manager_hpp */

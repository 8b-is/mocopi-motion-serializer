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

#include "mocopi_motion_format_api.hpp"
  
#include "./mocopi_motion_format_manager.hpp"

using mocopi_motion_format::MocopiMotionFormatManager;

MOCOPI_MOTION_FORMAT_API bool ConvertSkeletonDefinitionToBytes(
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
                                                             )
{
  return MocopiMotionFormatManager::GetInstance()->ConvertSkeletonDefinitionToBytes(
                                                                                  sender_ip,
                                                                                  sender_port,
                                                                                  data_size,
                                                                                  joint_ids,
                                                                                  parent_joint_ids,
                                                                                  rotations_x,
                                                                                  rotations_y,
                                                                                  rotations_z,
                                                                                  rotations_w,
                                                                                  positions_x,
                                                                                  positions_y,
                                                                                  positions_z,
                                                                                  converted_bytes_size,
                                                                                  converted_bytes
                                                                                  );
}

MOCOPI_MOTION_FORMAT_API bool ConvertFrameDataToBytes(
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
                                                    )
{
  return MocopiMotionFormatManager::GetInstance()->ConvertFrameDataToBytes(
                                                                         sender_ip,
                                                                         sender_port,
                                                                         frame_number,
                                                                         timestamp,
                                                                         utc_time,
                                                                         timecode_hour,
                                                                         timecode_min,
                                                                         timecode_sec,
                                                                         timecode_frame,
                                                                         timecode_frame_rate,
                                                                         timecode_drop_frame,
                                                                         data_size,
                                                                         joint_ids,
                                                                         rotations_x,
                                                                         rotations_y,
                                                                         rotations_z,
                                                                         rotations_w,
                                                                         positions_x,
                                                                         positions_y,
                                                                         positions_z,
                                                                         converted_bytes_size,
                                                                         converted_bytes
                                                                         );
}

MOCOPI_MOTION_FORMAT_API bool ConvertSensorDataToBytes(
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
                                                     )
{
  return MocopiMotionFormatManager::GetInstance()->ConvertSensorDataToBytes(
                                                                          sender_ip,
                                                                          sender_port,
                                                                          utc_time,
                                                                          sensor_address,
                                                                          sensor_name,
                                                                          sensor_joint_id,
                                                                          rotation_x,
                                                                          rotation_y,
                                                                          rotation_z,
                                                                          rotation_w,
                                                                          velocity_x,
                                                                          velocity_y,
                                                                          velocity_z,
                                                                          converted_bytes_size,
                                                                          converted_bytes
                                                                          );
}

MOCOPI_MOTION_FORMAT_API bool IsMmfBytes(int bytes_size, std::byte* bytes) {
  return MocopiMotionFormatManager::GetInstance()->IsMmfBytes(bytes_size, bytes);
}

MOCOPI_MOTION_FORMAT_API bool IsSkeletonDefinitionBytes(int bytes_size, std::byte* bytes) {
  return MocopiMotionFormatManager::GetInstance()->IsSkeletonDefinitionBytes(bytes_size, bytes);
}

MOCOPI_MOTION_FORMAT_API bool IsFrameDataBytes(int bytes_size, std::byte* bytes) {
  return MocopiMotionFormatManager::GetInstance()->IsFrameDataBytes(bytes_size, bytes);
}

MOCOPI_MOTION_FORMAT_API bool IsSensorDataBytes(int bytes_size, std::byte* bytes) {
  return MocopiMotionFormatManager::GetInstance()->IsSensorDataBytes(bytes_size, bytes);
}

MOCOPI_MOTION_FORMAT_API bool ConvertBytesToSkeletonDefinition(
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
                                                             )
{
  return MocopiMotionFormatManager::GetInstance()->ConvertBytesToSkeletonDefinition(
                                                                                  bytes_size,
                                                                                  bytes,
                                                                                  sender_ip,
                                                                                  sender_port,
                                                                                  data_size,
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

MOCOPI_MOTION_FORMAT_API bool ConvertBytesToFrameData(
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
                                                    )
{
  return MocopiMotionFormatManager::GetInstance()->ConvertBytesToFrameData(
                                                                         bytes_size,
                                                                         bytes,
                                                                         sender_ip,
                                                                         sender_port,
                                                                         frame_number,
                                                                         timestamp,
                                                                         utc_time,
                                                                         timecode_hour,
                                                                         timecode_min,
                                                                         timecode_sec,
                                                                         timecode_frame,
                                                                         timecode_frame_rate,
                                                                         timecode_drop_frame,
                                                                         data_size,
                                                                         joint_ids,
                                                                         rotations_x,
                                                                         rotations_y,
                                                                         rotations_z,
                                                                         rotations_w,
                                                                         positions_x,
                                                                         positions_y,
                                                                         positions_z
                                                                         );
}

MOCOPI_MOTION_FORMAT_API bool ConvertBytesToSensorData(
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
                                                    )
{
  return MocopiMotionFormatManager::GetInstance()->ConvertBytesToSensorData(
                                                                          bytes_size,
                                                                          bytes,
                                                                          sender_ip,
                                                                          sender_port,
                                                                          utc_time,
                                                                          sensor_address,
                                                                          sensor_name,
                                                                          sensor_joint_id,
                                                                          rotation_x,
                                                                          rotation_y,
                                                                          rotation_z,
                                                                          rotation_w,
                                                                          velocity_x,
                                                                          velocity_y,
                                                                          velocity_z
                                                                          );
}

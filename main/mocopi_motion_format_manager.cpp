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

#include "mocopi_motion_format_manager.hpp"

namespace  mocopi_motion_format{

using mocopi_motion_format::MocopiMotionFormat;
using mocopi_motion_format::MocopiMotionFormatUtility;
using mocopi_motion_format::Box;
using mocopi_motion_format::BoxType;
using mocopi_motion_format::ConvertedData;
using mocopi_motion_format::Logger;

ConvertedData* converted_data = new ConvertedData();

MocopiMotionFormatManager* MocopiMotionFormatManager::instance = nullptr;

MocopiMotionFormatManager::MocopiMotionFormatManager(){

}

MocopiMotionFormatManager::~MocopiMotionFormatManager(){
  if(converted_data != nullptr){
    delete converted_data;
    converted_data = nullptr;
  }
}

bool MocopiMotionFormatManager::ConvertSkeletonDefinitionToBytes(
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
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  MocopiMotionFormat* mmf = MocopiMotionFormatUtility::ConvertSkeletonDefinitionToMmf(
                                                                                  std::string(sender_ip),
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
  
  if(mmf == nullptr){
    return false;
  }
  
  std::vector<std::byte> bytes = MocopiMotionFormatUtility::ConvertMmfToBytes(mmf);
  
  converted_bytes_size = (int)bytes.size();
  converted_bytes = new std::byte[converted_bytes_size];
  for(int i=0; i<converted_bytes_size; i++){
    converted_bytes[i] = bytes[i];
  }
  
  bytes.clear();
  bytes.shrink_to_fit();
  
  mmf->DeleteDeta();
  delete mmf;
  mmf = nullptr;
  
  converted_data->SetData(converted_bytes);
  
  return true;
}

bool MocopiMotionFormatManager::ConvertFrameDataToBytes(
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
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  MocopiMotionFormat* Mmf = MocopiMotionFormatUtility::ConvertFrameDataToMmf(
                                                                         std::string(sender_ip),
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
  
  if(Mmf == nullptr){
    Logger::LogError("mmf is null");
    return false;
  }
  
  std::vector<std::byte> bytes = MocopiMotionFormatUtility::ConvertMmfToBytes(Mmf);
  
  converted_bytes_size = (int)bytes.size();
  converted_bytes = new std::byte[converted_bytes_size];
  for(int i=0; i<converted_bytes_size; i++){
    converted_bytes[i] = bytes[i];
  }
  
  bytes.clear();
  bytes.shrink_to_fit();
  
  Mmf->DeleteDeta();
  delete Mmf;
  Mmf = nullptr;
  
  converted_data->SetData(converted_bytes);
  
  return true;
}

bool MocopiMotionFormatManager::ConvertSensorDataToBytes(
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
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  MocopiMotionFormat* mmf = MocopiMotionFormatUtility::ConvertSensorDataToMmf(
                                                                          std::string(sender_ip),
                                                                          sender_port,
                                                                          utc_time,
                                                                          std::string(sensor_address),
                                                                          std::string(sensor_name),
                                                                          sensor_joint_id,
                                                                          rotation_x,
                                                                          rotation_y,
                                                                          rotation_z,
                                                                          rotation_w,
                                                                          velocity_x,
                                                                          velocity_y,
                                                                          velocity_z
                                                                          );
  
  if(mmf == nullptr){
    Logger::LogError("mmf is null");
    return false;
  }
  
  std::vector<std::byte> bytes = MocopiMotionFormatUtility::ConvertMmfToBytes(mmf);
  
  converted_bytes_size = (int)bytes.size();
  converted_bytes = new std::byte[converted_bytes_size];
  for(int i=0; i<converted_bytes_size; i++){
    converted_bytes[i] = bytes[i];
  }
  
  bytes.clear();
  bytes.shrink_to_fit();
  
  mmf->DeleteDeta();
  delete mmf;
  mmf = nullptr;
  
  converted_data->SetData(converted_bytes);
  
  return true;
}

bool MocopiMotionFormatManager::IsMmfBytes(int bytes_size, std::byte* bytes) {
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  bool result = MocopiMotionFormatUtility::IsMmfBytes(vec_bytes);
  
  return result;
}

bool MocopiMotionFormatManager::IsSkeletonDefinitionBytes(int bytes_size, std::byte* bytes) {
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  bool result = MocopiMotionFormatUtility::IsSkeletonDefinitionBytes(vec_bytes);
  
  return result;
}

bool MocopiMotionFormatManager::IsFrameDataBytes(int bytes_size, std::byte* bytes) {
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  bool result = MocopiMotionFormatUtility::IsFrameDataBytes(vec_bytes);
  
  return result;
}

bool MocopiMotionFormatManager::IsSensorDataBytes(int bytes_size, std::byte* bytes) {
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  bool result = MocopiMotionFormatUtility::IsSensorDataBytes(vec_bytes);
  
  return result;
}

bool MocopiMotionFormatManager::ConvertBytesToSkeletonDefinition(
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
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  std::string sender_ip_str = "";
  
  bool result = MocopiMotionFormatUtility::ConvertBytesToSkeletonDefinition(
                                                                          vec_bytes,
                                                                          sender_ip_str,
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
  
  sender_ip = Box::IpAddressToULong(sender_ip_str);
  
  converted_data->SetData(
                          joint_ids, parent_joint_ids,
                          rotations_x,
                          rotations_y,
                          rotations_z,
                          rotations_w,
                          positions_x,
                          positions_y,
                          positions_z
                          );
  
  return result;
}

bool MocopiMotionFormatManager::ConvertBytesToFrameData(
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
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  std::string sender_ip_str = "";
  
  bool result = MocopiMotionFormatUtility::ConvertBytesToFrameData(
                                                                 vec_bytes,
                                                                 sender_ip_str,
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
  
  sender_ip = Box::IpAddressToULong(sender_ip_str);
  
  converted_data->SetData(
                          joint_ids,
                          rotations_x,
                          rotations_y,
                          rotations_z,
                          rotations_w,
                          positions_x,
                          positions_y,
                          positions_z
                          );
  
  return result;
}

bool MocopiMotionFormatManager::ConvertBytesToSensorData(
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
  std::lock_guard<std::mutex> lock(convert_mtx);
  
  std::vector<std::byte> vec_bytes{};
  for(int i=0; i<bytes_size; i++){
    vec_bytes.push_back(bytes[i]);
  }
  
  std::string sender_ip_str = "";
  std::string sensor_address_str = "";
  std::string sensor_name_str = "";
  
  bool result = MocopiMotionFormatUtility::ConvertBytesToSensorData(
                                                                  vec_bytes,
                                                                  sender_ip_str,
                                                                  sender_port,
                                                                  utc_time,
                                                                  sensor_address_str,
                                                                  sensor_name_str,
                                                                  sensor_joint_id,
                                                                  rotation_x,
                                                                  rotation_y,
                                                                  rotation_z,
                                                                  rotation_w,
                                                                  velocity_x,
                                                                  velocity_y,
                                                                  velocity_z
                                                                  );
  
  sender_ip = Box::IpAddressToULong(sender_ip_str);

  sensor_address = new char[sensor_address_str.size() + 1];
  std::copy(sensor_address_str.begin(), sensor_address_str.end(), sensor_address);
  sensor_address[sensor_address_str.size()] = '\0';
  
  sensor_name = new char[sensor_name_str.size() + 1];
  std::copy(sensor_name_str.begin(), sensor_name_str.end(), sensor_name);
  sensor_name[sensor_name_str.size()] = '\0';
   
  return result;
}


}

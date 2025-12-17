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

#include "mocopi_motion_format_utility.hpp"

namespace mocopi_motion_format{

using mocopi_motion_format::Logger;

const std::type_info& MocopiMotionFormatUtility::GetDataTypeFromBoxType(
                                                                      BoxType box_type,
                                                                      bool& is_success
                                                                      )
{
  is_success = false;
  
  switch (box_type) {
    case BoxType::Head:
      is_success = true;
      return typeid(Box*);
    case BoxType::FormatType:
      is_success = true;
      return typeid(std::string);
    case BoxType::Version:
      is_success = true;
      return typeid(std::byte);
    case BoxType::SenderDefinition:
      is_success = true;
      return typeid(Box*);
    case BoxType::IpAddress:
      is_success = true;
      return typeid(unsigned long long);
    case BoxType::RecievingPort:
      is_success = true;
      return typeid(unsigned short);
    case BoxType::SkeletonDefinition:
      is_success = true;
      return typeid(Box*);
    case BoxType::BoneDataArray:
      is_success = true;
      return typeid(Box*);
    case BoxType::BoneData:
      is_success = true;
      return typeid(Box*);
    case BoxType::BoneId:
      is_success = true;
      return typeid(short);
    case BoxType::ParentBoneId:
      is_success = true;
      return typeid(short);
    case BoxType::TransformData:
      is_success = true;
      // NOTE: Using float instead of double to reduce memory usage
      return typeid(float*);
    case BoxType::FrameData:
      is_success = true;
      return typeid(Box*);
    case BoxType::FrameNumber:
      is_success = true;
      return typeid(int);
    case BoxType::Timestamp:
      is_success = true;
      return typeid(float);
    case BoxType::UtcTime:
      is_success = true;
      return typeid(double);
    case BoxType::Timecode:
      is_success = true;
      return typeid(uint8_t*);
    case BoxType::BoneTransformDataArray:
      is_success = true;
      return typeid(Box*);
    case BoxType::BoneTransformData:
      is_success = true;
      return typeid(Box*);
    case BoxType::SensorData:
      is_success = true;
      return typeid(Box*);
    case BoxType::SensorAddress:
      is_success = true;
      return typeid(std::string);
    case BoxType::SensorName:
      is_success = true;
      return typeid(std::string);
    case BoxType::SensorJointId:
      is_success = true;
      return typeid(int);
    case BoxType::SensorValue:
      is_success = true;
      // NOTE: Using float instead of double to reduce memory usage
      return typeid(float*);
    default:
      Logger::LogError("Get box type failed");
      is_success = false;
      return typeid(int);
  }
}

std::vector<std::byte> MocopiMotionFormatUtility::ConvertSkeletonDefinitionToBytes(
                                                                                 std::string sender_ip,
                                                                                 int sender_port,
                                                                                 int data_size,
                                                                                 int *joint_ids,
                                                                                 int *parent_joint_ids,
                                                                                 float *rotations_x,
                                                                                 float *rotations_y,
                                                                                 float *rotations_z,
                                                                                 float *rotations_w,
                                                                                 float *positions_x,
                                                                                 float *positions_y,
                                                                                 float *positions_z
                                                                                 )
{
  MocopiMotionFormat* mmf = ConvertSkeletonDefinitionToMmf(
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
  
  if(mmf == nullptr){
    return std::vector<std::byte>();
  }
  
  return ConvertMmfToBytes(mmf);
}

MocopiMotionFormat* MocopiMotionFormatUtility::ConvertSkeletonDefinitionToMmf(
                                                                          std::string sender_ip,
                                                                          int sender_port,
                                                                          int data_size,
                                                                          int *joint_ids,
                                                                          int *parent_joint_ids,
                                                                          float *rotations_x,
                                                                          float *rotations_y,
                                                                          float *rotations_z,
                                                                          float *rotations_w,
                                                                          float *positions_x,
                                                                          float *positions_y,
                                                                          float *positions_z
                                                                          )
{
  std::vector<Box*> boxes;
  
  std::vector<Box*> bone_data_array_boxes;
  for(int i=0; i<data_size; i++){
    float rotation[] = {
      rotations_x[i],
      rotations_y[i],
      rotations_z[i],
      rotations_w[i]
    };
    
    float position[] = {
      positions_x[i],
      positions_y[i],
      positions_z[i]
    };
    
    std::vector<Box*> bone_data_box;
    bone_data_box.push_back(new Box(
                                    BoxType::BoneId,
                                    (short)joint_ids[i],
                                    sizeof(short)
                                    )
                            );
    bone_data_box.push_back(new Box(
                                    BoxType::ParentBoneId,
                                    (short)parent_joint_ids[i],
                                    sizeof(short)
                                    )
                            );
    bone_data_box.push_back(new Box(BoxType::TransformData, rotation, position));
    
    Box* bone_box = new Box(BoxType::BoneData, bone_data_box);
    
    bone_data_array_boxes.push_back(bone_box);
  }
  
  boxes.push_back(new Box(BoxType::BoneDataArray, bone_data_array_boxes));
  
  Box* skeleton_definition_data = new Box(BoxType::SkeletonDefinition, boxes);
  
  MocopiMotionFormat* mmf = new MocopiMotionFormat(sender_ip, sender_port);
  mmf->box_data = skeleton_definition_data;
  
  return mmf;
}

std::vector<std::byte> MocopiMotionFormatUtility::ConvertFrameDataToBytes(
                                                                        std::string sender_ip,
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
                                                                        int *joint_ids,
                                                                        float *rotations_x,
                                                                        float *rotations_y,
                                                                        float *rotations_z,
                                                                        float *rotations_w,
                                                                        float *positions_x,
                                                                        float *positions_y,
                                                                        float *positions_z
                                                                        )
{
  MocopiMotionFormat* mmf = ConvertFrameDataToMmf(
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
  
  if(mmf == nullptr){
    return std::vector<std::byte>();
  }
  
  return ConvertMmfToBytes(mmf);
}

MocopiMotionFormat* MocopiMotionFormatUtility::ConvertFrameDataToMmf(
                                                                 std::string sender_ip,
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
                                                                 int *joint_ids,
                                                                 float *rotations_x,
                                                                 float *rotations_y,
                                                                 float *rotations_z,
                                                                 float *rotations_w,
                                                                 float *positions_x,
                                                                 float *positions_y,
                                                                 float *positions_z
                                                                 )
{
  std::vector<Box*> boxes;
  
  std::vector<Box*> transform_boxes;
  for(int i=0; i<data_size; i++){
    float rotation[] = {
      rotations_x[i],
      rotations_y[i],
      rotations_z[i],
      rotations_w[i]
    };
    
    float position[] = {
      positions_x[i],
      positions_y[i],
      positions_z[i]
    };
    
    std::vector<Box*> bone_data_box;
    bone_data_box.push_back(new Box(
                                    BoxType::BoneId,
                                    (short)joint_ids[i],
                                    sizeof(short)
                                    )
                            );
    bone_data_box.push_back(new Box(BoxType::TransformData, rotation, position));
    
    Box* bone_box = new Box(BoxType::BoneTransformData, bone_data_box);
    
    transform_boxes.push_back(bone_box);
  }
  
  Box* transform_array_box = new Box(
                                BoxType::BoneTransformDataArray,
                                transform_boxes
                                );
  
  boxes.push_back(new Box(
                          BoxType::FrameNumber,
                          frame_number,
                          sizeof(int)
                          ));
  boxes.push_back(new Box(
                          BoxType::Timestamp,
                          timestamp,
                          sizeof(float)
                          ));
  boxes.push_back(new Box(
                          BoxType::UtcTime,
                          utc_time,
                          sizeof(double)
                          ));
  boxes.push_back(new Box(
                          BoxType::Timecode,
                          timecode_hour,
                          timecode_min,
                          timecode_sec,
                          timecode_frame,
                          timecode_frame_rate,
                          timecode_drop_frame
                          ));
  
  boxes.push_back(transform_array_box);

  Box* frame_data = new Box(BoxType::FrameData, boxes);
  
  MocopiMotionFormat* mmf = new MocopiMotionFormat(sender_ip, sender_port);
  mmf->box_data = frame_data;
  
  return mmf;
}

std::vector<std::byte> MocopiMotionFormatUtility::ConvertSensorDataToBytes(
                                                                         std::string sender_ip,
                                                                         int sender_port,
                                                                         double utc_time,
                                                                         std::string sensor_address,
                                                                         std::string sensor_name,
                                                                         int sensor_joint_id,
                                                                         float rotation_x,
                                                                         float rotation_y,
                                                                         float rotation_z,
                                                                         float rotation_w,
                                                                         float velocity_x,
                                                                         float velocity_y,
                                                                         float velocity_z
                                                                        )
{
  MocopiMotionFormat* mmf = ConvertSensorDataToMmf(
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
  
  if(mmf == nullptr){
    return std::vector<std::byte>();
  }
  
  return ConvertMmfToBytes(mmf);
}

MocopiMotionFormat* MocopiMotionFormatUtility::ConvertSensorDataToMmf(
                                                                  std::string sender_ip,
                                                                  int sender_port,
                                                                  double utc_time,
                                                                  std::string sensor_address,
                                                                  std::string sensor_name,
                                                                  int sensor_joint_id,
                                                                  float rotation_x,
                                                                  float rotation_y,
                                                                  float rotation_z,
                                                                  float rotation_w,
                                                                  float velocity_x,
                                                                  float velocity_y,
                                                                  float velocity_z
                                                                 )
{
  float rotation[] = {
    rotation_x,
    rotation_y,
    rotation_z,
    rotation_w
  };
  
  float velocity[] = {
    velocity_x,
    velocity_y,
    velocity_z
  };
  
  std::vector<Box*> boxes;
  
  boxes.push_back(new Box(
                          BoxType::UtcTime,
                          utc_time,
                          sizeof(double)
                          ));
  
  boxes.push_back(new Box(
                          BoxType::SensorAddress,
                          sensor_address
                          ));
  
  boxes.push_back(new Box(
                          BoxType::SensorName,
                          sensor_name
                          ));
  
  boxes.push_back(new Box(
                          BoxType::SensorJointId,
                          sensor_joint_id,
                          sizeof(int)
                          ));
  
  boxes.push_back(new Box(BoxType::SensorValue, rotation, velocity));
    
  Box* sensor_data = new Box(BoxType::SensorData, boxes);
  
  MocopiMotionFormat* mmf = new MocopiMotionFormat(sender_ip, sender_port);
  mmf->box_data = sensor_data;
  
  return mmf;
}

std::vector<std::byte> MocopiMotionFormatUtility::ConvertMmfToBytes(
                                                                  MocopiMotionFormat* motion_format
                                                                  )
{
  if(motion_format == nullptr){
    Logger::LogError("motion format is null");
    return std::vector<std::byte>();
  }
  
  if(motion_format->box_header == nullptr){
    Logger::LogError("motion format header is null");
    return std::vector<std::byte>();
  }
  
  if(motion_format->box_sender_difinition == nullptr){
    Logger::LogError("motion format sender definition is null");
    return std::vector<std::byte>();
  }
  
  if(motion_format->box_data == nullptr){
    Logger::LogError("motion format data is null");
    return std::vector<std::byte>();
  }
  
  std::vector<Box*> boxes;
  boxes.push_back(motion_format->box_header);
  boxes.push_back(motion_format->box_sender_difinition);
  boxes.push_back(motion_format->box_data);

  return BoxesToBytes(boxes);
}

MocopiMotionFormat* MocopiMotionFormatUtility::ConvertBytesToMmf(std::vector<std::byte> bytes){
  int box_index_header = -1;
  int box_index_sender_definition = -1;
  int box_index_skeleton_definition = -1;
  int box_index_frame_data = -1;
  
  int box_size_header = 0;
  int box_size_sender_definition = 0;
  int box_size_skeleton_definition = 0;
  int box_size_frame_data = 0;
  
  SearchBoxIndex(
                 bytes,
                 BoxType::Head,
                 box_index_header,
                 box_size_header
                 );
  SearchBoxIndex(
                 bytes,
                 BoxType::SenderDefinition,
                 box_index_sender_definition,
                 box_size_sender_definition
                 );
  SearchBoxIndex(
                 bytes,
                 BoxType::SkeletonDefinition,
                 box_index_skeleton_definition,
                 box_size_skeleton_definition
                 );
  SearchBoxIndex(
                 bytes,
                 BoxType::FrameData,
                 box_index_frame_data,
                 box_size_frame_data
                 );

  if(box_index_header < 0 || box_index_sender_definition < 0){
    Logger::LogError("Invalid bytes data.");
    return nullptr;
  }

  std::vector<std::byte> bytes_header = Slice(
                                              bytes,
                                              box_index_header,
                                              box_size_header
                                              );
  std::string format_type = "";
  std::byte version = std::byte(1);
  BytesToHeader(
                bytes_header,
                format_type,
                version
                );
  
  // version check
  if(!IsSupportedVersion(version)){
    Logger::LogError(
                     "Invalid data version. Current version : " + std::to_string((int)Const::FORMAT_VERSION) +
                     ", Target version : " + std::to_string((int)version) + "."
                     );
    return nullptr;
  }

  std::vector<std::byte> bytes_sender_definition = Slice(
                                                         bytes,
                                                         box_index_sender_definition,
                                                         box_size_sender_definition
                                                         );
  std::string sender_ip = "";
  int sender_port = 0;
  BytesToSenderDefinition(
                          bytes_sender_definition,
                          sender_ip,
                          sender_port
                          );
  
  MocopiMotionFormat* mmf = nullptr;
  
  if(box_index_skeleton_definition > 0){
    std::vector<std::byte> bytes_bone_definition = Slice(
                                                         bytes,
                                                         box_index_skeleton_definition,
                                                         box_size_skeleton_definition
                                                         );
    std::vector<int> joint_ids;
    std::vector<int> joint_ids_parent;
    std::vector<float> rotations_x;
    std::vector<float> rotations_y;
    std::vector<float> rotations_z;
    std::vector<float> rotations_w;
    std::vector<float> positions_x;
    std::vector<float> positions_y;
    std::vector<float> positions_z;
    BytesToSkeletonDefinition(
                              bytes_bone_definition,
                              joint_ids,
                              joint_ids_parent,
                              rotations_x,
                              rotations_y,
                              rotations_z,
                              rotations_w,
                              positions_x,
                              positions_y,
                              positions_z
                              );
    
    mmf = ConvertSkeletonDefinitionToMmf(
                                         sender_ip,
                                         sender_port,
                                         (int)joint_ids.size(),
                                         joint_ids.data(),
                                         joint_ids_parent.data(),
                                         rotations_x.data(),
                                         rotations_y.data(),
                                         rotations_z.data(),
                                         rotations_w.data(),
                                         positions_x.data(),
                                         positions_y.data(),
                                         positions_z.data()
                                         );
  }else if(box_index_frame_data){
    std::vector<std::byte> bytes_frame_data = Slice(
                                                    bytes,
                                                    box_index_frame_data,
                                                    box_size_frame_data
                                                    );
    int frame_number = 0;
    float timestamp = 0;
    double utc_time = 0;
    uint8_t timecode_hour = 0;
    uint8_t timecode_min = 0;
    uint8_t timecode_sec = 0;
    uint8_t timecode_frame = 0;
    uint8_t timecode_frame_rate = 0;
    bool    timecode_drop_frame = false;
    std::vector<int> joint_ids;
    std::vector<float> rotations_x;
    std::vector<float> rotations_y;
    std::vector<float> rotations_z;
    std::vector<float> rotations_w;
    std::vector<float> positions_x;
    std::vector<float> positions_y;
    std::vector<float> positions_z;
    BytesToFrameData(
                     bytes_frame_data,
                     frame_number,
                     timestamp,
                     utc_time,
                     timecode_hour,
                     timecode_min,
                     timecode_sec,
                     timecode_frame,
                     timecode_frame_rate,
                     timecode_drop_frame,
                     joint_ids,
                     rotations_x,
                     rotations_y,
                     rotations_z,
                     rotations_w,
                     positions_x,
                     positions_y,
                     positions_z
                     );
    
    mmf = ConvertFrameDataToMmf(
                                sender_ip,
                                sender_port,
                                frame_number,
                                timestamp,
                                utc_time,
                                timecode_hour,
                                timecode_min,
                                timecode_sec,
                                timecode_frame,
                                timecode_drop_frame,
                                timecode_drop_frame,
                                (int)joint_ids.size(),
                                joint_ids.data(),
                                rotations_x.data(),
                                rotations_y.data(),
                                rotations_z.data(),
                                rotations_w.data(),
                                positions_x.data(),
                                positions_y.data(),
                                positions_z.data()
                                );
  }
  
  return mmf;
}

bool MocopiMotionFormatUtility::ConvertBytesToSkeletonDefinition(
                                                               std::vector<std::byte> bytes,
                                                               std::string &sender_ip,
                                                               int &sender_port,
                                                               int &data_size,
                                                               int *&joint_ids,
                                                               int *&parent_joint_ids,
                                                               float *&rotations_x,
                                                               float *&rotations_y,
                                                               float *&rotations_z,
                                                               float *&rotations_w,
                                                               float *&positions_x,
                                                               float *&positions_y,
                                                               float *&positions_z
                                                               )
{
  std::vector<std::byte> bytes_skeleton_definition;
  if(!AnalysisBytes(
                    bytes, sender_ip, sender_port,
                    BoxType::SkeletonDefinition, bytes_skeleton_definition
                    )
  ){
    return false;
  }
  
  std::vector<int> vec_joint_ids;
  std::vector<int> vec_joint_ids_parent;
  std::vector<float> vec_rotations_x;
  std::vector<float> vec_rotations_y;
  std::vector<float> vec_rotations_z;
  std::vector<float> vec_rotations_w;
  std::vector<float> vec_positions_x;
  std::vector<float> vec_positions_y;
  std::vector<float> vec_positions_z;
  BytesToSkeletonDefinition(
                            bytes_skeleton_definition,
                            vec_joint_ids,
                            vec_joint_ids_parent,
                            vec_rotations_x,
                            vec_rotations_y,
                            vec_rotations_z,
                            vec_rotations_w,
                            vec_positions_x,
                            vec_positions_y,
                            vec_positions_z
                            );
  data_size = (int)vec_joint_ids.size();
  
  // Must use new instead of .data() to allocate memory, otherwise it will be freed before use at the return destination
  joint_ids = new int[data_size];
  parent_joint_ids = new int[data_size];
  rotations_x = new float[data_size];
  rotations_y = new float[data_size];
  rotations_z = new float[data_size];
  rotations_w = new float[data_size];
  positions_x = new float[data_size];
  positions_y = new float[data_size];
  positions_z = new float[data_size];
  for(int i=0; i<data_size; i++){
    joint_ids[i] = vec_joint_ids[i];
    parent_joint_ids[i] = vec_joint_ids_parent[i];
    rotations_x[i] = vec_rotations_x[i];
    rotations_y[i] = vec_rotations_y[i];
    rotations_z[i] = vec_rotations_z[i];
    rotations_w[i] = vec_rotations_w[i];
    positions_x[i] = vec_positions_x[i];
    positions_y[i] = vec_positions_y[i];
    positions_z[i] = vec_positions_z[i];
  }
  
  return true;
}

bool MocopiMotionFormatUtility::ConvertBytesToFrameData(
                                                      std::vector<std::byte> bytes,
                                                      std::string &sender_ip,
                                                      int &sender_port,
                                                      int &frame_number,
                                                      float &timestamp,
                                                      double &utc_time,
                                                      uint8_t &timecode_hour,
                                                      uint8_t &timecode_min,
                                                      uint8_t &timecode_sec,
                                                      uint8_t &timecode_frame,
                                                      uint8_t &timecode_frame_rate,
                                                      bool    &timecode_drop_frame,
                                                      int &data_size,
                                                      int *&joint_ids,
                                                      float *&rotations_x,
                                                      float *&rotations_y,
                                                      float *&rotations_z,
                                                      float *&rotations_w,
                                                      float *&positions_x,
                                                      float *&positions_y,
                                                      float *&positions_z
                                                      )
{
  std::vector<std::byte> bytes_frame_data;
  if(!AnalysisBytes(
                    bytes, sender_ip, sender_port,
                    BoxType::FrameData, bytes_frame_data
                    )
     ){
    return false;
  }

  frame_number = 0;
  timestamp = 0;
  utc_time = 0;
  timecode_hour = 0;
  timecode_min = 0;
  timecode_sec = 0;
  timecode_frame = 0;
  timecode_frame_rate = 0;
  timecode_drop_frame = false;
  std::vector<int> vec_joint_ids;
  std::vector<float> vec_rotations_x;
  std::vector<float> vec_rotations_y;
  std::vector<float> vec_rotations_z;
  std::vector<float> vec_rotations_w;
  std::vector<float> vec_positions_x;
  std::vector<float> vec_positions_y;
  std::vector<float> vec_positions_z;
  BytesToFrameData(
                   bytes_frame_data,
                   frame_number,
                   timestamp,
                   utc_time,
                   timecode_hour,
                   timecode_min,
                   timecode_sec,
                   timecode_frame,
                   timecode_frame_rate,
                   timecode_drop_frame,
                   vec_joint_ids,
                   vec_rotations_x,
                   vec_rotations_y,
                   vec_rotations_z,
                   vec_rotations_w,
                   vec_positions_x,
                   vec_positions_y,
                   vec_positions_z
                   );
  data_size = (int)vec_joint_ids.size();
  
  // Must use new instead of .data() to allocate memory, otherwise it will be freed before use at the return destination
  joint_ids = new int[data_size];
  rotations_x = new float[data_size];
  rotations_y = new float[data_size];
  rotations_z = new float[data_size];
  rotations_w = new float[data_size];
  positions_x = new float[data_size];
  positions_y = new float[data_size];
  positions_z = new float[data_size];
  for(int i=0; i<data_size; i++){
    joint_ids[i] = vec_joint_ids[i];
    rotations_x[i] = vec_rotations_x[i];
    rotations_y[i] = vec_rotations_y[i];
    rotations_z[i] = vec_rotations_z[i];
    rotations_w[i] = vec_rotations_w[i];
    positions_x[i] = vec_positions_x[i];
    positions_y[i] = vec_positions_y[i];
    positions_z[i] = vec_positions_z[i];
  }
  return true;
}

bool MocopiMotionFormatUtility::ConvertBytesToSensorData(
                                                       std::vector<std::byte> bytes,
                                                       std::string &sender_ip,
                                                       int &sender_port,
                                                       double &utc_time,
                                                       std::string &sensor_address,
                                                       std::string &sensor_name,
                                                       int &sensor_joint_id,
                                                       float &rotation_x,
                                                       float &rotation_y,
                                                       float &rotation_z,
                                                       float &rotation_w,
                                                       float &velocity_x,
                                                       float &velocity_y,
                                                       float &velocity_z
                                                       )
{
  std::vector<std::byte> bytes_sensor_data;
  if(!AnalysisBytes(
                    bytes, sender_ip, sender_port,
                    BoxType::SensorData, bytes_sensor_data
                    )
     ){
    return false;
  }
  
  utc_time = 0;
  sensor_address = "";
  sensor_name = "";
  
  BytesToSensorData(
                   bytes_sensor_data,
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
  
  return true;
}

std::vector<std::byte> MocopiMotionFormatUtility::BoxesToBytes(std::vector<Box*> boxes){
  std::vector<std::byte> bytes;
  
  for(int i=0; i<(int)boxes.size(); i++){
    std::vector<std::byte> tmp_bytes = BoxToBytes(boxes[i]);
    for(int i=0; i<tmp_bytes.size(); i++){
      bytes.push_back(tmp_bytes[i]);
    }
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::BoxToBytes(Box* box){
  std::vector<std::byte> bytes;
  
  std::vector<std::byte> bytes_size = GetBytes(box->data_size);
  std::vector<std::byte> bytes_type = GetBytes(box->box_type_4cc);
  std::vector<std::byte> bytes_data = GetBytes(box);
  
  int size_size = (int)bytes_size.size(); // 4byte
  int size_type = (int)bytes_type.size(); // 4byte
  int size_data = (int)bytes_data.size(); // Depends on data content
  
  for(int i=0; i<size_size; i++){
    bytes.push_back(bytes_size[i]);
  }
  
  for(int i=0; i<size_type; i++){
    bytes.push_back(bytes_type[i]);
  }
  
  for(int i=0; i<size_data; i++){
    bytes.push_back(bytes_data[i]);
  }

  return bytes;
}

Box* MocopiMotionFormatUtility::GetBox(BoxType box_type, Box* box){
  if(box == nullptr){
    return nullptr;
  }
  
  if(box->box_type == box_type){
    return box;
  }
  
  for(int i=0; i<(int)box->children.size(); i++){
    Box* b = GetBox(box_type, box->children[i]);
    if(b != nullptr){
      return b;
    }
  }
  
  return nullptr;
}

bool MocopiMotionFormatUtility::IsSupportedVersion(std::byte version){
  for(std::byte v : Const::SUPPORTED_VERSION){
    if(v == version){
      return true;
    }
  }
  return false;
}

bool MocopiMotionFormatUtility::IsMmfBytes(std::vector<std::byte> bytes){
  int box_index_header = -1;
  int box_size_header = 0;
  
  return SearchBoxIndex(
                 bytes,
                 BoxType::Head,
                 box_index_header,
                 box_size_header
                 );
}

bool MocopiMotionFormatUtility::IsSkeletonDefinitionBytes(std::vector<std::byte> bytes){
  int box_index_skeleton_definition = -1;
  int box_size_skeleton_definition = 0;
  
  return SearchBoxIndex(
                 bytes,
                 BoxType::SkeletonDefinition,
                 box_index_skeleton_definition,
                 box_size_skeleton_definition
                 );
}

bool MocopiMotionFormatUtility::IsFrameDataBytes(std::vector<std::byte> bytes){
  int box_index_frame_data = -1;
  int box_size_frame_data = 0;
  
  return SearchBoxIndex(
                 bytes,
                 BoxType::FrameData,
                 box_index_frame_data,
                 box_size_frame_data
                 );
}

bool MocopiMotionFormatUtility::IsSensorDataBytes(std::vector<std::byte> bytes){
  int box_index_sensor_data = -1;
  int box_size_sensor_data = 0;
  
  return SearchBoxIndex(
                        bytes,
                        BoxType::SensorData,
                        box_index_sensor_data,
                        box_size_sensor_data
                        );
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(int i){
  unsigned char* data = reinterpret_cast<unsigned char*>(&i);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(int); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(float f){
  unsigned char* data = reinterpret_cast<unsigned char*>(&f);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(float); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(double d){
  unsigned char* data = reinterpret_cast<unsigned char*>(&d);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(double); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(unsigned long long u){
  unsigned char* data = reinterpret_cast<unsigned char*>(&u);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(unsigned long long); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(unsigned short u){
  unsigned char* data = reinterpret_cast<unsigned char*>(&u);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(unsigned short); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(short s){
  unsigned char* data = reinterpret_cast<unsigned char*>(&s);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(short); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(uint8_t i){
  unsigned char* data = reinterpret_cast<unsigned char*>(&i);
  
  std::vector<std::byte> bytes;
  for(int i=0; i<sizeof(uint8_t); i++){
    bytes.push_back(std::byte(data[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(std::string str){
  const char* chars = str.c_str();
  
  std::vector<std::byte> bytes;
  for(int i=0; i<str.length(); i++){
    bytes.push_back(std::byte(chars[i]));
  }
  
  return bytes;
}

std::vector<std::byte> MocopiMotionFormatUtility::GetBytes(Box* box){
  std::vector<std::byte> bytes;

  if(box->data.type() == typeid(Box*)){
    std::vector<std::byte> tmp_bytes = BoxesToBytes(box->children);
    AddBytes(tmp_bytes, &bytes);
  }else if(box->data.type() == typeid(Box**)){
    std::vector<std::byte> tmp_bytes = BoxesToBytes(box->children);
    AddBytes(tmp_bytes, &bytes);
  }else if(box->data.type() == typeid(std::byte)){
    bytes.push_back(std::any_cast<std::byte>(box->data));
  }else if(box->data.type() == typeid(std::byte*)){
    std::byte* tmp_bytes = std::any_cast<std::byte*>(box->data);
    AddBytes(tmp_bytes, box->data_size, &bytes);
  }else if(box->data.type() == typeid(float)){
    bytes = GetBytes(std::any_cast<float>(box->data));
  }else if(box->data.type() == typeid(float*)){
    float* float_array = std::any_cast<float*>(box->data);
    for(int i=0; i<box->data_num; i++){
      std::vector<std::byte> tmp_bytes = GetBytes(float_array[i]);
      for(std::byte b : tmp_bytes){
        bytes.push_back(b);
      }
    }
  }else if(box->data.type() == typeid(double)){
    bytes = GetBytes(std::any_cast<double>(box->data));
  }else if(box->data.type() == typeid(int)){
    bytes = GetBytes(std::any_cast<int>(box->data));
  }else if(box->data.type() == typeid(unsigned long long)){
    bytes = GetBytes(std::any_cast<unsigned long long>(box->data));
  }else if(box->data.type() == typeid(unsigned short)){
    bytes = GetBytes(std::any_cast<unsigned short>(box->data));
  }else if(box->data.type() == typeid(short)){
    bytes = GetBytes(std::any_cast<short>(box->data));
  }else if(box->data.type() == typeid(uint8_t*)){
    uint8_t* uint8_t_array = std::any_cast<uint8_t*>(box->data);
    for(int i=0; i<box->data_num; i++){
      std::vector<std::byte> tmp_bytes = GetBytes(uint8_t_array[i]);
      for(std::byte b : tmp_bytes){
        bytes.push_back(b);
      }
    }
  }else if(box->data.type() == typeid(std::string)){
    std::string str = std::any_cast<std::string>(box->data);
    bytes = GetBytes(str);
  }
  
  return bytes;
}

int MocopiMotionFormatUtility::GetInt(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(int)){
    return 0;
  }
  
  unsigned char data[sizeof(int)];
  for(int i=0; i<sizeof(int); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<int*>(data);
}

float MocopiMotionFormatUtility::GetFloat(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(float)){
    return 0;
  }
  
  unsigned char data[sizeof(float)];
  for(int i=0; i<sizeof(float); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<float*>(data);
}

double MocopiMotionFormatUtility::GetDouble(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(double)){
    return 0;
  }
  
  unsigned char data[sizeof(double)];
  for(int i=0; i<sizeof(double); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<double*>(data);
}

long MocopiMotionFormatUtility::GetLong(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(long)){
    return 0;
  }
  
  unsigned char data[sizeof(long)];
  for(int i=0; i<sizeof(long); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<long*>(data);
}

unsigned short MocopiMotionFormatUtility::GetUShort(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(unsigned short)){
    return 0;
  }
  
  unsigned char data[sizeof(unsigned short)];
  for(int i=0; i<sizeof(unsigned short); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<unsigned short*>(data);
}

short MocopiMotionFormatUtility::GetShort(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(short)){
    return 0;
  }
  
  unsigned char data[sizeof(short)];
  for(int i=0; i<sizeof(short); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<short*>(data);
}

unsigned long long MocopiMotionFormatUtility::GetULong(std::vector<std::byte>& bytes, int offset){
  if(bytes.size() < offset + sizeof(unsigned long long)){
    return 0;
  }
  
  unsigned char data[sizeof(unsigned long long)];
  for(int i=0; i<sizeof(unsigned long long); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<unsigned long long*>(data);
}

std::string MocopiMotionFormatUtility::GetString(std::vector<std::byte>& bytes, int offset, int length){
  if(bytes.size() < offset + length){
    return "";
  }

  std::string str = "";
  
  for(int i=0; i<length; i++){
    str += (char)bytes[offset + i];
  }
  
  return str;
}

uint8_t MocopiMotionFormatUtility::GetUInt8T(std::vector<std::byte> &bytes, int offset){
  if(bytes.size() < offset + sizeof(uint8_t)){
    return 0;
  }
  
  unsigned char data[sizeof(uint8_t)];
  for(int i=0; i<sizeof(uint8_t); i++){
    data[i] = (unsigned char)bytes[offset + i];
  }
  
  return *reinterpret_cast<uint8_t*>(data);
}

void MocopiMotionFormatUtility::AddBytes(std::byte *src, int size, std::vector<std::byte>* dst){
  for(int i=0; i<size; i++){
    dst->push_back(src[i]);
  }
}

void MocopiMotionFormatUtility::AddBytes(std::vector<std::byte> src, std::vector<std::byte> *dst){
  for(std::byte b : src){
    dst->push_back(b);
  }
}

std::vector<std::byte> MocopiMotionFormatUtility::Slice(
                                                      std::vector<std::byte> src,
                                                      int offset,
                                                      int read_count
                                                      )
{
  std::vector<std::byte> dst;
  
  for(int i=0; i<read_count; i++){
    if(offset + i >= (int)src.size()){
      Logger::LogError("Invalid index : " + std::to_string(offset + i));
      break;
    }
    
    dst.push_back(src[offset + i]);
  }
  
  return dst;
}

bool MocopiMotionFormatUtility::AnalysisBytes(
                                            std::vector<std::byte> bytes,
                                            std::string &sender_ip,
                                            int &sender_port,
                                            BoxType target_box_type,
                                            std::vector<std::byte> &target_box_data
                                            )
{
  int box_index_header = -1;
  int box_size_header = 0;
  SearchBoxIndex(
                 bytes,
                 BoxType::Head,
                 box_index_header,
                 box_size_header
                 );
  if(box_index_header < 0){
    Logger::LogError("Invalid header data.");
    return false;
  }
  
  int box_index_sender_definition = -1;
  int box_size_sender_definition = 0;
  SearchBoxIndex(
                 bytes,
                 BoxType::SenderDefinition,
                 box_index_sender_definition,
                 box_size_sender_definition
                 );
  if(box_index_sender_definition < 0){
    Logger::LogError("Invalid sender definition data.");
    return false;
  }
  
  int box_index_data = -1;
  int box_size_data = 0;
  SearchBoxIndex(
                 bytes,
                 target_box_type,
                 box_index_data,
                 box_size_data
                 );
  if(box_index_data < 0){
    Logger::LogError("Invalid value data.");
    return false;
  }
  
  
  std::vector<std::byte> bytes_header = Slice(
                                              bytes,
                                              box_index_header,
                                              box_size_header
                                              );
  std::string format_type = "";
  std::byte version = std::byte(1);
  BytesToHeader(
                bytes_header,
                format_type,
                version
                );
  
  // version check
  if(!IsSupportedVersion(version)){
    Logger::LogError(
                     "Invalid data version. Current version : " + std::to_string((int)Const::FORMAT_VERSION) +
                     ", Target version : " + std::to_string((int)version) + "."
                     );
    return false;
  }
  
  std::vector<std::byte> bytes_sender_definition = Slice(
                                                         bytes,
                                                         box_index_sender_definition,
                                                         box_size_sender_definition
                                                         );
  
  BytesToSenderDefinition(
                          bytes_sender_definition,
                          sender_ip,
                          sender_port
                          );
  
  target_box_data = Slice(
                          bytes,
                          box_index_data,
                          box_size_data
                          );
  
  return true;
}

bool MocopiMotionFormatUtility::SearchBoxIndex(
                                            std::vector<std::byte> bytes,
                                            BoxType target_box_type,
                                            int& start_index,
                                            int& box_size,
                                            int offset,
                                            int read_count
                                            )
{
  int read_end_index = read_count >=0? offset + read_count : (int)bytes.size();
  
  start_index = -1;
  box_size = 0;
  
  while(offset < read_end_index){
    if((int)bytes.size() < offset + Const::BOX_HEADER_SIZE){
      Logger::LogError("Size over");
      return false;
    }
    
    int size = -1;
    BoxType box_type = BoxType::None;
    
    CheckSizeAndBoxType(bytes, offset, size, box_type);
    
    if(target_box_type == box_type){
      start_index = offset;
      box_size = size + Const::BOX_HEADER_SIZE; // Add 8 bytes for size and 4cc
      return true;
    }
    
    offset += Const::BOX_HEADER_SIZE; // Advance by 8 bytes that were read
    
    // Data loading
    if(size > 0 && box_type != BoxType::None){
      bool is_valid_data;
      const std::type_info& type = GetDataTypeFromBoxType(box_type, is_valid_data);

      if(is_valid_data){
        if(type == typeid(Box)){
          if(SearchBoxIndex(bytes, target_box_type, start_index, box_size, offset, size)){
            return true;
          }
        }else if(type == typeid(Box*)){
          if(SearchBoxIndex(bytes, target_box_type, start_index, box_size, offset, size)){
            return true;
          }
        }

        offset += size;
      }else{
        offset += size;
      }
    }else{
      return false;
    }
  }
  
  return false;
}

void MocopiMotionFormatUtility::BytesToHeader(
                                            std::vector<std::byte> bytes,
                                            std::string &format_type,
                                            std::byte &version,
                                            int offset,
                                            int read_end
                                            ){
  if(read_end < 0){
    read_end = (int)bytes.size();
  }
  
  while(offset + Const::BOX_HEADER_SIZE < read_end){
    int size = 0;
    BoxType box_type;
    
    CheckSizeAndBoxType(bytes, offset, size, box_type);
    offset += Const::BOX_HEADER_SIZE; // Advance by 8 bytes that were read
        
    if(box_type == BoxType::FormatType){
      format_type = GetString(bytes, offset, size);
    }else if(box_type == BoxType::Version){
      version = bytes[offset];
    }else{
      bool is_valid_data;
      const std::type_info& type = GetDataTypeFromBoxType(box_type, is_valid_data);
      if(is_valid_data){
        if(type == typeid(Box*)){
          BytesToHeader(
                        bytes,
                        format_type,
                        version,
                        offset,
                        offset + size
                        );
        }
      }
    }
    
    offset += size;
  }
}

void MocopiMotionFormatUtility::BytesToSenderDefinition(
                                                      std::vector<std::byte> bytes,
                                                      std::string &sender_ip,
                                                      int &sender_port,
                                                      int offset,
                                                      int read_end
                                                      )
{
  if(read_end < 0){
    read_end = (int)bytes.size();
  }
  
  while(offset + Const::BOX_HEADER_SIZE < read_end){
    int size;
    BoxType box_type;
    
    CheckSizeAndBoxType(bytes, offset, size, box_type);
    offset += Const::BOX_HEADER_SIZE; // Advance by 8 bytes that were read
    
    if(box_type == BoxType::IpAddress){
      unsigned long long ip_long = GetULong(bytes, offset);
      sender_ip = Box::ULongToIpAddress(ip_long);
    }else if(box_type == BoxType::RecievingPort){
      sender_port = (int)GetUShort(bytes, offset);
    }else{
      bool is_valid_data;
      const std::type_info& type = GetDataTypeFromBoxType(box_type, is_valid_data);
      if(is_valid_data){
        if(type == typeid(Box*)){
          BytesToSenderDefinition(
                                  bytes,
                                  sender_ip,
                                  sender_port,
                                  offset,
                                  offset + size
                                  );
        }
      }
    }
    
    offset += size;
  }
}

void MocopiMotionFormatUtility::BytesToSkeletonDefinition(
                                                        std::vector<std::byte> bytes,
                                                        std::vector<int> &joint_ids,
                                                        std::vector<int> &parent_joint_ids,
                                                        std::vector<float> &rotations_x,
                                                        std::vector<float> &rotations_y,
                                                        std::vector<float> &rotations_z,
                                                        std::vector<float> &rotations_w,
                                                        std::vector<float> &positions_x,
                                                        std::vector<float> &positions_y,
                                                        std::vector<float> &positions_z,
                                                        int offset,
                                                        int read_end
                                                        )
{
  if(read_end < 0){
    read_end = (int)bytes.size();
  }
  
  while(offset + Const::BOX_HEADER_SIZE < read_end){
    int size;
    BoxType box_type;
    
    CheckSizeAndBoxType(bytes, offset, size, box_type);
    offset += Const::BOX_HEADER_SIZE; // Advance by 8 bytes that were read
    
    if(box_type == BoxType::BoneId){
      // Communication uses short, but function interface uses int
      joint_ids.push_back((int)GetShort(bytes, offset));
    }else if(box_type == BoxType::ParentBoneId){
      // Communication uses short, but function interface uses int
      parent_joint_ids.push_back((int)GetShort(bytes, offset));
    }else if(box_type == BoxType::TransformData){
      // Communication and function interface both use float
      // Convert 4 * 7 bytes
      rotations_x.push_back(GetFloat(bytes, offset));
      rotations_y.push_back(GetFloat(bytes, offset + sizeof(float)));
      rotations_z.push_back(GetFloat(bytes, offset + sizeof(float) * 2));
      rotations_w.push_back(GetFloat(bytes, offset + sizeof(float) * 3));
      positions_x.push_back(GetFloat(bytes, offset + sizeof(float) * 4));
      positions_y.push_back(GetFloat(bytes, offset + sizeof(float) * 5));
      positions_z.push_back(GetFloat(bytes, offset + sizeof(float) * 6));
    }else{
      bool is_valid_data;
      const std::type_info& type = GetDataTypeFromBoxType(box_type, is_valid_data);
      if(is_valid_data){
        if(type == typeid(Box*)){
          BytesToSkeletonDefinition(
                                    bytes,
                                    joint_ids,
                                    parent_joint_ids,
                                    rotations_x,
                                    rotations_y,
                                    rotations_z,
                                    rotations_w,
                                    positions_x,
                                    positions_y,
                                    positions_z,
                                    offset,
                                    offset + size
                                    );
        }
      }
    }
    
    offset += size;
  }
}

void MocopiMotionFormatUtility::BytesToFrameData(
                                               std::vector<std::byte> bytes,
                                               int& frame_number,
                                               float& timestamp,
                                               double& utc_time,
                                               uint8_t& timecode_hour,
                                               uint8_t& timecode_min,
                                               uint8_t& timecode_sec,
                                               uint8_t& timecode_frame,
                                               uint8_t& timecode_frame_rate,
                                               bool&    timecode_drop_frame,
                                               std::vector<int> &joint_ids,
                                               std::vector<float> &rotations_x,
                                               std::vector<float> &rotations_y,
                                               std::vector<float> &rotations_z,
                                               std::vector<float> &rotations_w,
                                               std::vector<float> &positions_x,
                                               std::vector<float> &positions_y,
                                               std::vector<float> &positions_z,
                                               int offset,
                                               int read_end
                                               )
{
  if(read_end < 0){
    read_end = (int)bytes.size();
  }
  
  while(offset + Const::BOX_HEADER_SIZE < read_end){
    int size;
    BoxType box_type;
    
    CheckSizeAndBoxType(bytes, offset, size, box_type);
    offset += Const::BOX_HEADER_SIZE; // Advance by 8 bytes that were read
    
    if(box_type == BoxType::BoneId){
      // Communication uses short, but function interface uses int
      joint_ids.push_back((int)GetShort(bytes, offset));
    }else if(box_type == BoxType::FrameNumber){
      frame_number = GetInt(bytes, offset);
    }else if(box_type == BoxType::Timestamp){
      timestamp = GetFloat(bytes, offset);
    }else if(box_type == BoxType::UtcTime){
      utc_time = GetDouble(bytes, offset);
    }else if(box_type == BoxType::Timecode){
      timecode_hour       = GetUInt8T(bytes, offset);
      timecode_min        = GetUInt8T(bytes, offset + sizeof(uint8_t));
      timecode_sec        = GetUInt8T(bytes, offset + sizeof(uint8_t) * 2);
      timecode_frame      = GetUInt8T(bytes, offset + sizeof(uint8_t) * 3);
      timecode_frame_rate = GetUInt8T(bytes, offset + sizeof(uint8_t) * 4);
      timecode_drop_frame = GetUInt8T(bytes, offset + sizeof(uint8_t) * 5) > 0;
    }else if(box_type == BoxType::TransformData){
      // Convert 4 * 7 bytes
      rotations_x.push_back(GetFloat(bytes, offset));
      rotations_y.push_back(GetFloat(bytes, offset + sizeof(float)));
      rotations_z.push_back(GetFloat(bytes, offset + sizeof(float) * 2));
      rotations_w.push_back(GetFloat(bytes, offset + sizeof(float) * 3));
      positions_x.push_back(GetFloat(bytes, offset + sizeof(float) * 4));
      positions_y.push_back(GetFloat(bytes, offset + sizeof(float) * 5));
      positions_z.push_back(GetFloat(bytes, offset + sizeof(float) * 6));
    }else{
      bool is_valid_data;
      const std::type_info& type = GetDataTypeFromBoxType(box_type, is_valid_data);
      if(is_valid_data){
        if(type == typeid(Box*)){
          BytesToFrameData(
                           bytes,
                           frame_number,
                           timestamp,
                           utc_time,
                           timecode_hour,
                           timecode_min,
                           timecode_sec,
                           timecode_frame,
                           timecode_frame_rate,
                           timecode_drop_frame,
                           joint_ids,
                           rotations_x,
                           rotations_y,
                           rotations_z,
                           rotations_w,
                           positions_x,
                           positions_y,
                           positions_z,
                           offset,
                           offset + size
                           );
        }
      }
    }
    
    offset += size;
  }
}

void MocopiMotionFormatUtility::BytesToSensorData(
                                                std::vector<std::byte> bytes,
                                                double& utc_time,
                                                std::string& sensor_address,
                                                std::string& sensor_name,
                                                int& sensor_joint_id,
                                                float& rotation_x,
                                                float& rotation_y,
                                                float& rotation_z,
                                                float& rotation_w,
                                                float& velocity_x,
                                                float& velocity_y,
                                                float& velocity_z,
                                                int offset,
                                                int read_end
                                               )
{
  if(read_end < 0){
    read_end = (int)bytes.size();
  }
  
  while(offset + Const::BOX_HEADER_SIZE < read_end){
    int size;
    BoxType box_type;
    
    CheckSizeAndBoxType(bytes, offset, size, box_type);
    offset += Const::BOX_HEADER_SIZE; // Advance by 8 bytes that were read
    
    if(box_type == BoxType::UtcTime){
      utc_time = GetDouble(bytes, offset);
    }else if(box_type == BoxType::SensorAddress){
      // Communication uses char*, but function interface uses std::string
      sensor_address = GetString(bytes, offset, size);
    }else if(box_type == BoxType::SensorName){
      // Communication uses char*, but function interface uses std::string
      sensor_name = GetString(bytes, offset, size);
    }else if(box_type == BoxType::SensorJointId){
      sensor_joint_id = GetInt(bytes, offset);
    }else if(box_type == BoxType::SensorValue){
      // NOTE: Note that this is not BoxType::TransformData
      
      // Convert 4 * 7 bytes
      rotation_x = GetFloat(bytes, offset);
      rotation_y = GetFloat(bytes, offset + sizeof(float));
      rotation_z = GetFloat(bytes, offset + sizeof(float) * 2);
      rotation_w = GetFloat(bytes, offset + sizeof(float) * 3);
      velocity_x = GetFloat(bytes, offset + sizeof(float) * 4);
      velocity_y = GetFloat(bytes, offset + sizeof(float) * 5);
      velocity_z = GetFloat(bytes, offset + sizeof(float) * 6);
    }else{
      bool is_valid_data;
      const std::type_info& type = GetDataTypeFromBoxType(box_type, is_valid_data);
      if(is_valid_data){
        if(type == typeid(Box*)){
          BytesToSensorData(
                            bytes,
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
                            offset,
                            offset + size
                            );
        }
      }
    }
    
    offset += size;
  }
}

void MocopiMotionFormatUtility::CheckSizeAndBoxType(
                                                  std::vector<std::byte> bytes,
                                                  int offset,
                                                  int& size,
                                                  BoxType& box_type
                                                  ){
  // Read size (4 bytes as int)
  // (Read size - 4 bytes as it's an int)
  size = GetInt(bytes, offset);
  offset += sizeof(int);
  
  // Read 4cc (4 bytes of ASCII) //
  std::string type_4cc = GetString(bytes, offset, Const::BOX_4CC_SIZE);
  box_type = Const::BoxType4CC::StringToBoxType(type_4cc);
}

}

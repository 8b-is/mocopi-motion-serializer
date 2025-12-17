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

//
// NOTE: 変換処理は mocopi_motion_format_api 経由で呼び出すこと
// (NOTE: Conversion processes should be called through mocopi_motion_format_api)
// MocopiMotionFormatUtility を直接呼び出すとメモリリークが発生する可能性があります
// (Directly calling MocopiMotionFormatUtility may cause memory leaks)
//

#ifndef mocopi_motion_format_utility_hpp
#define mocopi_motion_format_utility_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <typeinfo>
#include "./mocopi_motion_format.hpp"
#include "./box.hpp"
#include "./const.hpp"
#include "./logger.hpp"

#if __APPLE__
#import <TargetConditionals.h>
#if TARGET_OS_MAC
#include <sys/types.h>
#endif
#endif

namespace mocopi_motion_format {

class MocopiMotionFormatUtility{
  // NOTE: Return value pointers (especially arrays) must be freed by the caller
  //       Forgetting to free them will cause memory leaks
  
  // NOTE: Currently no version branching. Compatibility checks will be added for future feature extensions
public:
  static const std::type_info& GetDataTypeFromBoxType(BoxType box_type, bool& is_success);
  static std::vector<std::byte> ConvertSkeletonDefinitionToBytes(
                                                                 std::string sender_ip,
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
                                                                 float* positions_z
                                                                 );
  static MocopiMotionFormat* ConvertSkeletonDefinitionToMmf(
                                                          std::string sender_ip,
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
                                                          float* positions_z
                                                          );
  static std::vector<std::byte> ConvertFrameDataToBytes(
                                                        std::string sender_ip,
                                                        int sender_port,
                                                        int frame_number,
                                                        float timestamp,
                                                        double utc_time,
                                                        uint8_t timecode_hour,
                                                        uint8_t timecode_min,
                                                        uint8_t timecode_sec,
                                                        uint8_t timedode_frame,
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
                                                        float* positions_z
                                                        );
  static MocopiMotionFormat* ConvertFrameDataToMmf(
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
                                                 int* joint_ids,
                                                 float* rotations_x,
                                                 float* rotations_y,
                                                 float* rotations_z,
                                                 float* rotations_w,
                                                 float* positions_x,
                                                 float* positions_y,
                                                 float* positions_z
                                                 );
  static std::vector<std::byte> ConvertSensorDataToBytes(
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
                                                         );
  static MocopiMotionFormat* ConvertSensorDataToMmf(
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
                                                 );
  static std::vector<std::byte> ConvertMmfToBytes(MocopiMotionFormat* motion_format);
  [[deprecated("Please use function waht regarding to data type like ConvertSkeletonDefinitionToMmf.")]]
  static MocopiMotionFormat* ConvertBytesToMmf(std::vector<std::byte> bytes);
  static bool ConvertBytesToSkeletonDefinition(
                                               std::vector<std::byte> bytes,
                                               std::string& sender_ip,
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
  static bool ConvertBytesToFrameData(
                                      std::vector<std::byte> bytes,
                                      std::string& sender_ip,
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
  static bool ConvertBytesToSensorData(
                                       std::vector<std::byte> bytes,
                                       std::string& sender_ip,
                                       int& sender_port,
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
                                       float& velocity_z
                                       );
  static std::vector<std::byte> BoxesToBytes(std::vector<Box*> boxes);
  static std::vector<std::byte> BoxToBytes(Box* box);
  static Box* GetBox(BoxType box_type, Box* box);
  static bool IsSupportedVersion(std::byte version);
  static bool IsMmfBytes(std::vector<std::byte> bytes);
  static bool IsSkeletonDefinitionBytes(std::vector<std::byte> bytes);
  static bool IsFrameDataBytes(std::vector<std::byte> bytes);
  static bool IsSensorDataBytes(std::vector<std::byte> bytes);
private:
  static std::vector<std::byte> GetBytes(int i);
  static std::vector<std::byte> GetBytes(float f);
  static std::vector<std::byte> GetBytes(double d);
  static std::vector<std::byte> GetBytes(unsigned long long u);
  static std::vector<std::byte> GetBytes(unsigned short u);
  static std::vector<std::byte> GetBytes(short s);
  static std::vector<std::byte> GetBytes(uint8_t i);
  static std::vector<std::byte> GetBytes(std::string str); // NOTE : 1byte文字しか想定していない //
  static std::vector<std::byte> GetBytes(Box* box);
  
  static int          GetInt(std::vector<std::byte>& bytes, int offset);
  static float        GetFloat(std::vector<std::byte>& bytes, int offset);
  static double       GetDouble(std::vector<std::byte>& bytes, int offset);
  static long         GetLong(std::vector<std::byte>& bytes, int offset);
  static unsigned short      GetUShort(std::vector<std::byte>& bytes, int offset);
  static short        GetShort(std::vector<std::byte>& bytes, int offset);
  static unsigned long long       GetULong(std::vector<std::byte>& bytes, int offset);
  static std::string  GetString(std::vector<std::byte>& bytes, int offset, int length); // NOTE : 1byte文字しか想定していない
  static uint8_t      GetUInt8T(std::vector<std::byte>& bytes, int offset);
  
  static void AddBytes(std::byte* src, int size, std::vector<std::byte>* dst);
  static void AddBytes(std::vector<std::byte> src, std::vector<std::byte>* dst);
  static std::vector<std::byte> Slice(std::vector<std::byte> src, int offset, int read_count);
  static bool AnalysisBytes(
                                 std::vector<std::byte> bytes,
                                 std::string &sender_ip,
                                 int &sender_port,
                                 BoxType target_box_type,
                                 std::vector<std::byte> &target_box_data
                                 );
  static bool SearchBoxIndex(
                             std::vector<std::byte> bytes,
                             BoxType target_box_type,
                             int& start_index,
                             int& box_size,
                             int offset = 0,
                             int read_count = -1
                             );
  static void BytesToHeader(
                            std::vector<std::byte> bytes,
                            std::string& format_type,
                            std::byte& version,
                            int offset = 0,
                            int read_end = -1
                            );
  static void BytesToSenderDefinition(
                                      std::vector<std::byte> bytes,
                                      std::string& sender_ip,
                                      int& sender_port,
                                      int offset = 0,
                                      int read_end = -1
                                      );
  static void BytesToSkeletonDefinition(
                                        std::vector<std::byte> bytes,
                                        std::vector<int>& joint_ids,
                                        std::vector<int>& parent_joint_ids,
                                        std::vector<float>& rotations_x,
                                        std::vector<float>& rotations_y,
                                        std::vector<float>& rotations_z,
                                        std::vector<float>& rotations_w,
                                        std::vector<float>& positions_x,
                                        std::vector<float>& positions_y,
                                        std::vector<float>& positions_z,
                                        int offset = 0,
                                        int read_end = -1
                                        );
  static void BytesToFrameData(
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
                               std::vector<int>& joint_ids,
                               std::vector<float>& rotations_x,
                               std::vector<float>& rotations_y,
                               std::vector<float>& rotations_z,
                               std::vector<float>& rotations_w,
                               std::vector<float>& positions_x,
                               std::vector<float>& positions_y,
                               std::vector<float>& positions_z,
                               int offset = 0,
                               int read_end = -1
                               );
  static void BytesToSensorData(
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
                                int offset = 0,
                                int read_end = -1
                                );
  static void CheckSizeAndBoxType(
                                  std::vector<std::byte> bytes,
                                  int offset,
                                  int& size,
                                  BoxType& box_type
                                  );
};

}

#endif /* mocopi_motion_format_utility_hpp */

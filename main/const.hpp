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

#ifndef const_hpp
#define const_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "./enums.hpp"

namespace mocopi_motion_format {

class Const{
public:
  static const std::string  FORMAT_TYPE_NAME;
  static const std::byte    FORMAT_VERSION;
  static const std::string  FORMAT_TYPE_NAME_4CC;
  
  static const std::vector<std::byte> SUPPORTED_VERSION;
  
  // Boxの4ccのデータ容量(4byte)
  static const int BOX_4CC_SIZE;
  
  // Boxのsize, 4ccのデータ容量(8byte)
  static const int BOX_HEADER_SIZE;
  
  class BoxType4CC{
  public:
    static const std::string NONE;
    static const std::string HEAD_BOX;
    static const std::string FORMAT_TYPE_BOX;
    static const std::string VERSION_BOX;
    static const std::string SENDER_DEFINITION_BOX;
    static const std::string IP_ADDRESS_BOX;
    static const std::string RECIEVING_PORT_BOX;
    static const std::string SKELETON_DEFINITION_BOX;
    static const std::string BONE_DATA_ARRAY_BOX;
    static const std::string BONE_DATA_BOX;
    static const std::string BONE_ID_BOX;
    static const std::string PARENT_BONE_ID_BOX;
    static const std::string TRANSFORM_DATA_BOX;
    static const std::string FRAME_DATA_BOX;
    static const std::string FRAME_NUMBER_BOX;
    static const std::string TIMESTAMP_BOX;
    static const std::string UTC_TIME_BOX;
    static const std::string TIMECODE_BOX;
    static const std::string BONE_TRANSFORM_DATA_ARRAY_BOX;
    static const std::string BONE_TRANSFORM_DATA_BOX;
    static const std::string SENSOR_DATA_BOX;
    static const std::string SENSOR_ADDRESS_BOX;
    static const std::string SENSOR_NAME_BOX;
    static const std::string SENSOR_JOINT_ID_BOX;
    static const std::string SENSOR_VALUE_BOX;
    
    static std::string BoxTypeToString(BoxType box_type);
    static BoxType StringToBoxType(std::string box_type_4cc);
    static bool IsBoxDataType(std::string box_type_4cc);
    static bool IsBoxDataType(BoxType box_type);
  };
};

}

#endif /* const_hpp */

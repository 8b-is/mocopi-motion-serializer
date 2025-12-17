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

#include "const.hpp"

namespace mocopi_motion_format {

const std::string Const::FORMAT_TYPE_NAME = "sony motion format";
const std::byte   Const::FORMAT_VERSION = std::byte(1);
const std::string Const::FORMAT_TYPE_NAME_4CC = "smf1";

const std::vector<std::byte> Const::SUPPORTED_VERSION{
  std::byte(1)
};

const int Const::BOX_4CC_SIZE = 4;
const int Const::BOX_HEADER_SIZE = 8;

const std::string Const::BoxType4CC::NONE                           = "none";
const std::string Const::BoxType4CC::HEAD_BOX                       = "head";
const std::string Const::BoxType4CC::FORMAT_TYPE_BOX                = "ftyp";
const std::string Const::BoxType4CC::VERSION_BOX                    = "vrsn";
const std::string Const::BoxType4CC::SENDER_DEFINITION_BOX          = "sndf";
const std::string Const::BoxType4CC::IP_ADDRESS_BOX                 = "ipad";
const std::string Const::BoxType4CC::RECIEVING_PORT_BOX             = "rcvp";
const std::string Const::BoxType4CC::SKELETON_DEFINITION_BOX        = "skdf";
const std::string Const::BoxType4CC::BONE_DATA_ARRAY_BOX            = "bons";
const std::string Const::BoxType4CC::BONE_DATA_BOX                  = "bndt";
const std::string Const::BoxType4CC::BONE_ID_BOX                    = "bnid";
const std::string Const::BoxType4CC::PARENT_BONE_ID_BOX             = "pbid";
const std::string Const::BoxType4CC::TRANSFORM_DATA_BOX             = "tran";
const std::string Const::BoxType4CC::FRAME_DATA_BOX                 = "fram";
const std::string Const::BoxType4CC::FRAME_NUMBER_BOX               = "fnum";
const std::string Const::BoxType4CC::TIMESTAMP_BOX                  = "time";
const std::string Const::BoxType4CC::UTC_TIME_BOX                   = "uttm";
const std::string Const::BoxType4CC::TIMECODE_BOX                   = "tmcd";
const std::string Const::BoxType4CC::BONE_TRANSFORM_DATA_ARRAY_BOX  = "btrs";
const std::string Const::BoxType4CC::BONE_TRANSFORM_DATA_BOX        = "btdt";
const std::string Const::BoxType4CC::SENSOR_DATA_BOX                = "sndt";
const std::string Const::BoxType4CC::SENSOR_ADDRESS_BOX             = "snad";
const std::string Const::BoxType4CC::SENSOR_NAME_BOX                = "snnm";
const std::string Const::BoxType4CC::SENSOR_JOINT_ID_BOX            = "snji";
const std::string Const::BoxType4CC::SENSOR_VALUE_BOX               = "snvl";

std::string Const::BoxType4CC::BoxTypeToString(BoxType box_type){
  switch (box_type) {
    case BoxType::None:
      return NONE;
    case BoxType::Head:
      return HEAD_BOX;
    case BoxType::FormatType:
      return FORMAT_TYPE_BOX;
    case BoxType::Version:
      return VERSION_BOX;
    case BoxType::SenderDefinition:
      return SENDER_DEFINITION_BOX;
    case BoxType::IpAddress:
      return IP_ADDRESS_BOX;
    case BoxType::RecievingPort:
      return RECIEVING_PORT_BOX;
    case BoxType::SkeletonDefinition:
      return SKELETON_DEFINITION_BOX;
    case BoxType::BoneDataArray:
      return BONE_DATA_ARRAY_BOX;
    case BoxType::BoneData:
      return BONE_DATA_BOX;
    case BoxType::BoneId:
      return BONE_ID_BOX;
    case BoxType::ParentBoneId:
      return PARENT_BONE_ID_BOX;
    case BoxType::TransformData:
      return TRANSFORM_DATA_BOX;
    case BoxType::FrameData:
      return FRAME_DATA_BOX;
    case BoxType::FrameNumber:
      return FRAME_NUMBER_BOX;
    case BoxType::Timestamp:
      return TIMESTAMP_BOX;
    case BoxType::UtcTime:
      return UTC_TIME_BOX;
    case BoxType::Timecode:
      return TIMECODE_BOX;
    case BoxType::BoneTransformDataArray:
      return BONE_TRANSFORM_DATA_ARRAY_BOX;
    case BoxType::BoneTransformData:
      return BONE_TRANSFORM_DATA_BOX;
    case BoxType::SensorData:
      return SENSOR_DATA_BOX;
    case BoxType::SensorAddress:
      return SENSOR_ADDRESS_BOX;
    case BoxType::SensorName:
      return SENSOR_NAME_BOX;
    case BoxType::SensorJointId:
      return SENSOR_JOINT_ID_BOX;
    case BoxType::SensorValue:
      return SENSOR_VALUE_BOX;
    default:
      return NONE;
  }
}

BoxType Const::BoxType4CC::StringToBoxType(std::string box_type_4cc){
  if(box_type_4cc == NONE){
    return BoxType::None;
  }else if(box_type_4cc == HEAD_BOX){
    return BoxType::Head;
  }else if(box_type_4cc == FORMAT_TYPE_BOX){
    return BoxType::FormatType;
  }else if(box_type_4cc == VERSION_BOX){
    return BoxType::Version;
  }else if(box_type_4cc == SENDER_DEFINITION_BOX){
    return BoxType::SenderDefinition;
  }else if(box_type_4cc == IP_ADDRESS_BOX){
    return BoxType::IpAddress;
  }else if(box_type_4cc == RECIEVING_PORT_BOX){
    return BoxType::RecievingPort;
  }else if(box_type_4cc == SKELETON_DEFINITION_BOX){
    return BoxType::SkeletonDefinition;
  }else if(box_type_4cc == BONE_DATA_ARRAY_BOX){
    return BoxType::BoneDataArray;
  }else if(box_type_4cc == BONE_DATA_BOX){
    return BoxType::BoneData;
  }else if(box_type_4cc == BONE_ID_BOX){
    return BoxType::BoneId;
  }else if(box_type_4cc == PARENT_BONE_ID_BOX){
    return BoxType::ParentBoneId;
  }else if(box_type_4cc == TRANSFORM_DATA_BOX){
    return BoxType::TransformData;
  }else if(box_type_4cc == FRAME_DATA_BOX){
    return BoxType::FrameData;
  }else if(box_type_4cc == FRAME_NUMBER_BOX){
    return BoxType::FrameNumber;
  }else if(box_type_4cc == TIMESTAMP_BOX){
    return BoxType::Timestamp;
  }else if(box_type_4cc == UTC_TIME_BOX){
    return BoxType::UtcTime;
  }else if(box_type_4cc == TIMECODE_BOX){
    return BoxType::Timecode;
  }else if(box_type_4cc == BONE_TRANSFORM_DATA_ARRAY_BOX){
    return BoxType::BoneTransformDataArray;
  }else if(box_type_4cc == BONE_TRANSFORM_DATA_BOX){
    return BoxType::BoneTransformData;
  }else if(box_type_4cc == SENSOR_DATA_BOX){
    return BoxType::SensorData;
  }else if(box_type_4cc == SENSOR_ADDRESS_BOX){
    return BoxType::SensorAddress;
  }else if(box_type_4cc == SENSOR_NAME_BOX){
    return BoxType::SensorName;
  }else if(box_type_4cc == SENSOR_JOINT_ID_BOX){
    return BoxType::SensorJointId;
  }else if(box_type_4cc == SENSOR_VALUE_BOX){
    return BoxType::SensorValue;
  }else{
    return BoxType::None;
  }
}

bool Const::BoxType4CC::IsBoxDataType(std::string box_type_4cc){
  return IsBoxDataType(StringToBoxType(box_type_4cc));
}

bool Const::BoxType4CC::IsBoxDataType(BoxType box_type){
  switch (box_type) {
    case BoxType::SkeletonDefinition:
    case BoxType::FrameData:
    case BoxType::SensorData:
      return true;
    default:
      return false;
  }
}

}

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

#include "mocopi_motion_format.hpp"

namespace mocopi_motion_format{

MocopiMotionFormat::MocopiMotionFormat(Box* box_header, Box* box_sender_difinition, Box* box_data){
  this->box_header = box_header;
  this->box_sender_difinition = box_sender_difinition;
  this->box_data = box_data;
}

MocopiMotionFormat::MocopiMotionFormat(){
  SetUp("", 0);
}

MocopiMotionFormat::MocopiMotionFormat(std::string sender_ip, unsigned short sender_port){
  SetUp(sender_ip, sender_port);
}

bool MocopiMotionFormat::IsSkeletonDefinition(){
  return box_data != nullptr && box_data->box_type == BoxType::SkeletonDefinition;
}

bool MocopiMotionFormat::IsFrameData(){
  return box_data != nullptr && box_data->box_type == BoxType::FrameData;
}

void MocopiMotionFormat::DeleteDeta(){
  if(box_header != nullptr){
    box_header->DeleteData();
    delete box_header;
    box_header = nullptr;
  }
  
  if(box_sender_difinition != nullptr){
    box_sender_difinition->DeleteData();
    delete box_sender_difinition;
    box_sender_difinition = nullptr;
  }
  
  if(box_data != nullptr){
    box_data->DeleteData();
    delete box_data;
    box_data = nullptr;
  }
}

void MocopiMotionFormat::SetUp(std::string sender_ip, unsigned short sender_port){
  std::vector<Box*> header_data_box;
  header_data_box.push_back(new Box(
                                    BoxType::FormatType,
                                    Const::FORMAT_TYPE_NAME,
                                    (int)Const::FORMAT_TYPE_NAME.size()
                                    ));
  header_data_box.push_back(new Box(
                                    BoxType::Version,
                                    Const::FORMAT_VERSION,
                                    1
                                    ));

  Box* header = new Box(BoxType::Head, header_data_box);
  
  this->box_header = header;
  
  std::vector<Box*> sender_definition_data_box;
  sender_definition_data_box.push_back(new Box(
                                               BoxType::IpAddress,
                                               Box::IpAddressToULong(sender_ip),
                                               sizeof(unsigned long long)
                                               ));
  sender_definition_data_box.push_back(new Box(
                                           BoxType::RecievingPort,
                                           sender_port,
                                           sizeof(unsigned short)
                                           ));
  
  Box* sender_definition = new Box(BoxType::SenderDefinition, sender_definition_data_box);
  
  this->box_sender_difinition = sender_definition;
}

}

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

#include "box.hpp"

namespace mocopi_motion_format {

Box::Box(){
  this->box_type = BoxType::None;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
  this->data_size = 0;
  this->data_num = 0;
  this->data = nullptr;
}

Box::Box(BoxType box_type, std::any data, int data_size){
  this->box_type = box_type;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
  
  if(box_type == BoxType::None){
    this->data = nullptr;
    this->data_size = 0;
    this->data_num = 0;
  }else{
    this->data = data;
    this->data_size = data_size;
    this->data_num = 1;
  }
}

Box::Box(BoxType box_type, std::any data, int data_size, int data_num){
  this->box_type = box_type;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
  
  if(box_type == BoxType::None){
    this->data = nullptr;
    this->data_size = 0;
    this->data_num = 0;
  }else{
    this->data = data;
    this->data_size = data_size;
    this->data_num = data_num;
  }
}

Box::Box(BoxType box_type, std::vector<Box*> children){
  this->box_type = box_type;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
  
  if(box_type == BoxType::None){
    this->data_size = 0;
    this->data = nullptr;
    this->data_num = 0;
  }else{
    int size = 0;
    
    for(int i=0; i<children.size(); i++){
      this->children.push_back(children[i]);
      size += children[i]->GetBoxSize();
    }
    
    this->data = children.data();
    this->data_size = size;
    this->data_num = (int)this->children.size();
  }
}

Box::Box(BoxType box_type, const float* quaternion, const float* vector3){
  this->box_type = box_type;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
  
  float* data_value = new float[7]{
    quaternion[0],
    quaternion[1],
    quaternion[2],
    quaternion[3],
    vector3[0],
    vector3[1],
    vector3[2],
  };
  
  this->data = data_value;
  this->data_num = 7;
  this->float_array_data = data_value;
  
  this->data_size = sizeof(float) * 7; //sizeof(data);
}

Box::Box(BoxType box_type, std::string data){
  // Handled as std::string, but char* is used for communication
  
  this->box_type = box_type;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
  
  if(box_type == BoxType::None){
    this->data = nullptr;
    this->data_size = 0;
    this->data_num = 0;
  }else{
    this->data = data;
    this->data_size = sizeof(char) * (int)data.length();
    this->data_num = 1;
    
    this->string_data = data;
  }
}

Box::Box(BoxType box_type, uint8_t hour, uint8_t min, uint8_t sec, uint8_t frame, uint8_t frame_rate, bool drop_frame){
  // NOTE: Timecode only
  
  this->box_type = box_type;
  this->box_type_4cc = Const::BoxType4CC::BoxTypeToString(box_type);
   
  uint8_t df = drop_frame? 1:0;
  
  uint8_t* data_value = new uint8_t[6]{
    hour, min, sec, frame, frame_rate, df
  };
  
  this->data = data_value;
  this->data_num = 6;
  this->uint8_t_array_data = data_value;
  
  this->data_size = sizeof(uint8_t) * 6;
}

int Box::GetBoxSize(){
  return sizeof(int) + sizeof(char) * 4 + data_size;
}

void Box::DeleteData(){
  if(float_array_data != nullptr){
    delete[] float_array_data;
    float_array_data = nullptr;
  }
  
  if(uint8_t_array_data != nullptr){
    delete [] uint8_t_array_data;
    uint8_t_array_data = nullptr;
  }
  
  // Release memory by swapping with empty string
  std::string().swap(string_data);
  
  if(children.size() > 0){
    for(int i=0; i<children.size(); i++){
      if(children[i] != nullptr){
        children[i]->DeleteData();
        delete children[i];
        children[i] = nullptr;
      }
    }
    children.clear();
  }
  
  data_num = 0;
  data_size = 0;
  data = nullptr;
}

unsigned long long Box::IpAddressToULong(std::string ip_address){
  unsigned short ip1, ip2, ip3, ip4;
  
#ifdef _WIN64
  sscanf_s(ip_address.c_str(), "%hu.%hu.%hu.%hu", &ip1, &ip2, &ip3, &ip4);
#else
  sscanf(ip_address.c_str(), "%hu.%hu.%hu.%hu", &ip1, &ip2, &ip3, &ip4);
#endif

  return
  (unsigned long long)ip1 * (1000 * 1000 * 1000) +
  (unsigned long long)ip2 * (1000 * 1000) +
  (unsigned long long)ip3 * (1000) +
  (unsigned long long)ip4;
}

std::string Box::ULongToIpAddress(unsigned long long ip_address){
  short ip1, ip2, ip3, ip4;
  ip4 = ip_address % 1000;
  ip_address /= 1000;
  ip3 = ip_address % 1000;
  ip_address /= 1000;
  ip2 = ip_address % 1000;
  ip_address /= 1000;
  ip1 = ip_address % 1000;
  
  return std::string(
                     std::to_string(ip1) + "." +
                     std::to_string(ip2) + "." +
                     std::to_string(ip3) + "." +
                     std::to_string(ip4)
                    );
}

}

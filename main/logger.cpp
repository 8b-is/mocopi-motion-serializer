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

#include "logger.hpp"

namespace mocopi_motion_format {

// staticは明示的に初期化する必要あり
const std::string Logger::LOG_TAG = "mocopi_motion_format";
bool Logger::is_debug = true;

void Logger::SetDebugMode(bool is_debug){
  Logger::is_debug = is_debug;
}

void Logger::Log(std::string message){
  if(is_debug){
#ifdef __ANDROID__
    __android_log_write(ANDROID_LOG_DEBUG, Logger::LOG_TAG.c_str(), message.c_str());
#else
    //std::cout << "【" << Logger::LOG_TAG << "】" << message << "\n";
#endif
  }
}

void Logger::LogWaring(std::string message){
  if(is_debug){
#ifdef __ANDROID__
    __android_log_write(ANDROID_LOG_WARN, Logger::LOG_TAG.c_str(), message.c_str());
#else
    //std::cerr << "【" << Logger::LOG_TAG << "】" << message << "\n";
#endif
  }
}

void Logger::LogError(std::string message){
  if(is_debug){
#ifdef __ANDROID__
    __android_log_write(ANDROID_LOG_ERROR, Logger::LOG_TAG.c_str(), message.c_str());
#else
    //std::cerr << "【" << Logger::LOG_TAG << "】" << message << "\n";
#endif
  }
}

}

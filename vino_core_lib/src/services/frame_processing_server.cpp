// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <vino_param_lib/param_manager.h>
#include <ros/ros.h>
#include <ros/service_callback_helper.h>
#include <memory>
#include <string>
#include <map>
#include <chrono>
#include <thread>

#include "vino_core_lib/services/frame_processing_server.h"

#include "vino_core_lib/pipeline_manager.h"
#include "vino_core_lib/pipeline.h"
#include "vino_core_lib/inputs/base_input.h"
#include "vino_core_lib/inputs/image_input.h"
#include "vino_core_lib/slog.h"

namespace vino_service
{
template <typename T>
FrameProcessingServer<T>::FrameProcessingServer(const std::string& service_name, const std::string& config_path)
  : service_name_(service_name), config_path_(config_path)
{
  nh_ = std::make_shared<ros::NodeHandle>(service_name_);
  initService();
}

template <typename T>                                                           
void FrameProcessingServer<T>::initService()
{
  std::cout << "!!!!" << config_path_ << std::endl;
  Params::ParamManager::getInstance().parse(config_path_);
  Params::ParamManager::getInstance().print();

  auto pcommon = Params::ParamManager::getInstance().getCommon();
  auto pipelines = Params::ParamManager::getInstance().getPipelines();

  if (pipelines.size() != 1)
  {
    throw std::logic_error("1 and only 1 pipeline can be set to FrameProcessServer!");
  }

  for (auto& p : pipelines)
  {
    PipelineManager::getInstance().createPipeline(p);
  }

  ros::ServiceServer srv = nh_->advertiseService<ros::ServiceEvent<typename T::Request, typename T::Response> >(
      "/openvino_toolkit/service", std::bind(&FrameProcessingServer::cbService, this, std::placeholders::_1));
  service_ = std::make_shared<ros::ServiceServer>(srv);
}

template <typename T>
bool FrameProcessingServer<T>::cbService(ros::ServiceEvent<typename T::Request, typename T::Response>& event)
{
  boost::shared_ptr<typename T::Response> res = boost::make_shared<typename T::Response>();
  std::map<std::string, PipelineManager::PipelineData> pipelines_ = PipelineManager::getInstance().getPipelines();
  for (auto it = pipelines_.begin(); it != pipelines_.end(); ++it)
  {
    PipelineManager::PipelineData& p = pipelines_[it->second.params.name.c_str()];
    auto input = p.pipeline->getInputDevice();
    
    Input::Config config;
    config.path = event.getRequest().image_path;
    input->config(config);

    p.pipeline->runOnce();
    auto output_handle = p.pipeline->getOutputHandle();

    for (auto& pair : output_handle)
    {
      if (!pair.first.compare(kOutputTpye_RosService))
      {
        pair.second->setServiceResponse(res);
        event.getResponse() = *res;
        pair.second->clearData();
        return true;  // TODO(weizhi) , return directly, suppose only 1 pipeline dealing with 1 request.
      }
    }
  }
  slog::info << "[FrameProcessingServer] Callback finished!" << slog::endl;
  return false;
}

template <>
bool FrameProcessingServer<object_msgs::DetectObject>::cbService(ros::ServiceEvent<object_msgs::DetectObject::Request, object_msgs::DetectObject::Response>& event)
{
  boost::shared_ptr<object_msgs::DetectObject::Response> res = boost::make_shared<object_msgs::DetectObject::Response>();
  std::map<std::string, PipelineManager::PipelineData> pipelines_ = PipelineManager::getInstance().getPipelines();
  for (auto it = pipelines_.begin(); it != pipelines_.end(); ++it)
  {
    PipelineManager::PipelineData& p = pipelines_[it->second.params.name.c_str()];
    auto input = p.pipeline->getInputDevice();
    
    Input::Config config;

    //TODO (Corsair-cxs) It's unfinished, I only do it for build success.
    config.path = event.getRequest().image_paths.front();
    input->config(config);

    p.pipeline->runOnce();
    auto output_handle = p.pipeline->getOutputHandle();

    for (auto& pair : output_handle)
    {
      if (!pair.first.compare(kOutputTpye_RosService))
      {
        pair.second->setServiceResponse(res);
        event.getResponse() = *res;
        pair.second->clearData();
        return true;  // TODO(weizhi) , return directly, suppose only 1 pipeline dealing with 1 request.
      }
    }
  }
  slog::info << "[FrameProcessingServer] Callback finished!" << slog::endl;
  return false;
}
//TODO It is not advisable to use object_msgs like DetectObject or ClassifyObject as a param for FrameProcessingServer.
template class FrameProcessingServer<object_msgs::DetectObject>;

template class FrameProcessingServer<vino_people_msgs::PeopleSrv>;
template class FrameProcessingServer<vino_people_msgs::ReidentificationSrv>;
template class FrameProcessingServer<vino_people_msgs::ObjectsInMasksSrv>;
}  // namespace vino_service

/*
 * Copyright (c) 2018 Intel Corporation
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

/**
 * @brief a header file with declaration of EmotionsDetection class and
 * EmotionsResult class
 * @file emotions_recognition.cpp
 */

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "vino_core_lib/inferences/emotions_detection.h"
#include "vino_core_lib/outputs/base_output.h"
#include "vino_core_lib/slog.h"

// EmotionsResult
vino_core_lib::EmotionsResult::EmotionsResult(const cv::Rect& location) : Result(location)
{
}

// Emotions Detection
vino_core_lib::EmotionsDetection::EmotionsDetection() : vino_core_lib::BaseInference()
{
}

vino_core_lib::EmotionsDetection::~EmotionsDetection() = default;

void vino_core_lib::EmotionsDetection::loadNetwork(const std::shared_ptr<Models::EmotionDetectionModel> network)
{
  valid_model_ = network;
  setMaxBatchSize(network->getMaxBatchSize());
}

bool vino_core_lib::EmotionsDetection::enqueue(const cv::Mat& frame, const cv::Rect& input_frame_loc)
{
  if (getEnqueuedNum() == 0)
  {
    results_.clear();
  }
  bool succeed = vino_core_lib::BaseInference::enqueue<float>(frame, input_frame_loc, 1, getResultsLength(),
                                                                 valid_model_->getInputName());
  if (!succeed)
  {
    slog::err << "Failed enqueue Emotion frame." << slog::endl;
    // TODO(weizhi): throw an error here
    return false;
  }
  Result r(input_frame_loc);
  results_.emplace_back(r);
  return true;
}

bool vino_core_lib::EmotionsDetection::submitRequest()
{
  return vino_core_lib::BaseInference::submitRequest();
}

bool vino_core_lib::EmotionsDetection::fetchResults()
{
  bool can_fetch = vino_core_lib::BaseInference::fetchResults();
  if (!can_fetch)
    return false;
  int label_length = static_cast<int>(valid_model_->getLabels().size());
  /// std::cout << "label_length = " << label_length << std::endl;
  std::string output_name = valid_model_->getOutputName();
  /// std::cout << "output_name = " << output_name << std::endl;
  InferenceEngine::Blob::Ptr emotions_blob = getEngine()->getRequest()->GetBlob(output_name);
  /** emotions vector must have the same size as number of channels
      in model output. Default output format is NCHW so we check index 1 */

  int64 num_of_channels = emotions_blob->getTensorDesc().getDims().at(1);
  /// std::cout << "num_of_channels " << num_of_channels << std::endl;
  if (num_of_channels != label_length)
  {
    slog::err << "Output size (" << num_of_channels << ") of the Emotions Recognition network is not equal "
              << "to used emotions vector size (" << label_length << ")" << slog::endl;
    throw std::logic_error("Output size (" + std::to_string(num_of_channels) +
                           ") of the Emotions Recognition network is not equal "
                           "to used emotions vector size (" +
                           std::to_string(label_length) + ")");
  }

  /** we identify an index of the most probable emotion in output array
      for idx image to return appropriate emotion name */
  auto emotions_values = emotions_blob->buffer().as<float*>();
  for (unsigned int idx = 0; idx < results_.size(); ++idx)
  {
    auto output_idx_pos = emotions_values + idx;
    int64 max_prob_emotion_idx = std::max_element(output_idx_pos, output_idx_pos + label_length) - output_idx_pos;
    results_[idx].label_ = valid_model_->getLabels()[max_prob_emotion_idx];
  }

  return true;
}

int vino_core_lib::EmotionsDetection::getResultsLength() const
{
  return static_cast<int>(results_.size());
}

const vino_core_lib::Result* vino_core_lib::EmotionsDetection::getLocationResult(int idx) const
{
  return &(results_[idx]);
}

const std::string vino_core_lib::EmotionsDetection::getName() const
{
  return valid_model_->getModelCategory();
}

void vino_core_lib::EmotionsDetection::observeOutput(const std::shared_ptr<Outputs::BaseOutput>& output)
{
  if (output != nullptr)
  {
    output->accept(results_);
  }
}

const std::vector<cv::Rect>
vino_core_lib::EmotionsDetection::getFilteredROIs(const std::string filter_conditions) const
{
  if (!filter_conditions.empty())
  {
    slog::err << "Emotion detection does not support filtering now! "
              << "Filter conditions: " << filter_conditions << slog::endl;
  }
  std::vector<cv::Rect> filtered_rois;
  for (auto res : results_)
  {
    filtered_rois.push_back(res.getLocation());
  }
  return filtered_rois;
}

// Copyright 2021 The DaisyKit Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DAISYKIT_MODELS_POSE_DETECTOR_MOVENET_H_
#define DAISYKIT_MODELS_POSE_DETECTOR_MOVENET_H_

#include "daisykitsdk/common/types.h"
#include "daisykitsdk/models/image_model.h"
#include "daisykitsdk/models/ncnn_model.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

namespace daisykit {
namespace models {

/// Human pose estimation model.
class PoseDetectorMoveNet : public NCNNModel, public ImageModel {
 public:
  PoseDetectorMoveNet(const char* param_buffer,
                      const unsigned char* weight_buffer, int input_width = 256,
                      int input_height = 256, bool use_gpu = false);

  PoseDetectorMoveNet(const std::string& param_file,
                      const std::string& weight_file, int input_width = 256,
                      int input_height = 256, bool use_gpu = false);

  /// Detect single human pose.
  /// This function adds offset_x and offset_y to the keypoints.
  int Predict(const cv::Mat& image, std::vector<types::Keypoint>& keypoints,
              float offset_x = 0, float offset_y = 0);

  /// Detect multiple human poses.
  /// Return 0 on success, otherwise return the number of inference errors.
  int PredictMulti(const cv::Mat& image,
                   const std::vector<types::Object>& objects,
                   std::vector<std::vector<types::Keypoint>>& poses);

  /// Draw keypoints and their joints.
  void DrawKeypoints(const cv::Mat& image,
                     const std::vector<types::Keypoint>& keypoints);

 private:
  /// Preprocess image data to obtain net input.
  void Preprocess(const cv::Mat& image, ncnn::Mat& net_input) override;

  void PrepareFeatureKeypointsParams(float input_size);

  const int kNumJoints = 17;

  int feature_size_;
  float kpt_scale_;
  std::vector<std::vector<float>> dist_y_, dist_x_;

  // Preprocess params
  float scale_;
  int hpad_, wpad_;
};

}  // namespace models
}  // namespace daisykit

#endif

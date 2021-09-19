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

#include "daisykitsdk/flows/face_detector_with_mask_flow.h"
#include "daisykitsdk/common/visualizers/face_visualizer.h"
#include "daisykitsdk/thirdparties/json.hpp"

namespace daisykit {
namespace flows {

FaceDetectorWithMaskFlow::FaceDetectorWithMaskFlow(
    const std::string& config_str, const io::DataReader& data_reader) {
  nlohmann::json config = nlohmann::json::parse(config_str);

  char* model;
  char* weights;
  data_reader.ReadFile(config["face_detection_model"]["model"], &model);
  data_reader.ReadFile(config["face_detection_model"]["weights"], &weights);

  face_detector_ = new models::FaceDetectorWithMask(
      model, (unsigned char*)weights,
      config["face_detection_model"]["input_width"],
      config["face_detection_model"]["input_height"],
      config["face_detection_model"]["score_threshold"],
      config["face_detection_model"]["iou_threshold"]);

  // Free models and weights to prevent memory leak
  free(model);
  free(weights);

  with_landmark_ = config["with_landmark"];
  if (with_landmark_) {
    facial_landmark_estimator_ = new models::FacialLandmarkEstimator(
        config["facial_landmark_model"]["model"],
        config["facial_landmark_model"]["weights"]);
  }
}

FaceDetectorWithMaskFlow::~FaceDetectorWithMaskFlow() {
  delete face_detector_;
  face_detector_ = nullptr;
  delete facial_landmark_estimator_;
  facial_landmark_estimator_ = nullptr;
}

void FaceDetectorWithMaskFlow::Process(cv::Mat& rgb) {
  // Detect faces
  std::vector<types::Face> faces = face_detector_->Predict(rgb);

  // Detect landmarks
  if (with_landmark_) {
    facial_landmark_estimator_->DetectMulti(rgb, faces);
  }

  {
    const std::lock_guard<std::mutex> lock(faces_lock_);
    faces_ = faces;
  }
}

void FaceDetectorWithMaskFlow::DrawResult(cv::Mat& rgb) {
  // Draw face bounding boxes and keypoints
  double fps = profiler.Tick();
  {
    const std::lock_guard<std::mutex> lock(faces_lock_);
    for (auto face : faces_) {
      cv::Scalar color(0, 255, 0);
      if (face.wearing_mask_prob < 0.5) {
        color = cv::Scalar(255, 0, 0);
      }
      cv::rectangle(rgb, cv::Rect(face.x, face.y, face.w, face.h), color, 2);
      visualizers::BaseVisualizer::PutText(
          rgb, face.wearing_mask_prob < 0.5 ? "No Mask" : "Mask",
          cv::Point(face.x, face.y), cv::FONT_HERSHEY_SIMPLEX, 0.8, 2, 10,
          cv::Scalar(0, 0, 0), color);

      if (with_landmark_) {
        cv::putText(rgb, std::to_string(face.landmark.size()),
                    cv::Point(100, 100), cv::FONT_HERSHEY_PLAIN, 1.0,
                    cv::Scalar(0, 255, 0), 2);
        facial_landmark_estimator_->DrawKeypoints(rgb, face.landmark);
      }

      visualizers::BaseVisualizer::PutText(
          rgb, std::to_string(fps), cv::Point(100, 100),
          cv::FONT_HERSHEY_SIMPLEX, 0.8, 2, 10, cv::Scalar(0, 0, 0), color);
    }
  }
}

}  // namespace flows
}  // namespace daisykit

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

#ifndef DAISYKIT_PROCESSORS_IMAGE_PROCESSORS_IMG_UTILS_H_
#define DAISYKIT_PROCESSORS_IMAGE_PROCESSORS_IMG_UTILS_H_

#include <opencv2/opencv.hpp>

namespace daisykit {
namespace processors {

class ImgUtils {
 public:
  static cv::Mat SquarePadding(const cv::Mat& img, int target_width = 500);
};

}  // namespace processors
}  // namespace daisykit

#endif

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

#include "daisykitsdk/graphs/core/packet.h"
#include "daisykitsdk/common/utils/timer.h"

#include <atomic>
#include <chrono>
#include <ctime>
#include <memory>
#include <mutex>

namespace daisykit {
namespace graphs {

Packet::Packet() { data_available_ = false; }

Packet::Packet(std::shared_ptr<void> data, utils::TimePoint timestamp) {
  // We dont use SetData() here because using mutex lock
  // in the constructor causes unpredicted behaviors
  data_ = data;
  timestamp_ = timestamp;
  data_available_ = true;
}

}  // namespace graphs
}  // namespace daisykit

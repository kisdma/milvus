// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License.

#include "scheduler/resource/CpuResource.h"
#include "knowhere/index/vector_index/helpers/BuilderSuspend.h"

#include <utility>

namespace milvus {
namespace scheduler {

std::ostream&
operator<<(std::ostream& out, const CpuResource& resource) {
    out << resource.Dump().dump();
    return out;
}

CpuResource::CpuResource(std::string name, uint64_t device_id, bool enable_executor)
    : Resource(std::move(name), ResourceType::CPU, device_id, enable_executor) {
}

void
CpuResource::Load(TaskPtr task) {
    task->Load(LoadType::DISK2CPU, 0);
}

void
CpuResource::Execute(TaskPtr task) {
    if (task->Type() == TaskType::SearchTask) {
        knowhere::BuilderSuspend();
    }
    task->Execute();
    if (task->Type() == TaskType::SearchTask) {
        knowhere::BuildResume();
    }
}

}  // namespace scheduler
}  // namespace milvus

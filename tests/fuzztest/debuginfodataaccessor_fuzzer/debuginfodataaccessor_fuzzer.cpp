/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "debuginfodataaccessor_fuzzer.h"

#include "libpandafile/class_data_accessor-inl.h"
#include "libpandafile/debug_data_accessor-inl.h"
#include "libpandafile/method_data_accessor-inl.h"

namespace OHOS {
    void DebugInfoDataAccessorFuzzTest(const uint8_t* data, size_t size)
    {
        auto pf = panda::panda_file::OpenPandaFileFromMemory(data, size);
        if (pf == nullptr) {
            return;
        }
        auto classes = pf->GetClasses();
        const auto &panda_file = *pf;
        for (size_t i = 0; i < classes.Size(); i++) {
            panda::panda_file::File::EntityId id(classes[i]);
            if (panda_file.IsExternal(id)) {
                continue;
            }

            panda::panda_file::ClassDataAccessor cda(panda_file, id);
            cda.EnumerateMethods([&](panda::panda_file::MethodDataAccessor &data_accessor) {
                panda::panda_file::DebugInfoDataAccessor dda(panda_file, data_accessor.GetDebugInfoId().value());
            });
        }
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DebugInfoDataAccessorFuzzTest(data, size);
    return 0;
}

/*
 * This file is open source software, licensed to you under the terms
 * of the Apache License, Version 2.0 (the "License").  See the NOTICE file
 * distributed with this work for additional information regarding copyright
 * ownership.  You may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (C) 2015 Cloudius Systems, Ltd.
 */

#pragma once

#ifdef SEASTAR_DEBUG_SHARED_PTR

#ifndef SEASTAR_MODULE
#include <thread>
#include <cassert>

#include <seastar/core/on_internal_error.hh>
#include <seastar/util/modules.hh>
#endif

namespace seastar {

extern logger seastar_logger;

// A counter that is only comfortable being incremented on the cpu
// it was created on.  Useful for verifying that a shared_ptr
// or lw_shared_ptr isn't misued across cores.
SEASTAR_MODULE_EXPORT
class debug_shared_ptr_counter_type {
    long _counter = 0;
    std::thread::id _cpu = std::this_thread::get_id();
public:
    debug_shared_ptr_counter_type(long x) noexcept : _counter(x) {}
    operator long() const {
        check();
        return _counter;
    }
    debug_shared_ptr_counter_type& operator++() {
        check();
        ++_counter;
        return *this;
    }
    long operator++(int) {
        check();
        return _counter++;
    }
    debug_shared_ptr_counter_type& operator--() {
        check();
        --_counter;
        return *this;
    }
    long operator--(int) {
        check();
        return _counter--;
    }
private:
    void check() const {
        // if (__builtin_expect(_cpu != std::this_thread::get_id(), false)) {
        //    on_fatal_internal_error(seastar_logger, "shared_ptr accessed on non-owner cpu");
      //  }
    }
};

}

#endif


// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <boost/cstdint.hpp>
#include <ostream>
#include <string>

#include "gen_cpp/AgentService_types.h"
#include "gen_cpp/Descriptors_types.h"
#include "gen_cpp/Exprs_types.h"
#include "gen_cpp/Opcodes_types.h"
#include "gen_cpp/PlanNodes_types.h"
#include "gen_cpp/RuntimeProfile_types.h"
#include "gen_cpp/Types_types.h"

namespace starrocks {

class PUniqueId;

std::string print_plan_node_type(const TPlanNodeType::type& type);
std::string print_tstmt_type(const TStmtType::type& type);
std::string print_query_state(const QueryState::type& type);
std::string PrintTUnit(const TUnit::type& type);
std::string PrintTMetricKind(const TMetricKind::type& type);

// Returns a string "<product version number> (build <build hash>)"
// If compact == false, this string is appended: "\nBuilt on <build time>"
// This is used to set gflags build version
std::string get_build_version(bool compact);

// similar with std::string get_build_version(bool), but without allocate any memory from heap
size_t get_build_version(char* buffer, size_t max_size);

// Returns a string "<product version number> (<short build hash>)"
std::string get_short_version();

// Returns "<program short name> version <GetBuildVersion(compact)>"
std::string get_version_string(bool compact);

std::string hexdump(const char* buf, size_t len);

} // namespace starrocks

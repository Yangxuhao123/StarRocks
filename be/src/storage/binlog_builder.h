// Copyright 2021-present StarRocks, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "storage/binlog_file_writer.h"

namespace starrocks {

// Parameters to create a binlog builder
struct BinlogBuilderParams {
    // storage directory for binlog file
    std::string binlog_storage_path;
    // maximum size of binlog file
    int64_t max_file_size;
    // maximum size of a page in binlog file
    int32_t max_page_size;
    // compression type for page
    CompressionTypePB compression_type;
    // file id for the next new binlog file
    int64_t start_file_id;
    // active file writer and it's current meta. The new ingestion can
    // append binlog to the writer. nullptr if there is no active writer
    std::shared_ptr<BinlogFileWriter> active_file_writer;
    std::shared_ptr<BinlogFileMetaPB> active_file_meta;
};

// The result of builder which will be applied to the meta of BinlogManager. No matter whether
// the build is successful (BinlogBuilder#commit) or failed （BinlogBuilder#abort), should always
// update the metas of BinlogManager, such as next_file_id and active_writer used for the next
// build. If build successfully also need add new binlog file metas to BinlogManager.
struct BinlogBuildResult {
    // parameters used to create the builder, used to fallback if the result is discarded
    std::shared_ptr<BinlogBuilderParams> params;
    // file id for the next new binlog file in the next ingestion
    int64_t next_file_id;
    // the writer can be used for the next build. nullptr if there is
    // no active writer
    std::shared_ptr<BinlogFileWriter> active_writer;
    // New binlog file metas if build successfully
    std::vector<std::shared_ptr<BinlogFileMetaPB>> metas;
};

using BinlogBuilderParamsPtr = std::shared_ptr<BinlogBuilderParams>;
using BinlogBuildResultPtr = std::shared_ptr<BinlogBuildResult>;

// Write the binlog of an ingestion into multiple binlog files.
//
// How to use
//  std::shared_ptr<BinlogBuilder> builder;
//  // add multiple item
//  builder->add_insert_range()
//  builder->add_insert_range()
//  // commit the binlog data in this version, and ensure they are persisted
//  builder->commit();
//  // abort if there is any error when add_xxx() or commit()
//  builder->abort();

class BinlogBuilder {
public:
    BinlogBuilder(int64_t tablet_id, int64_t version, int64_t change_event_timestamp, BinlogBuilderParamsPtr& params);

    // Those add_xxx methods correspond to that in BinlogFileWriter
    Status add_empty();
    Status add_insert_range(const RowsetSegInfo& seg_info, int32_t start_row_id, int32_t num_rows);

    // Commit the binlog data, and make sure they are persisted. Return Status::OK()
    // if successfully, and the result of this build will be put into *result*,
    // otherwise return other status if error happens.
    Status commit(BinlogBuildResult* result);

    // Abort the build if there is any error when add_xxx() or commit(). The result of
    // this build will be put into *result*.
    void abort(BinlogBuildResult* result);

    // Delete binlog files in *file_paths*. Returns Status::OK() if all of them are deleted
    // successfully, otherwise return other status if some of them fail to delete
    static Status delete_binlog_files(std::vector<std::string>& file_paths);

    // Delete binlog data generated by a successful build (commit successfully),
    // and return a binlog writer that can be used for the next ingestion. The
    // writer can be nullptr if there is no such writer.
    static BinlogFileWriterPtr discard_binlog_build_result(int64_t version, BinlogBuildResult& result);

    // Generate binlog for duplicate key table.
    static Status build_duplicate_key(int64_t tablet_id, int64_t version, const RowsetSharedPtr& rowset,
                                      BinlogBuilderParamsPtr& builder_params, BinlogBuildResult* result);

    // For testing
    int32_t num_files() { return (_params->active_file_meta == nullptr ? 0 : 1) + _new_files.size(); }

    int64_t current_write_file_size() { return _current_writer == nullptr ? 0 : _current_writer->file_size(); }

private:
    Status _switch_writer_if_full();
    StatusOr<BinlogFileWriterPtr> _create_binlog_writer();
    Status _commit_current_writer(bool end_of_version);
    Status _abort_current_writer();
    Status _close_current_writer();

    int64_t _tablet_id;
    int64_t _version;
    int64_t _change_event_timestamp;
    BinlogBuilderParamsPtr _params;

    int64_t _next_file_id;
    // Whether to have used _params.active_file_writer to initialize
    // the current writer
    bool _init_writer;
    int64_t _next_seq_id;
    std::vector<std::string> _new_files;
    BinlogFileWriterPtr _current_writer;
    std::vector<std::shared_ptr<BinlogFileMetaPB>> _new_metas;
};

} // namespace starrocks

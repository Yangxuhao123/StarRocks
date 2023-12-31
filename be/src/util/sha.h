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

#include <openssl/sha.h>

#include <string>

namespace starrocks {

class SHA224Digest {
public:
    SHA224Digest();

    void update(const void* data, size_t length);
    void digest();

    const std::string& hex() const { return _hex; }

private:
    SHA256_CTX _sha224_ctx;
    std::string _hex;
};

class SHA256Digest {
public:
    SHA256Digest();

    void update(const void* data, size_t length);
    void digest();

    const std::string& hex() const { return _hex; }

private:
    SHA256_CTX _sha256_ctx;
    std::string _hex;
};

class SHA384Digest {
public:
    SHA384Digest();

    void update(const void* data, size_t length);
    void digest();

    const std::string& hex() const { return _hex; }

private:
    SHA512_CTX _sha384_ctx;
    std::string _hex;
};

class SHA512Digest {
public:
    SHA512Digest();

    void update(const void* data, size_t length);
    void digest();

    const std::string& hex() const { return _hex; }

private:
    SHA512_CTX _sha512_ctx;
    std::string _hex;
};
} // namespace starrocks

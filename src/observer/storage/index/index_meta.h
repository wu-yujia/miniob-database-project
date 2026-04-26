/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include "common/sys/rc.h"
#include "common/lang/string.h"
#include "common/lang/vector.h"

class TableMeta;
class FieldMeta;

namespace Json {
class Value;
}  // namespace Json

class IndexMeta
{
public:
  IndexMeta() = default;

  RC init(const char *name, const vector<const FieldMeta *> &fields);

public:
  const char        *name() const;
  const string      &field() const;  // 返回第一个字段，兼容旧代码
  const vector<string> &fields() const;
  int                field_num() const;

  void desc(ostream &os) const;

public:
  void      to_json(Json::Value &json_value) const;
  static RC from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index);

protected:
  string          name_;
  vector<string>  fields_;
};

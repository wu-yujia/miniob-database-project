/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "storage/index/index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("field_name");
const static Json::StaticString FIELD_FIELDS("fields");

RC IndexMeta::init(const char *name, const vector<const FieldMeta *> &fields)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_ = name;
  for (const FieldMeta *f : fields) {
    fields_.push_back(f->name());
  }
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[FIELD_NAME] = name_;
  if (fields_.size() == 1) {
    json_value[FIELD_FIELD_NAME] = fields_[0];
  }
  for (const string &f : fields_) {
    json_value[FIELD_FIELDS].append(f);
  }
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &name_value  = json_value[FIELD_NAME];
  const Json::Value &field_value = json_value[FIELD_FIELD_NAME];
  const Json::Value &fields_value = json_value[FIELD_FIELDS];

  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  vector<const FieldMeta *> field_metas;
  if (fields_value.isArray() && fields_value.size() > 0) {
    for (int i = 0; i < fields_value.size(); i++) {
      const Json::Value &fv = fields_value[i];
      if (!fv.isString()) continue;
      const FieldMeta *field = table.field(fv.asCString());
      if (nullptr == field) {
        LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), fv.asCString());
        return RC::SCHEMA_FIELD_MISSING;
      }
      field_metas.push_back(field);
    }
  } else if (field_value.isString()) {
    const FieldMeta *field = table.field(field_value.asCString());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
      return RC::SCHEMA_FIELD_MISSING;
    }
    field_metas.push_back(field);
  }

  return index.init(name_value.asCString(), field_metas);
}

const char *IndexMeta::name() const { return name_.c_str(); }

const string &IndexMeta::field() const { static string empty; return fields_.empty() ? empty : fields_[0]; }

const vector<string> &IndexMeta::fields() const { return fields_; }

int IndexMeta::field_num() const { return fields_.size(); }

void IndexMeta::desc(ostream &os) const
{
  os << "index name=" << name_ << ", fields=";
  for (size_t i = 0; i < fields_.size(); i++) {
    if (i > 0) os << ",";
    os << fields_[i];
  }
}

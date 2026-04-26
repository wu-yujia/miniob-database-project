#pragma once

#include "sql/stmt/stmt.h"
#include "common/lang/vector.h"

struct CreateIndexSqlNode;
class Table;
class FieldMeta;

class CreateIndexStmt : public Stmt
{
public:
  CreateIndexStmt(Table *table, const vector<const FieldMeta *> &field_metas, const string &index_name)
      : table_(table), field_metas_(field_metas), index_name_(index_name)
  {}

  virtual ~CreateIndexStmt() = default;

  StmtType type() const override { return StmtType::CREATE_INDEX; }

  Table                       *table() const { return table_; }
  const vector<const FieldMeta *> &field_metas() const { return field_metas_; }
  const string                &index_name() const { return index_name_; }

public:
  static RC create(Db *db, const CreateIndexSqlNode &create_index, Stmt *&stmt);

private:
  Table                       *table_ = nullptr;
  vector<const FieldMeta *>    field_metas_;
  string                       index_name_;
};

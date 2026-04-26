#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/text_type.h"
#include "common/value.h"

int TextType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::TEXTS && right.attr_type() == AttrType::TEXTS, "invalid type");
  return common::compare_string(
      (void *)left.data(), left.length(), (void *)right.data(), right.length());
}

RC TextType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  val.set_type(AttrType::TEXTS);
  return RC::SUCCESS;
}

RC TextType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::CHARS: {
      result.set_string((const char *)val.data());
      return RC::SUCCESS;
    }
    default: return RC::UNIMPLEMENTED;
  }
}

int TextType::cast_cost(AttrType type)
{
  if (type == AttrType::TEXTS) {
    return 0;
  }
  if (type == AttrType::CHARS) {
    return 1;
  }
  return INT32_MAX;
}

RC TextType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << (const char *)val.data();
  result = ss.str();
  return RC::SUCCESS;
}

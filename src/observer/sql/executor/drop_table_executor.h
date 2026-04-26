#pragma once

#include "sql/executor/command_executor.h"

class DropTableExecutor : public CommandExecutor
{
public:
  RC execute(SQLStageEvent *sql_event);
};
#include "sql/executor/command_executor.h"

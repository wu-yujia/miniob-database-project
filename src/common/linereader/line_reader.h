#ifndef COMMON_LINE_READER_H
#define COMMON_LINE_READER_H

#include <string>
#include <readline/readline.h>
#include <readline/history.h>

namespace common {
class MiniobLineReader
{
public:
  static MiniobLineReader &instance();

  void init(const std::string &history_file);

  std::string my_readline(const std::string &prompt);

  bool is_exit_command(const std::string &cmd);

private:
  MiniobLineReader();
  ~MiniobLineReader();

  bool check_and_save_history();

private:
  std::string    history_file_;
  time_t         previous_history_save_time_;
  int            history_save_interval_;
};
}  // namespace common

#endif  // COMMON_LINE_READER_H

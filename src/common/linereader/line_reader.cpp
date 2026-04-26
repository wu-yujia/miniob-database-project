#include "common/linereader/line_reader.h"
#include <cstdlib>
#include <ctime>

namespace common {

MiniobLineReader::MiniobLineReader()
    : previous_history_save_time_(0), history_save_interval_(30)
{
  rl_initialize();
}

MiniobLineReader::~MiniobLineReader()
{
  if (!history_file_.empty()) {
    write_history(history_file_.c_str());
  }
}

MiniobLineReader &MiniobLineReader::instance()
{
  static MiniobLineReader reader;
  return reader;
}

void MiniobLineReader::init(const std::string &history_file)
{
  history_file_ = history_file;
  if (!history_file.empty()) {
    read_history(history_file.c_str());
  }
  previous_history_save_time_ = time(nullptr);
}

std::string MiniobLineReader::my_readline(const std::string &prompt)
{
  char *line = readline(prompt.c_str());
  if (line == nullptr) {
    return "";
  }
  std::string result(line);
  free(line);

  if (!result.empty()) {
    add_history(result.c_str());
    check_and_save_history();
  }

  return result;
}

bool MiniobLineReader::is_exit_command(const std::string &cmd)
{
  return cmd == "exit" || cmd == "quit";
}

bool MiniobLineReader::check_and_save_history()
{
  time_t now = time(nullptr);
  if (now - previous_history_save_time_ >= history_save_interval_) {
    previous_history_save_time_ = now;
    if (!history_file_.empty()) {
      write_history(history_file_.c_str());
      return true;
    }
  }
  return false;
}

}  // namespace common

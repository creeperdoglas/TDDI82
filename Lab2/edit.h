#ifndef EDIT_H
#define EDIT_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

struct filtered_arguments
{

  std::vector<std::string> flags{};
  std::vector<std::string> parameters{};
};

class Edit
{

public:
  void open_file(std::string file_name, std::ifstream &file);
  std::vector<std::string> get_arguments(int argc, char *argv[]);
  std::vector<std::string> convert_text(std::ifstream &file);
  filtered_arguments filter_arguments(std::vector<std::string> arguments);

  void print(const std::vector<std::string> &text);
  void frequency(const std::vector<std::string> &text);
  void table(const std::vector<std::string> &text);
  std::vector<std::string> substitute(const filtered_arguments &filtered_args,
                                      std::vector<std::string> text);
  std::vector<std::string> remove(const filtered_arguments &filtered_args,
                                  std::vector<std::string> text);
  void print_word_counts(const std::map<std::string, unsigned int> &word_count, const std::string &largest_word, bool left_align);

private:
  std::string get_largest_word(const std::vector<std::string> &text);
  static std::map<std::string, unsigned int> count_word_frequency(const std::vector<std::string> &text);
  std::string word_new{};
  std::string word_old{};
};

#endif
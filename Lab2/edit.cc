#include "Edit.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <map>
#include <iomanip>

using namespace std;

string Edit::get_largest_word(const vector<string> &text)
{
  auto it = max_element(text.begin(), text.end(),
                        [](const auto &a, const auto &b) -> bool
                        {
                          return a.size() < b.size();
                        });
  return *it;
}

map<string, unsigned int> Edit::count_word_frequency(const vector<string> &text)
{
  map<string, unsigned int> word_count;
  for (const auto &word : text)
  {
    ++word_count[word];
  }
  return word_count;
}

void Edit::open_file(const string &file_name, ifstream &file)
{
  file.open(file_name);
}

vector<string> Edit::get_arguments(int argc, char *argv[])
{
  vector<string> args(argv + 2, argv + argc);
  return args;
}

vector<string> Edit::convert_text(ifstream &file)
{
  vector<string> text;
  copy(istream_iterator<string>(file),
       istream_iterator<string>(),
       back_inserter(text));
  return text;
}

filtered_arguments Edit::filter_arguments(const vector<string> &arguments)
{
  filtered_arguments new_args;

  for (const auto &arg : arguments)
  {
    if (arg.substr(0, 2) != "--") // Check if the flag doesn't start with "--"
    {
      throw std::invalid_argument("ERROR: Flags should be prefixed with '--'. Invalid flag: " + arg);
    }
    int first = arg.find("--");
    int second = arg.find("=");

    if (first != string::npos)
    {
      string flag_str = arg.substr(first, second - first);
      new_args.flags.push_back(flag_str);

      if (second != string::npos)
      {
        string parameter_str = arg.substr(second + 1);
        new_args.parameters.push_back(parameter_str);
      }
      else
      {
        new_args.parameters.push_back(""); // Default empty string if no parameter
      }
    }
  }

  return new_args;
}

void Edit::print(const vector<string> &text)
{
  for_each(text.begin(), text.end(), [](const string &s)
           { cout << s << " "; });
  cout << endl;
}

void Edit::print_word_counts(const vector<pair<string, unsigned int>> &word_pairs, const string &largest_word, bool left_align)
{
  for (const auto &item : word_pairs)
  {
    if (left_align)
    {
      cout << left << setw(largest_word.size()) << item.first << " " << item.second << endl;
    }
    else
    {
      cout << setw(largest_word.size()) << item.first << " " << item.second << endl;
    }
  }
}

void Edit::frequency(const vector<string> &text)
{
  map<string, unsigned int> word_count = count_word_frequency(text);
  vector<pair<string, unsigned int>> word_pairs(word_count.begin(), word_count.end());

  auto compare = [](const pair<const string, unsigned int> &lhs, const pair<const string, unsigned int> &rhs)
  {
    return lhs.second > rhs.second;
  };
  sort(word_pairs.begin(), word_pairs.end(), compare);

  string largest_word = get_largest_word(text);

  print_word_counts(word_pairs, largest_word, false);
}

void Edit::table(const vector<string> &text)
{
  map<string, unsigned int> word_count = count_word_frequency(text);
  vector<pair<string, unsigned int>> word_pairs(word_count.begin(), word_count.end());

  string largest_word = get_largest_word(text);

  print_word_counts(word_pairs, largest_word, true);
}

std::vector<std::string> Edit::remove(const filtered_arguments &filtered_args, std::vector<std::string> &text)
{
  for (size_t i = 0; i < filtered_args.flags.size(); ++i)
  {
    if (filtered_args.flags[i] == "--remove" && !filtered_args.parameters[i].empty())
    {
      const std::string &word_to_remove = filtered_args.parameters[i];
      std::cout << "Removing word: " << word_to_remove << std::endl;
      text.erase(std::remove(text.begin(), text.end(), word_to_remove), text.end());
    }
    else
    {
      std::cerr << "ERROR: Missing word to remove for --remove flag." << std::endl;
    }
  }
  return text;
}

std::vector<std::string> Edit::substitute(const filtered_arguments &filtered_args, std::vector<std::string> &text)
{
  for (size_t i = 0; i < filtered_args.flags.size(); ++i)
  {
    if (filtered_args.flags[i] == "--substitute" && !filtered_args.parameters[i].empty())
    {
      const std::string &param = filtered_args.parameters[i];
      size_t plus_pos = param.find("+");
      if (plus_pos != std::string::npos)
      {
        std::string old_word = param.substr(0, plus_pos);
        std::string new_word = param.substr(plus_pos + 1);
        std::cout << "Substituting word: " << old_word << " with " << new_word << std::endl;
        std::replace(text.begin(), text.end(), old_word, new_word);
      }
      else
      {
        std::cerr << "ERROR: Incorrect format for --substitute parameter. Expected =<old>+<new>." << std::endl;
      }
    }
  }
  return text;
}

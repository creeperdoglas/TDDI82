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
                        [](const auto &a, const auto &b)
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

void Edit::open_file(string file_name, ifstream &file)
{

  file.open(file_name);
}

vector<string> Edit::get_arguments(int argc, char *argv[])
{
  vector<string> args{argv, argv + argc};
  vector<string> temp_args{args.size()};

  copy(args.begin(), args.end(), temp_args.begin());
  temp_args.erase(temp_args.begin(), temp_args.begin() + 2);

  return temp_args;
}

vector<string> Edit::convert_text(ifstream &file)
{

  vector<string> text{};
  // for_each(istream_iterator<string>(file),
  //          istream_iterator<string>(), [&text](string s)
  //          { text.push_back(s); });
  // return text;
  copy(istream_iterator<string>(file),
       istream_iterator<string>(),
       back_inserter(text));

  return text;
}

// filtered_arguments Edit::filter_arguments(vector<string> arguments)
// {
//   filtered_arguments new_args{};

//   for_each(arguments.begin(), arguments.end(),
//            [&new_args](string s)
//            {
//              int first = s.find("--");
//              int second = s.find("=");

//              string flag_str = s.substr(first, second - first);
//              new_args.flags.push_back(flag_str);

//              string parameter_str = s.substr(second + 1, s.length() - flag_str.length() - 1);

//              if (s != flag_str)
//                new_args.parameters.push_back(parameter_str);
//            });

//   return new_args;
// }
filtered_arguments Edit::filter_arguments(vector<string> arguments)
{
  filtered_arguments new_args{};

  for (const auto &arg : arguments)
  {
    int first = arg.find("--");
    int second = arg.find("=");

    // Add the flag to the `flags` vector
    if (first != string::npos)
    {
      string flag_str = arg.substr(first, second - first);
      new_args.flags.push_back(flag_str);

      // Add the parameter to `parameters` if present
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

  for_each(text.begin(), text.end(), [](string s)
           { cout << s << " "; });
  cout << endl;
}
void Edit::frequency(const vector<string> &text)
{
  map<string, unsigned int> word_count = count_word_frequency(text);
  vector<pair<string, unsigned int>> word_pairs(word_count.begin(), word_count.end());

  auto compare = [](const auto &lhs, const auto &rhs)
  {
    return lhs.second > rhs.second;
  };
  sort(word_pairs.begin(), word_pairs.end(), compare);

  string largest_word = get_largest_word(text);

  for (const auto &item : word_pairs)
  {
    cout << setw(largest_word.size()) << item.first << " " << item.second << endl;
  }
}

void Edit::table(const vector<string> &text)
{
  map<string, unsigned int> word_count = count_word_frequency(text);
  vector<pair<string, unsigned int>> word_pairs(word_count.begin(), word_count.end());

  string largest_word = get_largest_word(text);

  for (const auto &item : word_pairs)
  {
    cout << left << setw(largest_word.size()) << item.first << " " << item.second << endl;
  }
}
std::vector<std::string> Edit::remove(const filtered_arguments &filtered_args, std::vector<std::string> text)
{

  for (size_t i = 0; i < filtered_args.flags.size(); ++i)
  {
    if (filtered_args.flags[i] == "--remove" && !filtered_args.parameters[i].empty())
    {
      std::string word_to_remove = filtered_args.parameters[i];
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

std::vector<std::string> Edit::substitute(const filtered_arguments &filtered_args, std::vector<std::string> text)
{
  for (size_t i = 0; i < filtered_args.flags.size(); ++i)
  {
    if (filtered_args.flags[i] == "--substitute" && !filtered_args.parameters[i].empty())
    {
      std::string param = filtered_args.parameters[i];
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

// vector<string> Edit::substitute(filtered_arguments filtered_args, vector<string> text)
// {

//   vector<string> param{filtered_args.parameters};
//   vector<string> flag{filtered_args.flags};
//   string old_word{};
//   string new_word{};

//   vector<string> new_text{text};

//   for_each(flag.begin(), flag.end(),
//            [&param, &old_word, &new_word](string s)
//            {
//              int tmp{};
//              string tmp_param{};
//              if (s != "--substitute")
//              {

//                tmp++;
//              }
//              else
//              {
//                tmp_param = param.at(tmp);

//                old_word = tmp_param.substr(0, tmp_param.find("+"));
//                new_word = tmp_param.substr(tmp_param.find("+") + 1);
//              }
//            });

//   replace(new_text.begin(), new_text.end(), old_word, new_word);

//   return new_text;
// }

// vector<string> Edit::remove(const filtered_arguments &filtered_args, vector<string> text)
// {
//   vector<string> params{filtered_args.parameters};
//   vector<string> flags{filtered_args.flags};
//   string remove_word{};

//   for (size_t i = 0; i < flags.size(); ++i)
//   {
//     if (flags[i] == "--remove")
//     {
//       if (i < params.size())
//       {
//         remove_word = params[i];
//         break;
//       }
//     }
//   }

//   if (!remove_word.empty())
//   {
//     cout << "Removing word: " << remove_word << endl;
//     text.erase(remove(text.begin(), text.end(), remove_word), text.end());
//   }
//   else
//   {
//     cout << "No word to remove or word not found in parameters." << endl;
//   }

//   return text;
// }

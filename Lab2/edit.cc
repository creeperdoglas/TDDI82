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

string Edit::get_largest_word(vector<string> text)
{

  auto it = max_element(text.begin(), text.end(),
                        [](const auto &a, const auto &b)
                        {
                          return a.size() < b.size();
                        });
  return *it;
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

  for_each(std::istream_iterator<std::string>(file),
           std::istream_iterator<std::string>(), [&text](std::string s)
           { text.push_back(s); });
  return text;
}

filtered_arguments Edit::filter_arguments(vector<string> arguments)
{
  filtered_arguments new_args{};

  for_each(arguments.begin(), arguments.end(),
           [&new_args](string s)
           {
             int first = s.find("--");
             int second = s.find("=");

             string flag_str = s.substr(first, second - first);
             new_args.flags.push_back(flag_str);

             string parameter_str = s.substr(second + 1, s.length() - flag_str.length() - 1);

             if (s != flag_str)
               new_args.parameters.push_back(parameter_str);
           });

  return new_args;
}

void Edit::print(vector<string> text)
{

  for_each(text.begin(), text.end(), [](string s)
           { cout << s << " "; });
  cout << endl;
}

void Edit::frequency(vector<string> text)
{

  string word = get_largest_word(text);

  auto print = [&word](pair<string, unsigned int> item)
  {
    cout << setw(word.size()) << item.first << " " << item.second << endl;
  };
  auto compare = [](pair<string, unsigned int> const &lhs, pair<string, unsigned int> const &rhs)
  {
    return lhs.second > rhs.second;
  };
  map<string, unsigned int> map_words;

  for_each(text.begin(), text.end(), [&map_words](string s)
           { map_words[s]++; });
  vector<pair<string, unsigned int>> setofwords{map_words.begin(), map_words.end()};

  sort(setofwords.begin(), setofwords.end(), compare);

  for_each(setofwords.begin(), setofwords.end(), print);
}

void Edit::table(vector<string> text)
{

  vector<string> tmp{text};
  sort(tmp.begin(), tmp.end());

  string word = get_largest_word(text);

  auto print = [&word](pair<string, unsigned int> item)
  {
    cout << left << setw(word.size()) << item.first << " " << item.second << endl;
  };

  map<string, unsigned int> map_words;

  for_each(text.begin(), text.end(), [&map_words](string s)
           { map_words[s]++; });
  vector<pair<string, unsigned int>> setofwords{map_words.begin(), map_words.end()};

  for_each(setofwords.begin(), setofwords.end(), print);
}

vector<string> Edit::substitute(filtered_arguments filtered_args, vector<string> text)
{

  vector<string> param{filtered_args.parameters};
  vector<string> flag{filtered_args.flags};
  string old_word{};
  string new_word{};

  vector<string> new_text{text};

  for_each(flag.begin(), flag.end(),
           [&param, &old_word, &new_word](string s)
           {
             int tmp{};
             string tmp_param{};
             if (s != "--substitute")
             {

               tmp++;
             }
             else
             {
               tmp_param = param.at(tmp);

               old_word = tmp_param.substr(0, tmp_param.find("+"));
               new_word = tmp_param.substr(tmp_param.find("+") + 1);
             }
           });

  replace(new_text.begin(), new_text.end(), old_word, new_word);

  return new_text;
}

// gammla remove, funkade inte pga att tmp alltid blev 1 pga att den var i for loopen, och då out of range kastas vid 0 eller 1 så funkade det inte
//  vector<string> Edit::remove(filtered_arguments filtered_args, vector<string> text)
//  {

//   vector<string> param{filtered_args.parameters};
//   vector<string> flag{filtered_args.flags};
//   string remove_word{};

//   vector<string> new_text{text};

//   for_each(flag.begin(), flag.end(),
//            [&remove_word, &param](string s)
//            {
//              int tmp{};
//              ++tmp;
//              if (s == "--remove")
//              {
//                remove_word = param.at(tmp);
//              }
//            });

//   cout << remove_word << endl;
//   new_text.erase(std::remove(new_text.begin(), new_text.end(), remove_word), new_text.end());

//   return new_text;
// }
vector<string> Edit::remove(filtered_arguments filtered_args, vector<string> text)
{
  vector<string> params{filtered_args.parameters};
  vector<string> flags{filtered_args.flags};
  string remove_word{};

  for (size_t i = 0; i < flags.size(); ++i)
  {
    if (flags[i] == "--remove")
    {
      if (i < params.size())
      {
        remove_word = params[i];
        break;
      }
    }
  }

  if (!remove_word.empty())
  {
    cout << "Removing word: " << remove_word << endl;
    text.erase(std::remove(text.begin(), text.end(), remove_word), text.end());
  }
  else
  {
    cout << "No word to remove or word not found in parameters." << endl;
  }

  return text;
}

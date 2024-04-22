#include <iostream>
#include <string>
#include <vector>
#include <fstream>
// #include <Edit.h>

int main(int argc, char *argv[])

{
  // check arguments
  std::vector<std::string> args{argv, argv + argc};

  std::vector<std::string> arguments{args.size()};

  std::copy(args.begin(), args.end(), arguments.begin());

  arguments.erase(arguments.begin(), arguments.begin() + 2);

  // arguments.resize(1);

  for (int i = 0; i < argc - 2; ++i)
    std::cout << arguments[i] << std::endl;

  // read textfile
  std::ifstream file(argv[1]);
  std::vector<std::string> text;

  std::string str{};

  while (file >> str)
  {
    text.push_back(str);
  }
  file.close();

  for (auto i : text)
  {
    std::cout << i << std::endl;
  }

  // text.txt --print --substitute=bla+bla --remove=bla

  return 0;
}
#include <map>
#include <string>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>

using namespace std;
int main()
{
  map<int, int> m;
  // istream_iterator<int> in(cin), eof;
  cout << "Enter numbers: " << endl;
  istream_iterator<int> in(cin), eof;
  while (in != eof)
  {
    m[*in]++;
    in++;
  }
  // for (auto it = m.begin(); it != m.end(); it++)
  // {
  //   cout << "Number: " << it->first << ", Frequency: " << it->second << endl;
  // }
  std::multimap<int, int, std::greater<>> freqMap;
  for (const auto &elem : m)
  {
    freqMap.insert(std::make_pair(elem.second, elem.first));
  }

  for (const auto &elem : freqMap)
  {
    std::cout << "Number: " << elem.second << ", Frequency: " << elem.first << std::endl;
  }

  return 0;
}

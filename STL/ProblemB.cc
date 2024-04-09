#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
using namespace std;

int main()
{
  vector<string> v;
  cout << "Enter numbers: " << endl;
  istream_iterator<string> in(cin), eof;
  copy(in, eof, back_inserter(v));
  vector<string> v2(v.begin(), v.end());
  sort(v.begin(), v.end());
  auto last = unique(v.begin(), v.end());
  v.erase(last, v.end());
  ostream_iterator<string> out(cout, "\n");
  copy(v.begin(), v.end(), out);
  cout << "Number of duplicates: " << (v2.size() - v.size()) << endl;
  return 0;
}

// int main()
// {
//   vector<int> s;
//   cout << "Enter numbers: " << endl;
//   istream_iterator<int> in(cin), eof;
//   copy(in, eof, back_inserter(s));
//   set<int> s2(s.begin(), s.end());
//   cout << "Number of duplicates: " << (s.size() - s2.size()) << endl;
//   ostream_iterator<int> out(cout, "\n");
//   copy(s2.begin(), s2.end(), out);
//   return 0;
// }

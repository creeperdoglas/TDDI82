#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

template <typename collection>
void read(collection &c)
{
  typename collection::value_type number;
  while (cin >> number)
  {
    c.push_back(number);
  }
}

template <typename collection>
void sort(collection &c)
{
  for (auto &i : c)
  {
    for (auto &j : c)
    {
      if (i < j)
        swap(i, j);
    }
  }
}

template <typename collection>
void uniq(collection &c)
{
  for (auto i = c.begin(); i != (c.end() - 1);)
  {

    if (*i == *(i + 1))
    {
      // cout << "Duplicate: " << *i << endl;
      i = c.erase(i);
    }

        else
    {
      i++;
    }
  }
}

template <typename collection>
void print(collection const &c)
{
  for (auto &i : c)
  {
    cout << i << endl;
  }
}

int main()
{
  vector<string> v;

  cout << "Filter unique numbers by hand" << endl;

  read(v);
  sort(v);
  uniq(v);
  print(v);
}

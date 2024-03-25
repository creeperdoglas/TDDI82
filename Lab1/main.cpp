#include "List.h"
#include <iostream>
#include <exception>

int main()
{
  List list{1, 2, 3, 4, 5, 6};

  // list.setSortOnInsert(false);

  // Test case: Check if providing invalid indices throws an exception
  try
  {
    List sublist{list.sub({2, 1, 2})};
    std::cout << "Sublist created) successfully. This was not expected." << std::endl;

    for (auto it = sublist.begin(); it != sublist.end(); ++it)
    {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  }
  catch (const std::exception &e)
  {

    std::cout << "Exception caught as expected: " << e.what() << std::endl;
  }

  return 0;
}
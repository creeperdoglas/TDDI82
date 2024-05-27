#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

struct Product
{
  std::string name;
  double weight;
  double volume;
};

std::ostream &operator<<(std::ostream &os, Product product)
{
  os << product.name << " [" << product.weight << " kg, " << product.volume << " m^3]";
  return os;
}

std::istream &operator>>(std::istream &is, Product &product)
{
  is >> product.name >> product.weight >> product.volume;
  return is;
}

double density(Product product)
{
  return product.weight / product.volume;
}

int main()
{
  double lower_tolerance;
  double upper_tolerance;

  std::cout << "Enter the lower density tolerance: ";
  std::cin >> lower_tolerance;

  std::cout << "Enter the upper density tolerance: ";
  std::cin >> upper_tolerance;

  int threshold;

  std::cout << "Enter the sequential fault threshold: ";
  std::cin >> threshold;

  std::ifstream ifs{"factory.txt"};

  std::vector<Product> products{
      std::istream_iterator<Product>{ifs},
      std::istream_iterator<Product>{}};

  auto it = std::search_n(products.begin(), products.end(), threshold, upper_tolerance,
                          [&lower_tolerance](Product const &p, double upper_tolerance)
                          {
                            double current_density{density(p)};
                            return !(lower_tolerance < current_density && current_density < upper_tolerance);
                          });

  if (it != products.end())
  {
    std::cout << "Found a faulty sequence after " << std::distance(products.begin(), it) << " products: ";
    std::copy_n(it, threshold, std::ostream_iterator<Product>{std::cout, " "});
  }
  else
  {
    std::cout << "No faults found";
  }

  std::cout << std::endl;
}
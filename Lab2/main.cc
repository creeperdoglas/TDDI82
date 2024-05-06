#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "Edit.h"
//   Information om komplettering:
//   Kompletteringen kan gälla hela filen och alla filer i labben,
//   så får ni komplettering på en sak, kan samma sak förekomma på
//   fler ställen utan att jag skrivit det.
//
//   Komplettering lämnas in via sendlab efter senast en (1) vecka
//
//   Har ni frågor om kompletteringen kan ni maila mig på:
//   Malte.Nilsson@liu.se

// Komplettering: Ni ska ha felkontroller för alla typer av fel som kan uppstå vid körning. (filnamn saknas, fil existerar ej, saknad parameter till flagga, felaktig parameter till flagga, fel antal kommandoradsargument)
/// Fixat

// Komplettering: Fånga exceptions på lämpligt sätt. Användaren ska aldrig behöva se “Terminate called after...”.
/// fixat.gjorde om main, nu slipper använder se terminate called after...

// Komplettering: kodupprepning mellan frequency och table
/// fixat, skapade count_word_frequency

// Komplettering. Programmet gör flertalet saker utan att användaren har angett detta.
/// fixade med den nya main metoden

// Komplettering: Använd const& för att undvika onödig kopiering (se även parametrar till lambda)

// Komplettering: Ta inte in alla argument i remove och substitute

// Komplettering: Det finns en mer lämplig algoritm för att kopiera från filen till en vektor. (eller konstruktor)

// Kommentar: Klassen har inget riktigt syfte

void run_flags(Edit &obj, std::vector<std::string> &text, const filtered_arguments &new_args)
{
  for (const std::string &s : new_args.flags)
  {
    if (s == "--print")
    {
      std::cout << "Text:\n";
      obj.print(text);
    }
    else if (s == "--table")
    {
      std::cout << "Table:\n";
      obj.table(text);
    }
    else if (s == "--frequency")
    {
      std::cout << "Frequency:\n";
      obj.frequency(text);
    }
    else if (s == "--substitute")
    {
      // Ensure parameters are provided
      if (new_args.parameters.empty())
      {
        throw std::invalid_argument("ERROR: Missing parameter for --substitute flag. Should be --substitute=<old>+<new>");
      }
      std::cout << "Text with substituted words:\n";
      text = obj.substitute(new_args, text);
      obj.print(text);
    }
    else if (s == "--remove")
    {
      // Ensure parameters are provided
      if (new_args.parameters.empty())
      {
        throw std::invalid_argument("ERROR: Missing parameter for --remove flag. Should be --remove<word>");
      }
      std::cout << "Text with removed words:\n";
      text = obj.remove(new_args, text);
      obj.print(text);
    }
    else
    {
      throw std::invalid_argument("ERROR: Invalid flag " + s + "." + " Flags: --print, --frequency, --table, --substitute<old>+<new>, --remove<word>");
    }
  }
}

void check_args(int argc, Edit obj, std::vector<std::string> text, filtered_arguments new_args)
{
  if (argc == 2)
  {
    std::cout << "ERROR: Missing arguments. \n"
                 "Usage: ./a.out filename.txt --flag \n"
                 "Flags: print, frequency, table, substitute<old>+<new>, remove<word>"
              << std::endl;
  }

  else if (argc > 2 && argc < 8)
  {
    run_flags(obj, text, new_args);
  }

  else if (argc > 7)
  {
    throw std::invalid_argument("ERROR: Too many arguments.");
    // std::cout << "ERROR: Too many arguments." << std::endl;
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "ERROR: File name is missing.\n"
                 "Usage: ./a.out filename.txt --flag\n"
                 "Flags: --print, --frequency, --table, --substitute<old>+<new>, --remove<word>"
              << std::endl;
    return 1;
  }

  std::string file_name = argv[1];
  if (file_name.find("--") == 0)
  {
    std::cout << "ERROR: First argument should be a file name, not a flag.\n"
                 "Usage: ./a.out filename.txt --flag\n"
                 "Flags: --print, --frequency, --table, --substitute<old>+<new>, --remove<word>"
              << std::endl;
    return 1;
  }

  std::vector<std::string> text;
  std::ifstream file;
  Edit obj;

  try
  {
    obj.open_file(file_name, file);

    // If the file couldn't be opened, handle the error
    if (!file.is_open())
    {
      throw std::ifstream::failure("File does not exist or cannot be opened: " + file_name);
    }

    // Convert file content to text
    text = obj.convert_text(file);
    file.close();

    // Check if the file content is empty
    if (text.empty())
    {
      throw std::invalid_argument("ERROR: The file '" + file_name + "' is empty.");
    }
  }
  catch (const std::ifstream::failure &)
  {
    std::cerr << "File does not exist or cannot be opened: " << file_name << std::endl;
    return 1;
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Process additional arguments
  std::vector<std::string> arguments = obj.get_arguments(argc, argv);
  filtered_arguments new_args = obj.filter_arguments(arguments);

  try
  {
    check_args(argc, obj, text, new_args);
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

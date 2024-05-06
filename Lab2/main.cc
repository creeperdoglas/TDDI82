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
/// fixat

// Komplettering: Ta inte in alla argument i remove och substitute
/// fixat, gjorde även om filter_arguments för att de skulle funka

// Komplettering: Det finns en mer lämplig algoritm för att kopiera från filen till en vektor. (eller konstruktor)
/// fixat

// Kommentar: Klassen har inget riktigt syfte

void process_print(Edit &obj, const std::vector<std::string> &text)
{
  std::cout << "Text:\n";
  obj.print(text);
}

void process_table(Edit &obj, const std::vector<std::string> &text)
{
  std::cout << "Table:\n";
  obj.table(text);
}

void process_frequency(Edit &obj, const std::vector<std::string> &text)
{
  std::cout << "Frequency:\n";
  obj.frequency(text);
}

void process_substitute(Edit &obj, std::vector<std::string> &text, const filtered_arguments &new_args)
{
  auto substitute_flag_idx = std::find(new_args.flags.begin(), new_args.flags.end(), "--substitute") - new_args.flags.begin();

  if (substitute_flag_idx < new_args.parameters.size() && !new_args.parameters[substitute_flag_idx].empty())
  {
    // Check if the parameter contains a '+'
    if (new_args.parameters[substitute_flag_idx].find('+') != std::string::npos)
    {
      std::cout << "Text with substituted words:\n";
      text = obj.substitute(new_args, text);
      obj.print(text);
    }
    else
    {
      throw std::invalid_argument("ERROR: Incorrect parameter for --substitute flag. Should be --substitute=<old>+<new>");
    }
  }
  else
  {
    throw std::invalid_argument("ERROR: Missing parameter for --substitute flag. Should be --substitute=<old>+<new>");
  }
}

void process_remove(Edit &obj, std::vector<std::string> &text, const filtered_arguments &new_args)
{
  auto remove_flag_idx = std::find(new_args.flags.begin(), new_args.flags.end(), "--remove") - new_args.flags.begin();
  if (remove_flag_idx < new_args.parameters.size() && !new_args.parameters[remove_flag_idx].empty())
  {
    std::cout << "Text with removed words:\n";
    text = obj.remove(new_args, text);
    obj.print(text);
  }
  else
  {
    throw std::invalid_argument("ERROR: Missing parameter for --remove flag. Should be --remove=<word>");
  }
}

void run_flags(Edit &obj, std::vector<std::string> &text, const filtered_arguments &new_args)
{
  for (const std::string &s : new_args.flags)
  {
    if (s == "--print")
    {
      process_print(obj, text);
    }
    else if (s == "--table")
    {
      process_table(obj, text);
    }
    else if (s == "--frequency")
    {
      process_frequency(obj, text);
    }
    else if (s == "--substitute")
    {
      process_substitute(obj, text, new_args);
    }
    else if (s == "--remove")
    {
      process_remove(obj, text, new_args);
    }
    else
    {
      throw std::invalid_argument("ERROR: Invalid flag " + s + ". Flags: --print, --frequency, --table, --substitute<old>+<new>, --remove=<word>");
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

  else if (argc >= 2 && argc < 8)
  {

    for (const std::string &flag : new_args.flags)
    {
      if (flag.substr(0, 2) != "--" && flag.substr(0, 1) != "-")
      {
        throw std::invalid_argument("ERROR: Flags should be prefixed with '--'. Invalid flag: " + flag);
      }
      if (flag != "--print" && flag != "--table" && flag != "--frequency" && flag != "--substitute" && flag != "--remove")
      {
        throw std::invalid_argument("ERROR: Invalid flag " + flag + ". Flags: --print, --frequency, --table, --substitute<old>+<new>, --remove=<word>");
      }
    }
    run_flags(obj, text, new_args);
  }
  else if (argc > 7)
  {
    throw std::invalid_argument("ERROR: Too many arguments.");
  }
}
int process_file(const std::string &file_name, std::vector<std::string> &text, Edit &obj)
{
  std::ifstream file;

  try
  {
    obj.open_file(file_name, file);

    if (!file.is_open())
    {
      throw std::ifstream::failure("File does not exist or cannot be opened: " + file_name);
    }

    text = obj.convert_text(file);
    file.close();

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

  return 0;
}

int process_arguments(int argc, char *argv[], std::vector<std::string> &text, Edit &obj, filtered_arguments &new_args)
{

  std::vector<std::string> arguments = obj.get_arguments(argc, argv);

  try
  {
    new_args = obj.filter_arguments(arguments);
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

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
  Edit obj;
  filtered_arguments new_args;

  if (process_file(file_name, text, obj) != 0)
  {
    return 1;
  }

  if (process_arguments(argc, argv, text, obj, new_args) != 0)
  {
    return 1;
  }

  return 0;
}

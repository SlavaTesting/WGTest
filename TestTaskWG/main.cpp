#include "OrderBook.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <boost/program_options.hpp>
using namespace boost::program_options;

int main(int argc, char *argv[])
{
    try
    {
        auto filePath = std::string{};
        options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "print usage message")
            ("file,f", value(&filePath), "specify file with orders info")
            ;

        auto vm = variables_map{};
        store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("file"))
        {
            auto ob = OrderBook{};

            auto tmp = vm["file"].as<std::string>();

            auto infile = std::ifstream{ vm["file"].as<std::string>() };
            auto line = std::string{};
            while (std::getline(infile, line))
            {
                std::istringstream iss(line);
                Order order;
                iss >> order;
                ob.ProcessOrder(order);
            }

            std::cout << ob.MaxPrice() << "\n";
            return 0;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }
    return 0;
}
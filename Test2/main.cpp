#include "Container.h"

#include <iostream>
#include <iterator>

#include <boost/filesystem.hpp>

#include <boost/program_options.hpp>
using namespace boost::program_options;
using namespace std;

int main(int argc, char *argv[])
{

    namespace fs = boost::filesystem;
    namespace sys = boost::system;

    try {
        string directoryPath;
        string fileToFind;

        options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "print usage message")
            ("scan,s", value(&directoryPath), "scan all files in specified directory")
            ("find,f", value(&fileToFind), "show all files with specified name")
            ;

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("scan"))
        {
            CustomAssociativeContainer c;
            fs::path filePath(vm["scan"].as<std::string>());
            for (fs::recursive_directory_iterator dir(filePath), dir_end; dir != dir_end; ++dir)
            {
                if (!(*dir).path().extension().empty())
                {
                    //std::cout << *dir << "\n";
                    c.Insert({ (*dir).path().filename().string(), (*dir).path().parent_path().string() });
                }
            }
            if (vm.count("find"))
            {
                auto node = c.SearchFiles(vm["find"].as<std::string>());
                std::copy(node.cbegin(), node.cend(), std::ostream_iterator<std::string>(std::cout, "\n"));
            }
            return 0;
        }
    }
    catch (exception& e)
    {
        cerr << e.what() << "\n";
    }
    return 0;
}
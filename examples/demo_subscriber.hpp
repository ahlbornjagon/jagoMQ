#include "include/jagonMQ.hpp"
#include "include/CLI11.hpp"

int main()
{

    



}

void parseArgs(int argc, char** argv)
{
    CLI::App app{"App description"};
    argv = app.ensure_utf8(argv);

    std::string filename = "default";
    app.add_option("-f,--file", filename, "A help string");

    CLI11_PARSE(app, argc, argv);
    return 0;

}
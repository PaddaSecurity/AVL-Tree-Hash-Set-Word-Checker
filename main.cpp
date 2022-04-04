#include <iostream>
#include "SpellCheckShell.hpp"


int main()
{
    try
    {
        SpellCheckShell shell;
        shell.run();
    }
    catch (SpellCheckShell::ShellException& e)
    {
        std::cout << "ERROR: " << e.reason() << std::endl;
    }

    return 0;
}


#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include <vector>

namespace CTRPluginFramework
{
    int main(void)
    {
        u64 TitleID = Process::GetTitleID();
        if (TitleID != 0x00040000001ACB00) // && TitleID != 0x000400000016AD00)
        {
            MessageBox(Color::Red << "Error", Color::Silver << "This plugin only works for DQMJ3P!")();
            Process::ReturnToHomeMenu();
        }

        PluginMenu *Plg = new PluginMenu(Color::Silver << "Present Codes", 1, 0, 0);
        Plg->SynchronizeWithFrame(true);
        *Plg += new MenuEntry("Present Code", nullptr, PresentCode, Color::Silver << "Allows you to input Present Codes with a txt file.\nSome codes are exclusive to DQMJ3, thus they will not work for DQMJ3P.\n\nInput file must be encoded in UTF-8!");
        Plg->Run();
        delete Plg;
        return 0;
    }
}

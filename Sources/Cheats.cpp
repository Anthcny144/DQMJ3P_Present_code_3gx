#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include <vector>

namespace CTRPluginFramework
{
    bool ChecksPattern(u32 &Address, const std::vector<u32> &Pattern)
    {
        for (u32 i = 0; i < Pattern.size(); i++)
            if (*(u32 *)(Address + i * 4) != Pattern[i])
                return false;

        return true;
    }

    void RemovePrefix(std::string &String)
    {
        u32 PrefixPos;
        for (u32 i = 0; i < String.length(); i++)
            if (String[i] == ' ')
            {
                PrefixPos = i;
                break;
            }

        std::string NewString = "";
        for (u32 i = PrefixPos + 1; i < String.length(); i++)
            NewString += String[i];

        String = NewString;
    }

    void PresentCode(MenuEntry *Entry)
    {
        // List files
        static std::vector<std::string> Files;
        Directory Folder("");
        Files.clear();
        Folder.ListFiles(Files, ".txt");
        if (Files.size() == 0)
        {
            MessageBox(Color::Red << "Error", Color::Silver << "No txt file found!")();
            Sleep(Milliseconds(100));
            return;
        }

        // Check if file exists
        File CodeFile("codes.txt", File::READ);
        if (!CodeFile.IsOpen())
        {
            MessageBox(Color::Red << "Error", Color::Silver << "File \"" << Color::Yellow << "codes.txt" << Color::Silver << "\" is missing!")();
            Sleep(Milliseconds(100));
            return;
        }

        // Search address
        static const std::vector<u32> Pattern = {0x82227C, 0x842932C};
        u32 Address, Stop = 0x9000000;
        if (!ChecksPattern(Address, Pattern))
        {
            Address = 0x8000000;
            while (true)
            {
                if (ChecksPattern(Address, Pattern))
                    break;

                if (Address == Stop)
                {
                    MessageBox(Color::Red << "Error", Color::Silver << "It looks like the Present Code menu is not opened!")();
                    Sleep(Milliseconds(100));
                    return;
                }

                Address += 4;
            }
        }

        // Read file lines
        LineReader Reader(CodeFile);
        std::vector<std::string> Codes;
        std::string Line;
        u32 Num = 0;
        Codes.clear();
        while (Reader(Line))
        {
            Num++;
            Codes.push_back(std::to_string(Num) << ") " << Line);
        }

        // Ask inputs
        static Keyboard KB(" ");
        KB.GetMessage() = CenterAlign("Choose code to input");
        KB.Populate(Codes);
        s8 Choice = KB.Open();
        if (Choice < 0) return;
        Line = Codes[Choice];

        KB.GetMessage() = CenterAlign("Choose line to write the code to");
        KB.Populate(std::vector<std::string>{"1", "2", "3", "4", "5"});
        Choice = KB.Open();
        if (Choice < 0) return;

        // Write code in memory
        RemovePrefix(Line);
        Address += (8 + Choice * 0x2EC);
        for (u8 i = 0; i < 20; i++) Process::Write16(Address, 0);
        Process::WriteString(Address, Line, Line.length() * 2, StringFormat::Utf16);
    }
}

/*
Code credits:
- Full list: https://jp.wazap.com/cheat/%E3%80%90DQMJ3%E3%80%91%E6%9C%80%E6%96%B0%E7%89%88%EF%BC%81%E3%83%97%E3%83%AC%E3%82%BC%E3%83%B3%E3%83%88%E3%82%B3%E3%83%BC%E3%83%89%E4%B8%80%E8%A6%A7%E3%80%90%E6%94%BB%E7%95%A5%E3%80%91%E3%83%91%E3%82%B9%E3%83%AF%E3%83%BC%E3%83%89/569834/
- J3P exlusive: https://angry-mhm.com/post-15720/
- J3 (confirmation): https://angry-mhm.com/post-942/
*/

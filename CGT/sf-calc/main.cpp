#include "./include/UCtrl.h"
#include <iostream>
#include <string>

void printHelp()
{
    std::cout << "Commands:\n"
              << "  + - * /     - operation\n"
              << "  =           - equals\n"
              << "  c           - clear\n"
              << "  <           - backspace\n"
              << "  !           - toggle sign\n"
              << "  s           - square\n"
              << "  i           - reciprocal (1/x)\n"
              << "  n           - negate\n"
              << "  m           - memory store\n"
              << "  r           - memory recall\n"
              << "  a           - memory add\n"
              << "  l           - memory clear\n"
              << "  y           - copy to clipboard\n"
              << "  p [frac]    - paste from clipboard (or set clipboard if arg given)\n"
              << "  t [frac]    - set clipboard (type)\n"
              << "  h           - help\n"
              << "  q           - quit\n"
              << "  ?           - show history\n"
              << "  @           - show settings\n"
              << "  #           - toggle display format\n"
              << "  $           - toggle operand source\n";
}

int main()
{
    TSettings settings;
    THistory history;
    TCtrl ctrl(&settings, &history);

    std::string clipboard;
    std::string memState;
    std::string display;

    std::cout << "Simple Fraction Calculator. Type h for help.\n";
    display = ctrl.getDisplay();
    std::cout << "Display: " << display << "\n";

    std::string line;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty())
            continue;
        char cmd = line[0];
        std::string arg = line.size() > 1 ? line.substr(1) : "";
        // trim leading/trailing spaces
        arg.erase(0, arg.find_first_not_of(" \t"));
        arg.erase(arg.find_last_not_of(" \t") + 1);

        int command = -1;
        bool isDigit = (cmd >= '0' && cmd <= '9');
        if (isDigit)
        {
            command = cmd - '0';
        }
        else
        {
            switch (cmd)
            {
            case '+':
                command = CMD_ADD;
                break;
            case '-':
                command = CMD_SUBTRACT;
                break;
            case '*':
                command = CMD_MULTIPLY;
                break;
            case '/':
                command = CMD_DIVIDE;
                break;
            case '=':
                command = CMD_EQUALS;
                break;
            case 'c':
                command = CMD_CLEAR;
                break;
            case '<':
                command = CMD_BACKSPACE;
                break;
            case '!':
                command = CMD_TOGGLE_SIGN;
                break;
            case 's':
                command = CMD_SQR;
                break;
            case 'i':
                command = CMD_RECIPROCAL;
                break;
            case 'n':
                command = CMD_NEGATE;
                break;
            case 'm':
                command = CMD_MEM_STORE;
                break;
            case 'r':
                command = CMD_MEM_RECALL;
                break;
            case 'a':
                command = CMD_MEM_ADD;
                break;
            case 'l':
                command = CMD_MEM_CLEAR;
                break;
            case 'y':
                command = CMD_COPY;
                break;
            case 'p':
                command = CMD_PASTE;
                if (!arg.empty())
                    clipboard = arg;
                break;
            case 't':
                clipboard = arg;
                std::cout << "Clipboard set to: " << clipboard << "\n";
                continue;
            case 'h':
                printHelp();
                continue;
            case 'q':
                return 0;
            case '?':
            {
                auto entries = history.getEntries();
                std::cout << "History:\n";
                for (const auto &e : entries)
                {
                    std::cout << "  " << e << "\n";
                }
                continue;
            }
            case '@':
            {
                std::cout << "Settings:\n"
                          << "  Display format: " << (settings.displayFormat() == DisplayFormat::Fraction ? "Fraction" : "Decimal") << "\n"
                          << "  Operand source: " << (settings.operandSource() == OperandSource::Memory ? "Memory" : "Clipboard") << "\n";
                continue;
            }
            case '#':
            {
                settings.setDisplayFormat(settings.displayFormat() == DisplayFormat::Fraction ? DisplayFormat::Decimal : DisplayFormat::Fraction);
                std::cout << "Display format toggled.\n";
                display = ctrl.getDisplay();
                std::cout << "Display: " << display << "\n";
                continue;
            }
            case '$':
            {
                settings.setOperandSource(settings.operandSource() == OperandSource::Memory ? OperandSource::Clipboard : OperandSource::Memory);
                std::cout << "Operand source toggled.\n";
                continue;
            }
            default:
                std::cout << "Unknown command. Type h for help.\n";
                continue;
            }
        }

        display = ctrl.executeCommand(command, clipboard, memState);
        std::cout << "Display: " << display << "\n";
        if (!memState.empty() && memState != "_Off")
        {
            std::cout << "Memory: " << memState << "\n";
        }
    }
    return 0;
}

#include "Debug.hpp"
#include "Console.hpp"

namespace Ge
{
        void Debug::Log(const char *format, ...)
        {
                char buffer[1000];
                char buffer2[1000];

                va_list argList;
                va_start(argList, format);
#ifdef _WIN32
                vsprintf(buffer, format, argList);
                sprintf_s(buffer2, "[ENGINE][USER] : %s\n", buffer);
#elif __unix__
                vsprintf(buffer, format, argList);
                sprintf(buffer2, "[ENGINE][USER] : %s\n", buffer);
#endif
                va_end(argList);

                std::cout << buffer2;
				if (Console::IsInit())
				{
					Console::GetConsole()->setBaseColor(ImColor(1.0f, 1.0f, 1.0f, 1.0f));
					Console::GetConsole()->AddLog("%s", buffer2);
				}
        }

        void Debug::Error(const char *format, ...)
        {
                char buffer[1000];
                char buffer2[1000];

                va_list argList;
                va_start(argList, format);
#ifdef _WIN32
                vsprintf(buffer, format, argList);
                sprintf_s(buffer2, "[ENGINE][ERROR] : %s\n", buffer);
#elif __unix__
                vsprintf(buffer, format, argList);
                sprintf(buffer2, "[ENGINE][ERROR] : %s\n", buffer);
#endif
                va_end(argList);

                std::cerr << termcolor::red << buffer2 << termcolor::reset;
				if (Console::IsInit())
				{
					Console::GetConsole()->setBaseColor(ImColor(0.5f, 0.0f, 0.0f, 1.0f));
					Console::GetConsole()->AddLog("%s", buffer2);
				}
                std::cin.ignore();
        }

        void Debug::INITSUCCESS(const char *format)
        {
                Debug::Info("Initialisation : %s", format);
        }

        void Debug::RELEASESUCCESS(const char *format)
        {
                Debug::Info("Liberation : %s", format);
        }

        void Debug::INITFAILED(const char *format)
        {
                Debug::Error("Erreur lors de l'initialisation : %s", format);
        }

        void Debug::Warn(const char *format, ...)
        {
                char buffer[1000];
                char buffer2[1000];

                va_list argList;
                va_start(argList, format);
#ifdef _WIN32
                vsprintf(buffer, format, argList);
                sprintf_s(buffer2, "[ENGINE][WARN] : %s\n", buffer);
#elif __unix__
                vsprintf(buffer, format, argList);
                sprintf(buffer2, "[ENGINE][WARN] : %s\n", buffer);
#endif
                va_end(argList);

                std::cout << termcolor::yellow << buffer2 << termcolor::reset;
				if (Console::GetConsole() != nullptr)
				{
					Console::GetConsole()->setBaseColor(ImColor(0.5f, 0.5f, 0.0f, 1.0f));
					Console::GetConsole()->AddLog("%s", buffer2);
				}
        }

        void Debug::Info(const char *format, ...)
        {
                char buffer[1000];
                char buffer2[1000];

                va_list argList;
                va_start(argList, format);
#ifdef _WIN32
                vsprintf(buffer, format, argList);
                sprintf_s(buffer2, "[ENGINE][INFO] : %s\n", buffer);
#elif __unix__
                vsprintf(buffer, format, argList);
                sprintf(buffer2, "[ENGINE][INFO] : %s\n", buffer);
#endif
                va_end(argList);

                std::cout << termcolor::cyan << buffer2 << termcolor::reset;
				if (Console::IsInit())
				{
					Console::GetConsole()->setBaseColor(ImColor(0.0f, 0.4f, 0.4f, 1.0f));
					Console::GetConsole()->AddLog("%s", buffer2);
				}
        }

        void Debug::VLayer(const char *format, ...)
        {
                char buffer[1000];
                char buffer2[1000];

                va_list argList;
                va_start(argList, format);
#ifdef _WIN32
                vsprintf(buffer, format, argList);
                sprintf_s(buffer2, "[ENGINE][VLAYER] : %s\n", buffer);
#elif __unix__
                vsprintf(buffer, format, argList);
                sprintf(buffer2, "[ENGINE][VLAYER] : %s\n", buffer);
#endif
                va_end(argList);

                std::cout << termcolor::magenta << buffer2 << termcolor::reset;
				if (Console::IsInit())
				{
					Console::GetConsole()->setBaseColor(ImColor(0.25f, 0.0f, 0.5f, 1.0f));
					Console::GetConsole()->AddLog("%s", buffer2);
				}
        }
}
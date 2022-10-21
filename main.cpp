#include <sstream>
#include <windows.h>

bool dump(int process_id, std::uintptr_t offset, std::uint64_t size, bool print);

int main(int argc, char** argv)
{
	  int process_id = std::atoi(argv[1]);

    std::uint64_t size;
    std::uintptr_t offset;
	  std::stringstream stream;

    stream << argv[2];
    stream >> std::hex >> offset;
    stream.clear();

    stream << argv[3];
    stream >> std::hex >> size;
    stream.clear();

    bool print = !strcmp(argv[4], "true") ? true : false;

    bool result = dump(process_id, offset, size, print);

    printf("%s\n", result ? "[!] Successfully dumped content!" : "[x] Could not dump the content :(");
    return 0;

}

bool dump(int process_id, std::uintptr_t offset, std::uint64_t size, bool print)
{
    bool result;
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
    if (!handle)
    {
        printf("[!] Could not open the process :(\n");
        return false;
    }

    char* buffer = new char[size];
    if (ReadProcessMemory(handle, (void*)offset, (void*)buffer, size, NULL))
    {
        if (print)
        {
            printf("[!] Caught buffer... fwriting...\n");
            fwrite(buffer, sizeof(char), size, stdout);
            printf("\n\n");
        }

        FILE* file = fopen("dump.bin", "wb");
        fwrite(buffer, sizeof(char), size, file);
        fclose(file); 
        result = true;
    }
    else
    {
        result = false;
    }

    delete[] buffer;
    CloseHandle(handle);
    return result;
}

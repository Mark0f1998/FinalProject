#include <iostream>


#ifndef HAVE_STDINT_H
#define HAVE_STDINT_H
#endif // HAVE_STDINT_H
#include <libcpuid/libcpuid.h>
#include <ncurses.h>




int main()
{
    initscr(); // compulsory transfer to curses - mode


    printw("Welcome to our project CPU_INFO %s. Here you can clearly understand your CPU name, clock and features! Starting to check your CPUID instruction\n\n", cpuid_lib_version()); // Initial greeting


    if (cpuid_present()) // Checks if the CPUID instruction is supported
    {
        printw("Ohh, super! Your CPUID instruction is supported!\n\n");
    }
    else
    {
        printw("Sorry, your CPU doesn't support CPUID! Error: %s", cpuid_error()); // Returns textual description of the last error
        return 1;
    }


    struct cpu_raw_data_t raw_data; // Contains only the most basic CPU data, required to do identification and feature recognition
    struct cpu_id_t id_data; // Contains the recognized CPU decoded data


    if (cpuid_get_raw_data(&raw_data) == 0) // Obtains the raw CPUID data from the current CPU
    {
        printw("Congratulations again! We successfully obtain the raw CPUID data from the current CPU!\n\n");
    }
    else
    {
        printw("Sorry, cannot get the CPUID raw data! Error: %s", cpuid_error());
        return 2;
    }


    if (cpu_identify(&raw_data, &id_data) == 0) // Identify the CPU, using the given raw_data as Input and id_data as Output
    {
        printw("We identified your CPU! This was the last check for today! Are you ready to see all the information about your processor?\n\n");
    }
    else
    {
        printw("Sorry, CPU identification failed! Error: %s", cpuid_error());
        return 3;
    }


    printw("CPU vendor string:\t %s\n", id_data.vendor_str);
    printw("Processor Model is:\t %s\n", id_data.cpu_codename);
    printw("The full brand is:\t %s\n", id_data.brand_str);
    printw("CPU clock by OS is:\t %d MHz\n\n", cpu_clock_by_os());


    std::pair<std::string, bool> flags[16] =                 // pairs of specific CPU features
    {std::make_pair("MMX", id_data.flags[CPU_FEATURE_MMX]),
     std::make_pair("MMX-ext", id_data.flags[CPU_FEATURE_MMXEXT]),
     std::make_pair("SSE", id_data.flags[CPU_FEATURE_SSE]),
     std::make_pair("SSE2", id_data.flags[CPU_FEATURE_SSE2]),
     std::make_pair("3DNow!", id_data.flags[CPU_FEATURE_3DNOW]),
     std::make_pair("AVX", id_data.flags[CPU_FEATURE_AVX]),
     std::make_pair("AVX2", id_data.flags[CPU_FEATURE_AVX2]),
     std::make_pair("512F", id_data.flags[CPU_FEATURE_AVX512F]),
     std::make_pair("512DQ", id_data.flags[CPU_FEATURE_AVX512DQ]),
     std::make_pair("512PF", id_data.flags[CPU_FEATURE_AVX512PF]),
     std::make_pair("512ER", id_data.flags[CPU_FEATURE_AVX512ER]),
     std::make_pair("512CD", id_data.flags[CPU_FEATURE_AVX512CD]),
     std::make_pair("512BW", id_data.flags[CPU_FEATURE_AVX512BW]),
     std::make_pair("512VL", id_data.flags[CPU_FEATURE_AVX512VL]),
     std::make_pair("SSE4A", id_data.flags[CPU_FEATURE_SSE4A]),
     std::make_pair("SSE5", id_data.flags[CPU_FEATURE_SSE5])};


    start_color(); // function to start work with colors
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    for (const auto& support : flags)
    {
        printw("%s\t ", support.first.c_str());
        attron(COLOR_PAIR(support.second ? 2 : 1));
        printw ("[%s]\n", (support.second ? "supported" : "not supported"));
        attroff(COLOR_PAIR(support.second ? 2 : 1));
    }


    printw("\n\nPlz tap any key to go out");


    refresh();
    getch(); // The function expects to press any key on the keyboard and returns the code for this key itself.
    endwin(); // release all resources allocated by the library and return the terminal to the state prior to initializing the library of your program


    return 0;
}

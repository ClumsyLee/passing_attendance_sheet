#include <cstdlib>
#include <iostream>
#include <random>
#include "classroom.h"

int main(int argc, char *argv[])
{
    if (argc == 6)
    {
        int row = std::atoi(argv[1]);
        int col = std::atoi(argv[2]);
        int start_row = std::atoi(argv[3]);
        int start_col = std::atoi(argv[4]);
        int turns = std::atoi(argv[5]);

        passing_attendance_sheet::Classroom classroom(row, col);
        for (int i = 0; i < turns; i++)
        {
            std::cout << "turn " << i << ": " << std::flush;
            std::cout << classroom.PassSheet(start_row, start_col)
                      << std::endl;
        }
    }

    return 0;
}

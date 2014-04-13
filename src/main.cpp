#include <cstdlib>
#include <iostream>
#include <random>
#include "classroom.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <row> <col> <repeat_times>\n";
        std::exit(EXIT_FAILURE);
    }

    int row = std::atoi(argv[1]);
    int col = std::atoi(argv[2]);
    int turns = std::atoi(argv[3]);

    passing_attendance_sheet::Classroom classroom(row, col);
    std::cout << "Classroom size: " << row << '*' << col << std::endl;

    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            long total_pass = 0L;
            for (int i = 1; i <= turns; i++)
            {
                total_pass += classroom.PassSheet(r, c);
            }
            std::cout << static_cast<double>(total_pass) /
                         static_cast<double>(turns) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

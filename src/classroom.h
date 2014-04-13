#ifndef PASSING_ATTENDANCE_SHEET_SRC_CLASSROOM_H_
#define PASSING_ATTENDANCE_SHEET_SRC_CLASSROOM_H_

#include <boost/multi_array.hpp>

namespace passing_attendance_sheet {

class Classroom
{
 public:
    // Make sure the opposite direction is on the opposite position.
    enum { UP, LEFT, RIGHT, DOWN};
    enum { DIRECTION_NUM = 4 };

    Classroom(int row, int col);
    ~Classroom();

    int PassSheet(int row, int col);

 private:
    class Student;
    typedef boost::multi_array<Student, 2> ArrayType;
    typedef ArrayType::index IndexType;

    void ModifySide();
    void Clear();

    static constexpr int kRowOffset[DIRECTION_NUM] = {-1, 0, 0, 1};
    static constexpr int kColOffset[DIRECTION_NUM] = {0, -1, 1, 0};

    bool is_clean;
    int row_, col_;
    boost::multi_array<Student, 2> students_;
};

}  // namespace passing_attendance_sheet

#endif  // PASSING_ATTENDANCE_SHEET_SRC_CLASSROOM_H_

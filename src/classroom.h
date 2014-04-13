#ifndef PASSING_ATTENDANCE_SHEET_SRC_CLASSROOM_H_
#define PASSING_ATTENDANCE_SHEET_SRC_CLASSROOM_H_

#include <random>
#include <vector>
#include <boost/multi_array.hpp>

namespace passing_attendance_sheet {

class Classroom
{
 public:
    // Make sure the opposite direction is on the opposite position.
    enum { UP, LEFT, RIGHT, DOWN};
    enum { DIRECTION_NUM = 4 };

    Classroom(int row, int col);

    int PassSheet(int start_row, int start_col);

 private:
    class Student
    {
     public:
        explicit Student(const std::vector<int> &neighbors = {0, 1, 2, 3});

        // seed the generator
        static void Seed(int seed) { generator.seed(seed); }

        int GiveSheet();
        // if new signed, return true
        bool PassedFrom(int direction, int &pass_to, int &status);

        bool has_signed() const { return has_signed_; }
        int passed_times(int direction) const;

     private:
        // make sure the first student will say 0
        enum { I_HAD_OTHER_CHOICES_BUT_I_PICKED_YOU,
               // "have you signed?"
               YOU_ARE_MY_ONLY_CHOICE_SO_DO_NOT_GIVE_IT_BACK_TO_ME,
               // "we have all signed over here"
               I_HAVE_SIGNED_AND_YOU_HAVE_OTHER_CHOICES_SO_TAKE_IT_BACK };
               // "yes, I have signed"

        // random generator
        static std::mt19937 generator;

        bool has_signed_;
        std::vector<int> possible_directions_;
        std::vector<int> signed_directions_;
        int passed_times_[DIRECTION_NUM];
    };  // class Student

    typedef boost::multi_array<Student, 2> ArrayType;
    typedef ArrayType::index IndexType;

    void ModifySide();
    void Clear();

    bool is_clean_;
    int row_, col_;
    boost::multi_array<Student, 2> students_;
};  // class Classroom

}  // namespace passing_attendance_sheet

#endif  // PASSING_ATTENDANCE_SHEET_SRC_CLASSROOM_H_

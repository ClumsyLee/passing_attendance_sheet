#include <algorithm>
#include <random>
#include <vector>
#include "classroom.h"

namespace passing_attendance_sheet {

std::mt19937 Classroom::Student::generator;

Classroom::Student::Student(const std::vector<int> &neighbors)
        : has_signed_(false),
          possible_directions_(neighbors),
          signed_directions_(),
          passed_times_{}
{
    std::shuffle(possible_directions_.begin(),
                 possible_directions_.end(),
                 generator);
}

int Classroom::Student::GiveSheet()
{
    has_signed_ = true;
    // get the choice
    int pass_choice = possible_directions_.back();
    possible_directions_.pop_back();
    signed_directions_.push_back(pass_choice);

    passed_times_[pass_choice]++;
    return pass_choice;
}


bool Classroom::Student::PassedFrom(int direction, int &pass_to, int &status)
{
    bool new_signed = false;
    if (has_signed_)
    {
        if (status == I_HAD_OTHER_CHOICES_BUT_I_PICKED_YOU)
        {
            status = I_HAVE_SIGNED_AND_YOU_HAVE_OTHER_CHOICES_SO_TAKE_IT_BACK;
            pass_to = direction;  // pass back
            passed_times_[direction]++;
            return false;
        }
    }
    else  // I haven't signed yet, so I'll take it
    {
        new_signed = true;
        has_signed_ = true;  // signing...Done.
    }

    auto position = std::find(possible_directions_.begin(),
                              possible_directions_.end(), direction);
    if (position != possible_directions_.end())  // I thought you didn't sign
    {
        possible_directions_.erase(position);  // well, now I know you did
        signed_directions_.push_back(direction);
    }

    // Now it's time to choose who to pass this sheet...
    int pass_choice;
    if (possible_directions_.size() >= 1)  // I have one or several choices...
    {
        pass_choice = possible_directions_.back();  // picked up a person
        possible_directions_.pop_back();            // randomly, here you go
        signed_directions_.push_back(pass_choice);

        status = possible_directions_.size() ?
                 I_HAD_OTHER_CHOICES_BUT_I_PICKED_YOU :
                 YOU_ARE_MY_ONLY_CHOICE_SO_DO_NOT_GIVE_IT_BACK_TO_ME;
    }
    else  // well, I'm sure everyone around me has signed
    {
        // well, I've signed, give it back to you
        if (status == I_HAD_OTHER_CHOICES_BUT_I_PICKED_YOU)
        {
            status = YOU_ARE_MY_ONLY_CHOICE_SO_DO_NOT_GIVE_IT_BACK_TO_ME;
            pass_choice = direction;
        }
        else  // "YOU_ARE_MY_ONLY_CHOICE_SO_DO_NOT_GIVE_IT_BACK_TO_ME"
        {
            // roll a dice, roll a dice!!!
            int pass_choice_index = generator() %
                                    (signed_directions_.size() - 1);
            int choice = signed_directions_[pass_choice_index];
            // pass to random one other than the one who give me the sheet
            pass_choice = choice == direction ?
                          signed_directions_[signed_directions_.size() - 1] :
                          choice;
        }
    }
    passed_times_[pass_choice]++;

    // place it here, so pass_to and direction can be a same variable
    pass_to = pass_choice;
    return new_signed;
}

inline int Classroom::Student::passed_times(int direction) const
{
    return direction < 0 || direction >= DIRECTION_NUM ?
           passed_times_[direction] :
           0;  // invalid direction
}


Classroom::Classroom(int row, int col)
        : is_clean_(false),
          row_(row),
          col_(col),
          students_()
{
    // seed the generator
    static std::random_device rd;
    Student::Seed(rd());

    if (row_ < 2)  // row too small
    {
        std::cerr << "row (" << row_ << ") too small! changed it to 2\n";
        row_ = 2;
    }
    if (col_ < 2)  // col too small
    {
        std::cerr << "col (" << col_ << ") too small! changed it to 2\n";
        col_ = 2;
    }

    ArrayType::extent_gen extents;
    students_.resize(extents[row_][col_]);

    ModifySide();
    is_clean_ = true;
}

int Classroom::PassSheet(int start_row, int start_col)
{
    static const int kRowOffset[DIRECTION_NUM] = {-1, 0, 0, 1};
    static const int kColOffset[DIRECTION_NUM] = {0, -1, 1, 0};

    if (start_row < 0 || start_row >= row_ ||
        start_col < 0 || start_col >= col_)
        return -1;  // invalid start point

    if (!is_clean_)
        Clear();

    int pass_count = 0;
    int not_signed_number = row_ * col_ - 1;

    // pass to the first student
    int pass_to = students_[start_row][start_col].GiveSheet();
    is_clean_ = false;  // no longer clean
    int status = 0;  // the fisrt student has at least two choice

    while (not_signed_number != 0)
    {
        pass_count++;
        // generate new row and col
        start_row += kRowOffset[pass_to];
        start_col += kColOffset[pass_to];
        // because of the order of the directions
        int passed_from = DIRECTION_NUM - pass_to - 1;
        // new signed student
        if (students_[start_row][start_col].PassedFrom(passed_from, pass_to,
                                                       status))
            not_signed_number--;
    }
    return pass_count;
}

void Classroom::ModifySide()
{
    students_[0][0] = Student({RIGHT, DOWN});           // upper left
    students_[0][col_ - 1] = Student({LEFT, DOWN});      // upper right
    students_[row_ - 1][0] = Student({UP, RIGHT});       // lower left
    students_[row_ - 1][col_ - 1] = Student({LEFT, UP});  // lower right

    for (IndexType col = 1; col < col_ - 1; col++)
    {
        students_[0][col] = Student({LEFT, DOWN, RIGHT});      // upside
        students_[row_ - 1][col] = Student({LEFT, UP, RIGHT});  // below
    }

    for (IndexType row = 1; row < row_ - 1; row++)
    {
        students_[row][0] = Student({UP, RIGHT, DOWN});       // left side
        students_[row][col_ - 1] = Student({UP, LEFT, DOWN});  // right side
    }
}

void Classroom::Clear()
{
    for (IndexType col = 1; col < col_ - 1; col++)
        for (IndexType row = 1; row < row_ - 1; row++)
        {
            students_[row][col] = Student();
        }
    ModifySide();
    is_clean_ = true;
}

}  // namespace passing_attendance_sheet



#include <stdio.h>

#include "tetris_piece.h"

namespace color
{
    std::uint32_t green  = (0b00001101 << 16) | (0b00000000 << 8) | 0b00000000;
    std::uint32_t yellow = (0b00001101 << 16) | (0b00001101 << 8) | 0b00000000;

} // namespace color


namespace Pattern
{
    enum patterns_state
    {
        PATTERN1,
        PATTERN2,
        PATTERN3,
        PATTERN4,
        NORMAL,
        LAST_PATTERN = NORMAL // explicitly setting the last item
    };
    bool initialized_piece {false};
    // define the frame struct
    struct Tetrispiece
    {
        static constexpr int max_rows {4};
        static constexpr int max_cols {2};
        // initialize these variables
        int id {0};
        int current_row {0};
        int mid_pixel {0};
        int pattern1[max_rows][max_cols] {};
        int pattern2[max_rows][max_cols] {};
        int pattern3[max_rows][max_cols] {};
        int pattern4[max_rows][max_cols] {};
        int normal_pattern[max_rows][max_cols] {};
        std::uint32_t color {0};
        int right_border {0};  // what hits the right border first
        int left_border {0};   // what hits the left border first
        // do not initialize
        patterns_state current_pattern {PATTERN1};
        int (*collection_patterns[5])[4][2] {};
        int pattern_counter {0};
    };

    // a general function to stop pieces from moving.
    // provide an id to identify the specific conditon.
    bool piece_stop_moving(const Tetrispiece &piece)
    {
        switch (piece.id)
        {
        case 1:
            if (piece.current_row >= 0)
            {
                return ((check_Ledplacement(piece.current_row, piece.mid_pixel) ||
                         check_Ledplacement(piece.current_row, piece.mid_pixel+1) ||
                         check_Ledplacement(piece.current_row, piece.mid_pixel-1)));
            }
        case 2:
            if (piece.current_pattern >= 0)
            {
                // current_row depicts the row where the pixel is currently lit at
                // col = 4 is hardcoded for now, should change when piece can move by the player
                return (check_Ledplacement(piece.current_row, piece.mid_pixel));
            }
                
        default:
            return false;
        }
    }

    // declare frames
    // piece1
    static int p1_row {8}; // consider passing this as a variable to functions
                           // instead of global variable
    static int p1_col {4}; // current col middle pixel of piece
    int p1_id {1};
    void initializePiece1(Tetrispiece &piece)
    {
        piece = {
            p1_id,  // id
            p1_row, // current row
            p1_col,
            {
                // pattern1
                {8, 3}, // Row 1
                {8, 4}, // Row 2
                {8, 5}, // Row 3
                {0, 0}  // this will be initialized as 0.. need to rework the
                        // code to only work with + numbers
            },
            {           // pattern2
             {8, 4},
             {7, 3},
             {7, 4},
             {7, 5}},
            {0}, // pattern3
            {0}, // pattern4
            {    // normal
             {p1_row + 1, 4},
             {p1_row, 3},
             {p1_row, 4},
             {p1_row, 5}},
            color::green,
            5,
            3,
        };
        // piece is initialized
        initialized_piece            = true;
        piece.collection_patterns[0] = &piece.pattern1;
        piece.collection_patterns[1] = &piece.pattern2;
        piece.collection_patterns[2] = &piece.pattern3;
        piece.collection_patterns[3] = &piece.pattern4;
        piece.collection_patterns[4] = &piece.normal_pattern;
    }

    // piece2
    static int p2_row {8}; // consider passing this as a variable to functions
                           // instead of global variable
    static int p2_col {4}; // current col middle pixel of piece
    int p2_id {2};
    void initializePiece2(Tetrispiece &piece)
    {
        piece = {
            p2_id,   // id
            p2_row,  // current row
            p2_col,
            {        // pattern1
             {8, 4}, // Row 1
             {0, 0}, // Row 2
             {0, 0}, // Row 3
             {0, 0}},
            {        // pattern2
             {8, 4},
             {7, 4},
             {0, 0},
             {0, 0}},
            {{8, 4}, {7, 4}, {6, 4}, {0, 0}}, // pattern3
            {{8, 4},
             {7, 4},
             {6, 4},
             {5, 4}},                            // pattern4
            {                                 // normal
             {8, 4},
             {7, 4},
             {6, 4},
             {5, 4}},
            color::yellow,
            4,
            4
        };
        // piece is initialized
        initialized_piece            = true;
        piece.collection_patterns[0] = &piece.pattern1;
        piece.collection_patterns[1] = &piece.pattern2;
        piece.collection_patterns[2] = &piece.pattern3;
        piece.collection_patterns[3] = &piece.pattern4;
        piece.collection_patterns[4] = &piece.normal_pattern;
    }
    // constant to keep track of the total amount of availible patterns
    constexpr int patterns_state_count {6}; // including last pattern twice
    // returns the next pattern in order
    patterns_state switch_pattern(Tetrispiece &piece)
    {
        if (piece.current_pattern != LAST_PATTERN)
        {
            bool all_zero {true};
            int index                   = static_cast<int>(piece.current_pattern) + 1;
            int(*current_pattern)[4][2] = piece.collection_patterns[index];

            for (int i = 0; i < piece.max_rows; ++i)
            {
                if ((*current_pattern)[i][0] > 0 || (*current_pattern)[i][1] > 0)
                {
                    all_zero = false;
                }
            }
            // if it is not all 0, meaning there is a next pattern, return that pattern
            if (!all_zero)
            {
                return static_cast<patterns_state>((static_cast<int>(piece.current_pattern) + 1) %
                                                   patterns_state_count);
            }
            // if it is all 0, it means that there are no more unique patterns and it should be set
            // to normal
            else
            {
                return NORMAL;
            }
        }
        else
        {
            return NORMAL;
        }
    }

    void reset_piece1(Tetrispiece &piece)
    {
        initializePiece1(piece);
    }

    void reset_piece2(Tetrispiece &piece)
    {
        initializePiece2(piece);
    }

} // namespace Pattern
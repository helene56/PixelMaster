#ifndef TetrisPatterns 
#define TetrisPatterns

#include <stdio.h>

#include "tetris_piece.h"

namespace color
{

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
    };

    // inline to avoid multiple definitions
    inline bool initialized_piece {false};

    // a general function to stop pieces from moving.
    // provide an id to identify the specific conditon.
    bool piece_stop_moving(const Tetrispiece &piece);

    // declare frames
    // piece1

    void initializePiece1(Tetrispiece &piece);

    // piece2
  
    void initializePiece2(Tetrispiece &piece);

    // returns the next pattern in order
    patterns_state switch_pattern(Tetrispiece &piece);

    void reset_piece1(Tetrispiece &piece);

    void reset_piece2(Tetrispiece &piece);

} // namespace Pattern

#endif // TetrisPatterns
/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 3: fifteen                             |
|     Created by Davy Boekhout on 14/09/2017             |
|                                                        |
|     Implements Game of Fifteen (generalized to d x d)  |
|     whereby the board's dimensions are to be d x d,    |
|     where d must be in [DIM_MIN,DIM_MAX]               |
|                                                        |
|     Note that usleep is obsolete, but it offers more   |
|     granularity than sleep and is simpler to use than  |
|     nanosleep; `man usleep` for more.                  |
|                                                        |
|     Usage: fifteen d                                   |
----------------------------------------------------------
*/

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// solved version of board
int solve[DIM_MAX][DIM_MAX];

// dimensions
int d;

// current position of our blank space (0) "_"
int pos_row;
int pos_col;

// prototypes
void clear(void);
void greet(void);
void init(int size);
void draw(int size);
bool move(int tile);
bool won(int size);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
               DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board of size d
    init(d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(d);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
    // experimenting with colors, below is: bold, black & white
    printf("\033[%d;%d;%dm", 1, 40, 37);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(int size)
{
    // count gives us the starting number and the remaining numbers
    int count = (size * size) - 1;

    // loop over each row and column on the board and fill in the numbers
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            board[row][col] = count;
            count--;
        }
    }

    // swap 1 & 2 in the result if board has even columns/rows
    if (size % 2 == 0)
    {
        board[size - 1][size - 3] = 1;
        board[size - 1][size - 2] = 2;
    }

    // loop over each row and column again, now generating the solved state
    count = 1;
    for (int wor = 0; wor < size; wor++)
    {
        for (int loc = 0; loc < size; loc++)
        {
            solve[wor][loc] = count;
            count++;
        }
    }

    // set the last value of solve to zero
    solve[size - 1][size - 1] = 0;

    // set the starting position of our blank space
    pos_row = size - 1;
    pos_col = size - 1;
}

/**
 * Prints the board in its current state.
 */
void draw(int size)
{
    // loop over the board array and print it on the screen
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            // switch out the zero with an underscore for better visuals
            if (board[row][col] == 0)
            {
                printf(" _");
            }
            // align single digits nicely underneath double digits
            else
            {
                printf("%2i ", board[row][col]);
            }
        }
        // after each row print a new line and a vertical tab
        printf("\n");
        printf("\v");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // declare indexes for left, right, top and bottom
    int lft, rgt, top, bot;

    lft = pos_col - 1;
    rgt = pos_col + 1;
    top = pos_row - 1;
    bot = pos_row + 1;

    board[pos_row][pos_col] = tile;

    // check if the given tile is to the top of the blank space
    if (board[top][pos_col] == tile)
    {
        board[top][pos_col] = 0;
        pos_row = top;
    }
    // check if the given tile is to the bottom of the blank space
    else if (board[bot][pos_col] == tile)
    {
        board[bot][pos_col] = 0;
        pos_row = bot;
    }
    // check if the given tile is to the left of the blank space
    else if (board[pos_row][lft] == tile)
    {
        board[pos_row][lft] = 0;
        pos_col = lft;
    }
    // check if the given tile is to the right of the blank space
    else if (board[pos_row][rgt] == tile)
    {
        board[pos_row][rgt] = 0;
        pos_col = rgt;
    }
    // if no match was found, move is illegal (tile was not adjacent to blank)
    else
    {
        return false;
    }
    return true;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(int size)
{
    // loop over each row and column
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            // if numbers do not match, player didn't win
            if (board[row][col] != solve[row][col])
            {
                return false;
            }
        }
    }
    // numbers match, player won
    return true;
}

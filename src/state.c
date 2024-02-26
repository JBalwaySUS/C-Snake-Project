#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.

  game_state_t* default_state = (game_state_t*) malloc (sizeof(game_state_t));
  default_state->num_rows = 18;

  default_state->board = (char **) malloc (18*sizeof(char*));
  for(int i=0; i<18; i++){
    default_state->board[i] = (char*) malloc (22*sizeof(char));
  }

  strcpy(default_state->board[0], "####################\n");
  strcpy(default_state->board[1], "#                  #\n");
  strcpy(default_state->board[2], "# d>D    *         #\n");
  strcpy(default_state->board[3], "#                  #\n");
  strcpy(default_state->board[4], "#                  #\n");
  strcpy(default_state->board[5], "#                  #\n");
  strcpy(default_state->board[6], "#                  #\n");
  strcpy(default_state->board[7], "#                  #\n");
  strcpy(default_state->board[8], "#                  #\n");
  strcpy(default_state->board[9], "#                  #\n");
  strcpy(default_state->board[10], "#                  #\n");
  strcpy(default_state->board[11], "#                  #\n");
  strcpy(default_state->board[12], "#                  #\n");
  strcpy(default_state->board[13], "#                  #\n");
  strcpy(default_state->board[14], "#                  #\n");
  strcpy(default_state->board[15], "#                  #\n");
  strcpy(default_state->board[16], "#                  #\n");
  strcpy(default_state->board[17], "####################\n");

  default_state->num_snakes = 1;

  default_state->snakes = (snake_t*) malloc (sizeof(snake_t));

  default_state->snakes[0].head_col = 4;
  default_state->snakes[0].head_row = 2;
  default_state->snakes[0].tail_col = 2;
  default_state->snakes[0].tail_row = 2;

  default_state->snakes[0].live = true;

  return default_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  for(int i=0; i<state->num_rows; i++){
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for(int i=0; i<state->num_rows; i++){
    fprintf(fp, "%s", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  if(c == 'w' || c == 'a' || c == 's' || c == 'd')
    return true;
  else
    return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  if(c == 'W' || c == 'S' || c == 'A' || c == 'D' || c == 'x')
    return true;
  else
    return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if(is_tail(c) || c == '^' || c == '<' || c == 'v' || c == '>' || is_head(c))
    return true;
  else
    return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  if(c == '^')
    return 'w';
  else if(c == '<')
    return 'a';
  else if(c == 'v')
    return 's';
  else if(c == '>')
    return 'd';
  else
    return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  if(c == 'W')
    return '^';
  if(c == 'A')
    return '<';
  if(c == 'S')
    return 'v';
  if(c == 'D')
    return '>';
  else
    return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if(c == 'v' || c == 's' || c == 'S')
    return cur_row + 1;
  else if(c == '^' || c == 'w' || c == 'W')
    return cur_row - 1;
  else
    return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if(c == '>' || c == 'd' || c == 'D')
    return cur_col + 1;
  else if(c == '<' || c == 'a' || c == 'A')
    return cur_col - 1;
  else
    return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int x = state->snakes[snum].head_row;
  unsigned int y = state->snakes[snum].head_col;
  char c = state->board[x][y];
  if(is_snake(c))
    return state->board[get_next_row(x, c)][get_next_col(y, c)];
  else
    return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int x = state->snakes[snum].head_row;
  unsigned int y = state->snakes[snum].head_col;
  char c = state->board[x][y];

  unsigned int upd_x = get_next_row(x, c);
  unsigned int upd_y = get_next_col(y, c);
  char upd_c = head_to_body(c);

  state->board[upd_x][upd_y] = c;
  state->board[x][y] = upd_c;

  state->snakes[snum].head_row = upd_x;
  state->snakes[snum].head_col = upd_y;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int x = state->snakes[snum].tail_row;
  unsigned int y = state->snakes[snum].tail_col;
  char c = state->board[x][y];

  unsigned int upd_x = get_next_row(x, c);
  unsigned int upd_y = get_next_col(y, c);
  char upd_c = state->board[upd_x][upd_y];

  state->board[upd_x][upd_y] = body_to_tail(upd_c);
  state->board[x][y] = ' ';

  state->snakes[snum].tail_row = upd_x;
  state->snakes[snum].tail_col = upd_y;
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  for(unsigned int i=0; i<state->num_snakes; i++){
    unsigned int x = state->snakes[i].head_row;
    unsigned int y = state->snakes[i].head_col;
    char c = state->board[x][y];

    unsigned int nex_x = get_next_row(x, c);
    unsigned int nex_y = get_next_col(y, c);
    char nex_c = state->board[nex_x][nex_y];

    if(nex_c == '*'){
      update_head(state, i);
      add_food(state);
    }
    else if(is_snake(nex_c) || nex_c == '#'){
      state->board[x][y] = 'x';
      state->snakes[i].live = false;
    }
    else{
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  // TODO: Implement this function.
  game_state_t* load_state; 
  
  load_state = (game_state_t*) malloc (sizeof(game_state_t));

  load_state->num_snakes = 0;
  load_state->snakes = NULL;

  load_state->board = (char**) malloc (sizeof(char*));
  load_state->board[0] = (char*) malloc (sizeof(char));

  char c;
  unsigned int i = 0;
  unsigned int j = 0;
  while((c = (char) fgetc(fp)) != EOF){
    if(c == '\n'){
      load_state->board[i][j] = '\n';
      j++;
      load_state->board[i] = (char*) realloc (load_state->board[i], (j+1)*sizeof(char));
      load_state->board[i][j] = '\0';
      i++;
      j = 0;
      load_state->board = (char**) realloc (load_state->board, (i+1)*sizeof(char*));
      load_state->board[i] = (char*) malloc (sizeof(char));
    }
    else{
      load_state->board[i][j] = c;
      j++;
      load_state->board[i] = (char*) realloc (load_state->board[i], (j+1)*sizeof(char));
    }
  }

  load_state->num_rows = i;

  return load_state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int x = state->snakes[snum].tail_row;
  unsigned int y = state->snakes[snum].tail_col;
  char c = state->board[x][y];

  while(!is_head(c)){
    x = get_next_row(x, c);
    y = get_next_col(y, c);
    c = get_board_at(state, x, y);
  }

  state->snakes[snum].head_row = x;
  state->snakes[snum].head_col = y;

  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  state->num_snakes = 0;
  state->snakes = (snake_t*) malloc (sizeof(snake_t));

  for(unsigned int i=0; i<state->num_rows; i++){
    for(unsigned int j=0; j<strlen(state->board[i]); j++){
      if(is_tail(get_board_at(state, i, j))){
        state->snakes = (snake_t*) realloc (state->snakes, (state->num_snakes+1) * sizeof(snake_t));
        state->snakes[state->num_snakes].tail_row = i;
        state->snakes[state->num_snakes].tail_col = j;
        find_head(state, state->num_snakes);
        if(get_board_at(state, state->snakes[state->num_snakes].head_row, state->snakes[state->num_snakes].head_col) == 'x'){
          state->snakes[state->num_snakes].live = false;
        }
        else{
          state->snakes[state->num_snakes].live = true;
        }
        state->num_snakes++;
      }
    }
  }
  
  return state;
}

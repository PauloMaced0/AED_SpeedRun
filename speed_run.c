//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. XXXXXX  Name: XXXXXXX
//


//
// static configuration
//

#define _max_road_size_  800  // the maximum problem size
#define _min_road_speed_   2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_   9  // must not be larger than 9 (only because of the PDF figure)

//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "elapsed_time.h"
#include "make_custom_pdf.c"
#include <assert.h>

//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for(i = 0;i <= _max_road_size_;i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
    if(max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if(max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}


//
// description of a solution
//

typedef struct
{
  int n_moves;                         // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_];  // the positions (the first one must be zero)
}
solution_t;

static solution_t solution_1,solution_1_best,solution,best_solution;
static double solution_1_elapsed_time,solution_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count,solution_count; // effort dispended solving the problem

int veryNicefunction(int move_number,int position,int speed,int final_position){
  // check if is an available jump 
  if((speed * (speed - 1)) / 2 > final_position - position || max_road_speed[position] < speed) return 0;
  for(int p = position + 1 - speed;p < position ;p++)
    if(max_road_speed[p] < speed) return 0;
  //
  // record move
  solution_count++;
  solution.positions[move_number] = position;
  //
  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < best_solution.n_moves)
    {
      best_solution= solution;
      best_solution.n_moves = move_number;
    }
    return 1;
  }
  if(veryNicefunction(move_number + 1,position + speed + 1,speed + 1,final_position)) return 1;
  if(veryNicefunction(move_number + 1,position + speed,speed,final_position)) return 1;
  if(veryNicefunction(move_number + 1,position + speed - 1,speed - 1,final_position)) return 1;
  return 0;
}
// Given method to solve the problem
static void solution_1_recursion(int move_number,int position,int speed,int final_position)
{
  int i,new_speed;

  // record move
  solution_1_count++;
  solution_1.positions[move_number] = position;
  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_1_best.n_moves)
    {
      solution_1_best = solution_1;
      solution_1_best.n_moves = move_number;
    }
    return;
  }
  // no, try all legal speeds
  for(new_speed = speed - 1;new_speed <= speed + 1;new_speed++)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++)
        ;
      if(i > new_speed)
        solution_1_recursion(move_number + 1,position + new_speed,new_speed,final_position);
    }
}
void solveWithveryNicefunction(int final_position){
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solveVeryNiceFunction: bad final_position\n");
    exit(1);
  }
  solution_elapsed_time = cpu_time();
  solution_count = 0ul;
  best_solution.n_moves = final_position + 100;
  veryNicefunction(0,0,0,final_position);
  solution_elapsed_time = cpu_time() - solution_elapsed_time;
  
}

static void solve_1(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }
  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;
  solution_1_best.n_moves = final_position + 100;
  solution_1_recursion(0,0,0,final_position);
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}


//
// example 
//

static void example(void)
{
  int i,final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;
  solve_1(final_position);
  make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
  printf("mad road speeds:");
  for(i = 0;i <= final_position;i++)
    printf(" %d",max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for(i = 0;i <= solution_1_best.n_moves;i++)
    printf(" %d",solution_1_best.positions[i]);
  printf("\n");
}


//
// main program
//

int main(int argc,char *argv[argc + 1])
{
#ifndef RUN_SOLUTION
#define RUN_SOLUTION 1 // To run first solution: #define SOLUTION 0
  FILE *fp1;           // To run second solution: # define SOLUTION 1 
  #if RUN_SOLUTION > 0
  fp1 = fopen("solutions_executionTime/solution.txt", "w");
  #endif
  #if RUN_SOLUTION == 0
  FILE *fp2;
  fp2 = fopen("solutions_executionTime/bad_solution.txt", "w");
  #endif
# define _time_limit_  3600.0
  int n_mec,final_position;
  char file_name[64];

  // generate the example data
  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    example();
    return 0;
  }
  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");
  while(final_position <= _max_road_size_/* && final_position <= 20*/)
  {
    #if RUN_SOLUTION == 0
    int print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |",final_position); 
    fprintf(fp2,"%3d\t",final_position);

    // first solution method (very bad)
    if(solution_1_elapsed_time < _time_limit_)
    {
      solve_1(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_1.pdf",final_position);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
      }
      printf(" %3d %16lu %9.3e |",solution_1_best.n_moves,solution_1_count,solution_1_elapsed_time);
      fprintf(fp2,"%4d\t%16lu\t%9.3e", solution_1_best.n_moves,solution_1_count, solution_1_elapsed_time);
    }
    else
    {
      solution_1_best.n_moves = -1;
      printf("                                |");
    }
    printf("\n");
    fprintf(fp2,"\n");
    #endif
    //
    //
    // second solution method (less bad)
    //
    //
    #if RUN_SOLUTION > 0
    int print= (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |",final_position);
    fprintf(fp1,"%3d\t",final_position);

    if(solution_elapsed_time < _time_limit_)
    {
      solveWithveryNicefunction(final_position);
      if(print!= 0)
      {
        sprintf(file_name,"%03d_solution.pdf",final_position);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],best_solution.n_moves,&best_solution.positions[0],solution_elapsed_time,solution_count,"Alternative method");
      }
      printf(" %3d %16lu %9.3e |", best_solution.n_moves, solution_count, solution_elapsed_time);
      fprintf(fp1,"%3d\t%16lu\t%9.3e", best_solution.n_moves,solution_count, solution_elapsed_time);
    }
    else
    {
      best_solution.n_moves = -1;
      printf("                                |");
    }

    // done
    printf("\n");
    fprintf(fp1,"\n");
    #endif
    #endif
    fflush(stdout);

    // new final_position
    if(final_position < 50)
      final_position += 1;
    else if(final_position < 100)
      final_position += 5;
    else if(final_position < 200)
      final_position += 10;
    else if(final_position < 1000)
      final_position += 20;
    else 
      final_position += 5000;
  }
  printf("--- + --- ---------------- --------- +\n");
  return 0;
# undef _time_limit_
}

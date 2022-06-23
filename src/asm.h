#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONTAINERS_FULL    "../data/containers_full.csv"
#define CONTAINERS_DOUBLE  "../data/containers_double.csv"
#define CONTAINERS_SINGLE  "../data/containers_single.csv"
#define DATA_CONTAINERS    "../data/data_containers.csv"

#define RED(str)     "\x1b[31m" str "\x1b[0m"
#define GREEN(str)   "\x1b[32m" str "\x1b[0m"
#define YELLOW(str)  "\x1b[33m" str "\x1b[0m"

typedef struct {
	unsigned short id;  //Byte 1
	short x;            //Byte 1
	short y;            //Byte 1
	short z;            //Byte 1
	float length;       //Byte 2
	float width;        //Byte 2
	float height;       //Byte 3
	short type;         //Byte 3
	short f_mat;        //Byte 3
	short s_mat;        //Byte 4
	short t_mat;        //Byte 4
	float f_depth;      //Byte 4
	float s_depth;      //Byte 5
	float t_depth;      //Byte 5
} s_container;

typedef struct {
	s_container* container_array;
	int used;
	int max;
} s_container_arr;

#define X 3
#define Y 3
#define Z 3

#define APP_CLEAR \
"\n\n\n\n\n\n\n\n\n\n" \
"\n\n\n\n\n\n\n\n\n\n" \
"\n\n\n\n\n\n\n\n\n\n" \
"\n\n\n\n\n\n\n\n\n\n"

#define MENU_STR \
"#===============================#\n" \
" #                             #\n" \
" #   1. Load CSV (Full)        #\n" \
" #   2. Load CSV (Double)      #\n" \
" #   3. Load CSV (Single)      #\n" \
" #                             #\n" \
" #   4. Print Container Data   #\n" \
" #   5. Check Refrigerated     #\n" \
" #   6. Change Power Output    #\n" \
" #   7. Power Log Switch       #\n" \
" #   8. Clear Data             #\n" \
" #                             #\n" \
" #   0. Close application      #\n" \
" #                             #\n" \
"#===============================#\n\n"

//C Functions
s_container_arr* alloc_struct();
int count_lines();
void container_parse();
void container_data_parse();
void reset_array();
int alloc_array();
void print_containers();
void calculate_power_consumption();
int getContainerIndexFromID();
float getMaterialResistanceFromID();
void checkContainerRefrigerated();
void alterPowerOutput();

//Assembly Functions
int check_type(s_container* container_array, int index);

#endif

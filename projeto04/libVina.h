#ifndef __VINA
#define __VINA

#include <stdio.h>

void insert_operation(FILE *archiver, char **argv, int members_quantity);

void update_file(FILE *archiver, char *filename);

void move_file(FILE *archiver, char *filename);

void extract_file(FILE *archiver, char *filename);

void remove_file(FILE *archiver, char *filename);

void list_files(FILE *archiver);

void help_utility(char *call);

#endif

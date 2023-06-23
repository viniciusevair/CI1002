#ifndef __VINA
#define __VINA

#include <stdio.h>

void insert_operation(FILE *archiver, char **argv, int members_quantity);

void update_operation(FILE *archiver, char **argv, int members_quantity);

void move_operation(FILE *archiver, char **argv, int members_quantity);

int extract_operation(FILE *archiver, char **argv, int members_quantity);

void remove_operation(FILE *archiver, char **argv, int members_quantity);

void list_files(FILE *archiver);

void help_utility(char *call);

#endif

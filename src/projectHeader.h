#ifndef PROJECTHEADER_H_   /* Include guard */
#define PROJECTHEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>

#define LISTA_OPCOES "HL:S:D:P:i:o:"

typedef struct country{

    char country[30];
    char country_ID[4];
    char continent[10];
    int population;
    struct data *nextD;
    struct country *nextC;

} country;

typedef struct data{

    char indicator[8];
    int weekly_count;
    char year_week[8];
    float rate_14_day;
    int cumulative_count;
    struct data* next;
} data;


data *find_data_tail(country *);
data *search_order(country *, bool, bool, char*);
country *cases_order(country *, bool, bool,bool, bool, char*);
country *create_country(bool, bool, char *, char *, FILE*);
country *create_country_dat(FILE *);
country *check_country(country *, char *);
country *troca(country *, country *);

/*=====funções da The GNU C Library============*/
char *strsep(char **stringp, const char *delim);
char *my_strdup (char *s);
/*=============================================*/

bool order_manager(country *, bool, bool,bool, bool, char*);
bool check_remove(bool, bool, char *, char *, char *);
bool restriction_manager(country *, bool, bool, int);
bool restriction_data_manager(data *, bool, bool, char*, char*);

void insert_end_list(country *, country *);
void token_check(char *, country*, FILE*);
void print_list(country *);
void delete_data_ratio(country *);
void delete_data_cd(country *);
void restriction(country **, bool, bool, int);
void delete_all_data(country *);
void restriction_data(country **, bool, bool, char*, char*);
void free_list(country *);
void list_modes(char *, char *, char **, char *, char *, char **);
void verify(char *, char *, char **, char *, char *, char **);
void printFile_csv(country *, char*);
void printFile_dat(country *, char*);
void usage();
#endif










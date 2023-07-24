#include "projectHeader.h"
/**
 * @brief função que reproduz o ficheiro em formato csv, escreve os dados da lista, após (1) Ler os dados; (2) Selecionar; (3) Restringir; (4) Ordenar
 *        de acordo com as necessidades do utilizador
 * 
 * @param listHead country* apontador para a "cabeça" da lista de listas a ser passada para o ficheiro
 * @param file FILE* nome do ficheiro a ser reproduzido
 */
void printFile_csv(country *listHead, char *file)
{
    FILE *fptr;
    country *current_c;
    data *current_d;

    if ((fptr = fopen(file, "w")) == NULL)
    {
        printf("Erro: Problema na abertura do ficheiro\n");
        usage();
        exit(EXIT_FAILURE);
    }

    fprintf(fptr, "country,country_code,continent,population,indicator,weekly_count,year_week,rate_14_day,cumulative_count\n");

    for (current_c = listHead; current_c != NULL; current_c = current_c->nextC)
    {

        for (current_d = current_c->nextD; current_d != NULL; current_d = current_d->next)
        {
            fprintf(fptr, "%s,%s,%s,%d,", current_c->country, current_c->country_ID, current_c->continent, current_c->population);
            fprintf(fptr, "%s,%d,%s,%f,%d\n", current_d->indicator, current_d->weekly_count, current_d->year_week, current_d->rate_14_day, current_d->cumulative_count);
        }
    }
    fclose(fptr);
}
/**
 * @brief função que reproduz o ficheiro em formato dat, escreve os dados da lista, após (1) Ler os dados; (2) Selecionar; (3) Restringir; (4) Ordenar
 *        de acordo com as necessidades do utilizador
 * 
 * @param listHead country* apontador para a "cabeça" da lista de listas a ser passada para o ficheiro
 * @param file FILE* nome do ficheiro a ser reproduzido
 */
void printFile_dat(country *listHead, char *file)
{
    FILE *fptr;
    country *current_c;
    data *current_d;
    int n = 0;

    if ((fptr = fopen(file, "wb")) == NULL)
    {
        printf("Erro: Problema na abertura do ficheiro");
        usage();
        exit(EXIT_FAILURE);
    }
    for (current_c = listHead; current_c != NULL; current_c = current_c->nextC)
    {
        n++;
    }

    fwrite(&n, sizeof(n), 1, fptr);

    for (current_c = listHead; current_c != NULL; current_c = current_c->nextC)
    {
        fwrite(&current_c->country, sizeof(current_c->country), 1, fptr);
        fwrite(&current_c->country_ID, sizeof(current_c->country_ID), 1, fptr);
        fwrite(&current_c->continent, sizeof(current_c->continent), 1, fptr);
        fwrite(&current_c->population, sizeof(current_c->population), 1, fptr);

        for (current_d = current_c->nextD; current_d != NULL; current_d = current_d->next)
        {
            fwrite(&current_d->indicator, sizeof(current_d->indicator), 1, fptr);
            fwrite(&current_d->weekly_count, sizeof(current_d->weekly_count), 1, fptr);
            fwrite(&current_d->year_week, sizeof(current_d->year_week), 1, fptr);
            fwrite(&current_d->rate_14_day, sizeof(current_d->rate_14_day), 1, fptr);
            fwrite(&current_d->cumulative_count, sizeof(current_d->cumulative_count), 1, fptr);
            fwrite(&current_d->next, sizeof(current_d->next), 1, fptr);
        }
    }
    fclose(fptr);
}
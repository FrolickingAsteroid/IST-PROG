#include "projectHeader.h"
/**
 * @brief Função principal de organização dos modos de tratamento de dados, recebe os parâmetros introduzidos pelo utilizador e organiza-os de
 *        forma a reproduzir o output pretendido
 *
 * @param read char* apontador para o parâmetro -L
 * @param select char* apontador para o parâmetro -D
 * @param restri char** apontador para o parâmetro -P
 * @param in char* apontador para o parâmetro -i
 * @param out char* apontador para o parâmetro -o
 * @param order char** apontador para o parâmetro -S
 */
void list_modes(char *read, char *select, char **restri, char *in, char *out, char **order)
{
    country *listHead = NULL;
    FILE *file_in = NULL;

    char *indicator = "NULL";
    bool checkcontinent = false, checkindicator = false, popup = false, popdown = false, dat = false; /*controlo da restrição e da criação da lista*/
    bool two_dates = false, one_date = false, alpha = true, interference = false, popul = false, cases = false, deaths = false; /*controlo da ordenação e restrição*/
    int n = 0, t = 0, length = 0;
/*=========================== escrita da lista =========================================================================================*/
    if (select != NULL)/*parte da seleção, se assim especificado, é iniciada na construção da lista, são removidas linhas com indicators diferentes dos pedidos*/
    {
        if (strstr(select, "inf"))
        {
            indicator = "cases";
            checkindicator = true;
        }
        else
        {
            indicator = "deaths";
            checkindicator = true;
        }
    }
    length = strlen(in);

    if (strstr(read, "all")==NULL)
    {
        checkcontinent = true;
    }

    if (in[length-4] == '.' && in[length-3]== 'c' && in[length-2] == 's' && in[length-1] == 'v')
    {
        if ((file_in = fopen(in, "r")) == NULL)
        {
            printf("Erro: Problema na abertura do ficheiro\n");
            exit(EXIT_FAILURE);
        }
        listHead = create_country(checkcontinent, checkindicator, read, indicator, file_in);

        fclose(file_in);
    }
    else if (in[length-4] == '.' && in[length-3]== 'd' && in[length-2] == 'a' && in[length-1] == 't')
    {
        dat = true; /*verifica se o ficheiro é do tipo .dat, de forma a não serem executados nenhums algoritmos de ordenção*/
        if ((file_in = fopen(in, "rb")) == NULL)
        {
            printf("Erro: Problema na abertura do ficheiro\n");
            exit(EXIT_FAILURE);
        }
        listHead = create_country_dat(file_in);

        fclose(file_in);
    }
    else
    {
        fprintf(stderr, "> formato de ficheiro inválido.\n");
        usage();
        exit(EXIT_FAILURE);
    }
/*=========================== realização dos algoritmos de seleção =========================================================================================*/
    if (select != NULL)
    {
        if (strstr(select, "racio"))
        {
            delete_data_ratio(listHead);
        }
        else
        {
            delete_data_cd(listHead);
        }
    }
/*=========================== realização dos algoritmos de restrição =========================================================================================*/
    if (restri[0] != NULL)
    {
        if (strstr(restri[0], "min") != NULL)
        {
            popup = true;
            n = atoi(restri[1]);
            restriction(&listHead, popup, popdown, n);
        }
        else if (strstr(restri[0], "max") != NULL)
        {
            popdown = true;
            n = atoi(restri[1]);
            restriction(&listHead, popup, popdown, n);
        }

        if (strstr(restri[0], "date"))
        {
            if (strcmp(restri[0], "date") == 0)
            {
                one_date = true;
            }
            else
            {
                two_dates = true;
            }
            restriction_data(&listHead, one_date, two_dates, restri[1], restri[2]);
        }
    }
/*=========================== realização dos algoritmos de ordenação =========================================================================================*/
    if (strstr(order[0], "alfa") == NULL && dat == false)
    {
        listHead = cases_order(listHead, alpha, cases, deaths, popul, order[1]); /*a ordenação alfabética é sempre realizada para que em locais onde o algoritmo não toque, fiquem automaticamente por ordem alfabética*/

        if (select != NULL && ((strstr(select, "inf") != NULL && strstr(order[0], "dea") != NULL) || (strstr(select, "dea") != NULL && strstr(order[0], "inf") != NULL)))
        {
            interference = true;
        }

        if (restri[0] != NULL)
        {
            if (order[1] != NULL)
            {
                if (one_date == true && strcmp(restri[1], order[1]) != 0)
                {
                    interference = true;
                }
                if (two_dates == true)
                {
                    t = strcmp(restri[1], restri[2]);
                    if (t < 0 && (strcmp(order[1], restri[1]) < 0 || strcmp(order[1], restri[2]) > 0))
                    {
                        interference = true;
                    }
                    if (t == 0 && (strcmp(order[1], restri[1]) != 0))
                    {
                        interference = true;
                    }
                    if (t > 0 && (strcmp(order[1], restri[2]) < 0 || strcmp(order[1], restri[1]) > 0))
                    {
                        interference = true;
                    }
                }
            }
        }
        if (strstr(order[0], "inf") != NULL && interference != true)
        {
            cases = true, alpha = false;
            listHead = cases_order(listHead, alpha, cases, deaths, popul, order[1]);
        }
        else if (strstr(order[0], "dea") != NULL && interference != true)
        {
            deaths = true, alpha = false;
            listHead = cases_order(listHead, alpha, cases, deaths, popul, order[1]);
        }

        if (strstr(order[0], "pop") != NULL)
        {
            alpha = false, popul = true;
            listHead = cases_order(listHead, alpha, cases, deaths, popul, order[1]);
        }
    }
    else if (dat == false)
    {
        listHead = cases_order(listHead, alpha, cases, deaths, popul, order[1]);
    }
/*=========================== criação do novo ficheiro =========================================================================================*/
    length = strlen(out);
    if (out[length-4] == '.' && out[length-3]== 'c' && out[length-2] == 's' && out[length-1] == 'v')
    {
        printFile_csv(listHead, out);
    }
    else if (out[length-4] == '.' && out[length-3]== 'd' && out[length-2] == 'a' && out[length-1] == 't')
    {
        printFile_dat(listHead, out);
    }
    else
    {
        fprintf(stderr, "> formato de ficheiro inválido.\n");
        usage();
        exit(EXIT_FAILURE);
    }
/*=========================== libertação dos dados da lista =========================================================================================*/
    free_list(listHead);
}

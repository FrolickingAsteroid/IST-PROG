#include "projectHeader.h"
/**
 * @brief Função que realiza a restrição relativa à população, localiza os nós que não cumprem a restrição e liberta-os através das funções free
 *        e delete_all_data
 * 
 * @param listhead country* ponteiro para a lista de listas a ser analisada
 * @param popup bool controlo da restrição min
 * @param popdown bool controlo da restrição max
 * @param n int número da população (em milhares) a ser analisada
 */
void restriction(country **listhead,bool popup,bool popdown, int n)
{
    country *curr = NULL, *prev = NULL, *save = NULL;
    curr = *listhead;

    while (curr)
    {
        if (restriction_manager(curr, popup, popdown,n))
        {
            if (curr == *listhead)
            {
                save = curr;
                *listhead = curr->nextC;
                delete_all_data(save);
                free(save);

                curr = (*listhead);
            }
            else
            {
                save = curr;
                prev->nextC = curr->nextC;
                delete_all_data(save);
                free(save);
                curr = prev;

                curr = curr->nextC;
            }
        }
        else
        {
            prev = curr;
            curr = curr->nextC;
        }
    }
}
/**
 * @brief função que cambia entre os dois parâmetros, max e min, de acordo com as especificações do utilizador, conclui se o nó deve ou não ser eliminado 
 * 
 * @param curr country* nó do país a ser analisado
 * @param popup bool controlo da restrição min
 * @param popdown bool controlo da restrição max
 * @param n int número da população (em milhares) a ser analisada
 * @return true se o nó for para ser eliminado
 * @return false se o nó se mantiver
 */
bool restriction_manager(country *curr,bool popup,bool popdown, int n ) 
{
    if (popup == true && curr->population <= n*1000)
    {
        
        return true;
    }

    if (popdown == true && curr->population >= n*1000)
    {
        return true;
    }
    return false;
}
/**
 * @brief Função que realiza a restrição relativa às datas localiza os nós que não cumprem a restrição e liberta-os através das funções free
 *        se o país não apresentar, de facto, quaisquer dados dentro das datas ou data especificada(s) elimina o país 
 * 
 * @param listHead country* ponteiro para a lista de listas a ser analisada
 * @param one_date bool controlo para somente uma data especificada
 * @param two_dates bool controlo para duas data especificadas
 * @param data1 char* data a ser analisada
 * @param data2 char* data a ser analisada ( se two_dates = true)
 */
void restriction_data(country **listHead, bool one_date, bool two_dates, char *data1, char *data2)
{
    country *currC = NULL, *prev_c = NULL, *saveC = NULL;
    data *current = NULL, *prev = NULL, *save = NULL;

    currC = *listHead;
    
    while (currC)
    {
        current = currC->nextD;
        while (current)
        {
            if (restriction_data_manager(current, one_date, two_dates, data1, data2))
            {
                if (current == currC->nextD)
                {
                    save = current;
                    currC->nextD = current->next;
                    free(save);
                    current = currC->nextD;
                }
                else
                {
                    save = current;
                    prev->next = current->next;
                    free(save);
                    current = prev;

                    current = current->next;
                }
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }

        if (currC->nextD == NULL)/*se todas os nós da linha verical forem eliminados, não há dados relativos à data especificada, o país deve então ser eliminado*/
        {
            if (currC == *listHead)
            {
                saveC = currC;
                *listHead = currC->nextC;
                free(saveC);

                currC = (*listHead);
            }
            else
            {
                saveC = currC;
                prev_c->nextC = currC->nextC;
                free(saveC);
                currC = prev_c;

                currC = currC->nextC;
            }
        }
        else
        {
            prev_c = currC;
            currC = currC->nextC;
        }
    }
}
/**
 * @brief função que cambia entre os dois parâmetros, date e dates, de acordo com as especificações do utilizador, conclui se o nó deve ou não ser eliminado 
 * 
 * @param current data* nó da linha vertical a ser analisado
 * @param one_date bool controlo para somente uma data especificada
 * @param two_dates bool controlo para duas data especificadas
 * @param data1 char* data a ser analisada
 * @param data2 char* data a ser analisada ( se two_dates = true)
 * @return true implementação semelhate à função restriction_manager = se o nó for para ser eliminado
 * @return false false se o nó se mantiver
 */
bool restriction_data_manager(data *current, bool one_date, bool two_dates, char *data1, char *data2)
{
    int t = 0;
    if (one_date == true && strcmp(current->year_week, data1) != 0)
    {
        return true;
    }

    if (two_dates == true)
    {
        t = strcmp(data1, data2);
        if (t == 0 && strcmp(current->year_week, data1) != 0)
        {
            return true;
        }

        if (t < 0 && (strcmp(current->year_week, data1) < 0 || strcmp(current->year_week, data2) > 0))
        {
            return true;
        }

        if (t > 0 && (strcmp(current->year_week, data1) > 0 || strcmp(current->year_week, data2) < 0))
        {
            return true;
        }
    }
    return false;
}
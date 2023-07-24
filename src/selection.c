#include "projectHeader.h"
/**
 * @brief função de seleção referente às opções de rácio (racioinf e raciodea), dipõe na lista apenas a linha com o maior rácio para cada país
 * 
 * @param listHead country* ponteiro para a "cabeça" da lista de listas onde será realizada a seleção
 */
void delete_data_ratio(country *listHead)
{
    country *currC = NULL;
    data *current = NULL, *save = NULL;

    for (currC = listHead; currC != NULL; currC = currC->nextC)
    {
        current = currC->nextD; /*ponteiro para a lista vertical é atualizado sempre que se muda de nó na lista horizontal*/

        while (current->next != NULL)
        {
            if (current->rate_14_day < current->next->rate_14_day)/*se o primeiro for menor que o segundo, o primeiro é removido e o segundo torna-se o primeiro*/
            {
                currC->nextD = current->next;
                free(current);
                current = currC->nextD;
            }
            else if (current->rate_14_day > current->next->rate_14_day)/*se o primeiro for maior que o segundo, o primeiro é ligado ao terceiro e o segundo removido*/
            {
                save = current->next;
                current->next = save->next;
                free(save);
            }
            else if (current->rate_14_day == current->next->rate_14_day)/*se os valores forem idênticos o que permanece terá a semana mais recente, com uma implementação semelhante ao realizado anteriormente*/
            {
                if (strcmp(current->year_week, current->next->year_week) < 0)
                {
                    currC->nextD = current->next;
                    free(current);
                    current = currC->nextD;
                }
                else
                {
                    save = current->next;
                    current->next = save->next;
                    free(save);
                }
            }
        }
    }
}
/**
 * @brief função de seleção referente às opções de cases/deaths, dipõe na lista apenas a linha com o maior número de infetados ou mortes para cada país
 *        em tudo semelhante à anterior.
 * 
 * @param listHead country* ponteiro para a "cabeça" da lista de listas onde será realizada a seleção
 */
void delete_data_cd(country *listHead)
{
    country *currC = NULL;
    data *current = NULL, *save = NULL;

    for (currC = listHead; currC != NULL; currC = currC->nextC)
    {
        current = currC->nextD;

        while (current->next != NULL)
        {
            if (current->weekly_count < current->next->weekly_count)
            {
                currC->nextD = current->next;
                free(current);
                current = currC->nextD;
            }
            else if (current->weekly_count > current->next->weekly_count)
            {
                save = current->next;
                current->next = save->next;
                free(save);
            }
            else if (current->weekly_count == current->next->weekly_count)
            {
                if (strcmp(current->year_week, current->next->year_week) < 0)
                {
                    currC->nextD = current->next;
                    free(current);
                    current = currC->nextD;
                }
                else
                {
                    save = current->next;
                    current->next = save->next;
                    free(save);
                }
            }
        }
    }
}
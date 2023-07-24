#include "projectHeader.h"
/**
 * @brief função que liberta a lista principal, faz uso da função delete_all_data, que liberta a lista vertical de cada nó da horizontal
 * 
 * @param listHead ponteiro para a "cabeça" da lista de listas
 */
void free_list(country *listHead)
{
    country *save;
    save = listHead;

    while(listHead != NULL)
    {
        save = listHead;
        listHead = listHead -> nextC;
        delete_all_data(save);
        free(save);
    }
}
/**
 * @brief função auxiliar que liberta a lista vertical de cada nó da horizontal
 * 
 * @param current ponteiro para o nó da lista horizontal de onde a lista vertical será eliminada
 */
void delete_all_data(country *current)
{
    data *save;
    data *curr = current->nextD;

    while (curr != NULL)
    {
        save = curr;
        curr = curr->next;
        free(save);
    }

    current->nextD = NULL;
}
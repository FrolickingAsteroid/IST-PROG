#include "projectHeader.h"

/**
 * @brief a função ordena por ordem decrescente de número de casos, mortes e população e é ainda utilizada para a realização da ordenação alfabetica através do algoritmo Bubble sort
 * 
 * @param ListHead country* apontador para o inicio da lista
 * @param alpha bool parametro que permite escolher a ordenação alfabetica
 * @param cases bool  parametro que permite escolher a ordenação por casos
 * @param deaths bool  parametro que permite escolher a ordenação por mortes
 * @param popul bool  parametro que permite escolher a ordenação por população
 * @param data char* apontador para a data a ser analizada
 * @return country* retorna a cabeça da lista após a ordenação
 */
country *cases_order(country *ListHead, bool alpha, bool cases, bool deaths, bool popul, char* data)
    {
        int trocas = 1;
        country *left, *right, *head, aux_struct;

        head = &aux_struct;
        head->nextC = ListHead;

        if ((ListHead != NULL) && ((ListHead->nextC != NULL)))
        {

            while (trocas)
            {
                trocas = 0;
                left = head;
                right = head->nextC;
                while (right->nextC != NULL)
                {
                    if (order_manager(right, alpha, cases, deaths, popul, data))  //chama a função de ordenação
                    {
                        left->nextC = troca(right, right->nextC);
                        trocas = 1;
                    }
                    left = right;
                    if (right->nextC != NULL)
                        right = right->nextC;
                }
            }
        }
        right = head->nextC;
        return right;
    } 
/**
 * @brief a função é responsavel por trocar dois parametros das listas 
 * 
 * @param left country* ponteiro auxiliar para a realização da troca (explicado no relatório)
 * @param right country* ponteiro auxiliar para a realização da troca (explicado no relatório)
 * @return country* retorna a cabeça da lista após realizar a troca 
 */
 
    country *troca(country *left, country *right)
    {
        left->nextC = right->nextC;
        right->nextC = left;
        return right;
    }
/**
 * @brief a função é responsável por indicar se é necessário ou não a realização de uma troca de parametros
 * 
 * @param right country* apontador para os vários blocos da lista 
 * @param alpha bool parametro que permite escolher a ordenação alfabetica
 * @param cases bool parametro que permite escolher a ordenação por casos
 * @param deaths bool parametro que permite escolher a ordenação por mortes
 * @param popul bool parametro que permite escolher a ordenação por população
 * @param data1 char* apontador para a data a ser analizada
 * @return true retorna o valor de 1
 * @return false retorna o valor de 0 
 */
 bool order_manager(country *right, bool alpha, bool cases, bool deaths, bool popul, char *data1)
    {
        data *current = NULL, *next_curr;
        country *next = right->nextC;

        if ((deaths == true || cases == true))
        {
            current = search_order(right, cases, deaths, data1);
            next_curr = search_order(next, cases, deaths, data1);
            if ((current == NULL && next_curr == NULL) || (current != NULL && next_curr == NULL)) // lista sem parametros ou lista anterior com parametros e seguinte sem parametros 
            {
                return false;
            }
            if (current == NULL && next_curr != NULL)
            {
                return true;
            }
            if (current->weekly_count < next_curr->weekly_count)
            {
                return true;
            }
        }

        if ((right->population < right->nextC->population) && popul == true)
        {
            return true;
        }

        if ((strcmp(right->country, right->nextC->country) > 0) && alpha == true)
        {
            return true;
        }

        return false;
    }
/**
 * @brief a função avalia se os parametros pedidos se encontram numa determinada lista, em caso afimativo é retornado um apontador 
 *        para a lista em questão, em caso negativo é retornado NULL
 * 
 * @param right country* apontador para os vários blocos da lista 
 * @param cases bool parametro que permite escolher a ordenação por casos
 * @param deaths bool parametro que permite escolher a ordenação por mortes
 * @param data1 char* apontador para a data a ser analizada
 * @return data* apontador para a lista a retornar que ou contem um nó ou NULL
 */
     data *search_order(country *right, bool cases, bool deaths, char *data1)
    {
        data *current;

        for (current = right->nextD; current != NULL; current = current->next)
        {
            if (cases == true)
            {
                if (strstr(current->indicator, "cases") && strstr(current->year_week, data1))
                {
                    return current;
                }
            }
            else if (deaths == true)
            {
                if (strstr(current->indicator, "deaths") && strstr(current->year_week, data1))
                {
                    return current;
                }
            }
        }

        return current;
    }
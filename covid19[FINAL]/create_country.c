#include "projectHeader.h"
/**
 * @brief Create a country object: Função principal responsável pela criação da lista horizontal (nós que contém informação fixa sobre os países)
 *        e da lista vertical (nós que contêm informação variavel sobre os países), realiza a ligação entre as duas listas e realiza parte das utilidades
 *        de seleção (remove linhas com casos ou mortes dependendo da utilização), faz ainda uso das funções check_country, insert_end_list e strsep
 *
 * @param cont bool, a enviar à função check_remove, flag para a verificação de um certo continente na linha a avaliar
 * @param indi bool, a enviar à função check_remove, flag para a verificação de um certo indicador na linha a avaliar
 * @param continent bool, a enviar à função check_remove, nome do continente a verificar linha a avaliar
 * @param indicator bool, a enviar à função check_remove, nome do indicador a verificar linha a avaliar
 * @param file_in FILE, ponteiro para o ficheiro de dados
 * @return country* ponteiro para a cabeça da lista criada
 */
country *create_country(bool cont, bool indi, char *continent, char *indicator, FILE *file_in)
{
    country *new_country = NULL, *send = NULL;
    data *data_head = NULL, *data_node = NULL, *data_tail = NULL; //datahead = liga ao next_D; data_node = utilizado na criação dos nós da lista vertical
                                                                  //data_tail = utilizada para a ligação entre nós da lista vertical
    char string[140], *found;
    bool remove = true, save = true, first = true;/*verifica se é a primeira linha, não deve ser verificada na função check token*/
    int n = 0;

    while (fgets(string, sizeof(string), file_in) != NULL)
    {
        char *line = string;
        if(first == false){token_check(line,send,file_in);}
        first = false;
        remove = check_remove(cont, indi, continent, indicator, line);
        if (remove == false) /*se a linha não for ignorada são criados novos nós*/
        {
            found = strsep(&line, ",");
            new_country = check_country(send, found);

            if (new_country == NULL) /*se não existir país é criado um novo nó*/
            {

                if ((new_country = (country *)calloc(1, sizeof(country))) == NULL)
                {
                    printf("Não foi possível alocar o bloco de memória\n");
                    exit(EXIT_FAILURE);
                }
                insert_end_list(send, new_country);
                if (save == true)
                {
                    send = new_country;
                    save = false;
                }

                strcpy(new_country->country, found);

                found = strsep(&line, ",");
                strcpy(new_country->country_ID, found);

                found = strsep(&line, ",");
                strcpy(new_country->continent, found);

                found = strsep(&line, ",");
                new_country->population = atoi(found);

                if ((data_node = (data *)calloc(1, sizeof(data))) == NULL)
                {
                    printf("Não foi possível alocar o bloco de memória\n");
                    exit(EXIT_FAILURE);
                }

                data_node->next = NULL;

                data_head = data_node;
                data_tail = data_node;

                new_country->nextD = data_head;

                found = strsep(&line, ",");
                strcpy(data_node->indicator, found);

                found = strsep(&line, ",");
                data_node->weekly_count = atoi(found);

                found = strsep(&line, ",");
                strcpy(data_node->year_week, found);

                found = strsep(&line, ",");
                data_node->rate_14_day = atof(found);

                found = strsep(&line, ",");
                data_node->cumulative_count = atoi(found);
            }
            else
            {
                data_tail = find_data_tail(new_country); //no caso de já existir o nó do país, deve ser procurado o último nó da lista vertical

                if ((data_node = (data *)calloc(1, sizeof(data))) == NULL)
                {
                    printf("Não foi possível alocar o bloco de memória\n");
                    exit(EXIT_FAILURE);
                }

                data_node->next = NULL;

                data_tail->next = data_node;
                data_tail = data_node;

                for(n = 0; n<3; n++)
                {
                    found = strsep(&line, ",");
                }

                found = strsep(&line, ",");
                strcpy(data_node->indicator, found);

                found = strsep(&line, ",");
                data_node->weekly_count = atoi(found);

                found = strsep(&line, ",");
                strcpy(data_node->year_week, found);

                found = strsep(&line, ",");
                data_node->rate_14_day = atof(found);

                found = strsep(&line, ",");
                data_node->cumulative_count = atoi(found);
            }
        }
    }
    return send;
}
/**
 * @brief Verifica se um certo país já tem um nó na lista
 *
 * @param listHead country* cabeça da lista
 * @param name char* nome do país a verificar
 * @return country* retorna um ponteiro para o nó do país se existir ou NULL se ainda não tiver sido criado
 */
country *check_country(country *listHead, char *name)
{
    country *curr = NULL;

    for (curr = listHead; curr != NULL; curr = curr->nextC)
    {
        if (strcmp(curr->country, name) == 0)
        {
            return curr;
        }
    }

    return curr;
}
/**
 * @brief liga os elementos da lista horizontal, criando a "linked list"
 *
 * @param head country* cabeça da lista principal
 * @param new_country country* nó currente a ser ligado à lista
 */
void insert_end_list(country *head, country *new_country)
{
    country *current;
    if (head == NULL) /*se head é NULL então o primeiro node torna-se a cabeça*/
    {
        head = new_country;
    }
    else
    {
        current = head;
        while (current->nextC != NULL) /*Avança na lista até encontrar o fim */
        {
            current = current->nextC;
        }
        current->nextC = new_country;
    }
}
/**
 * @brief encontra o último nó colocado na lista vertical, útil quando os elementos da lista horizontal não se encontram de forma sequencial
 *
 * @param country_head country* cabeça do nó do país a ser percorrido
 * @return data* retorna o último nó colocado na lista vertical na lista vertical
 */
  data *find_data_tail(country *country_head)
    {
        data *data, *prev = NULL;
        data = country_head -> nextD;

        while(data != NULL)
        {
            prev = data;
            data = data -> next;
        }
        return prev; //retorna a instância antes do ponteiro apontar para NULL => O último nó da lista vertical
    }
/**
 * @brief função auxiliar das funcionalidades -L e -D, remove, se assim especificado, linhas que não estão de acordo com os comandos anteriores
 *
 * @param cont bool flag para a verificação de um certo continente na linha a avaliar
 * @param indi bool flag para a verificação de um certo indicador na linha a avaliar
 * @param needlecont bool nome do continente a verificar linha a avaliar
 * @param needlecase bool nome do indicador a verificar linha a avaliar
 * @param line linha a ser avaliada
 * @return true se a linha for removida
 * @return false se a linha não for removida
 */
    bool check_remove(bool cont, bool indi, char *needlecont, char *needlecase, char *line)
    {
        static bool first = true;

        if (first == true) //A primeira linha será sempre retirada da lista
        {
            first = false;
            return true;
        }

        if (strstr(line, needlecont) == NULL && cont == true)
        {
            return true;
        }

        if ((strstr(line, needlecase) == NULL) && indi == true)
        {
            return true;
        }

        return false;
    }
/**
 * @brief função strsep, realiza a separação da linha em tokens para posterior colocação nos elementos dos nós, como não se encontra na standart library
 *         teve de ser colocada nativamente.
 *
 * @param stringp ponteiro para o ponteiro, da string a ser cortada
 * @param delim   carater delimitador do corte
 * @return char*  retorna o ponteiro para a string após o corte
 */
char *strsep(char **string, const char *delim) {
    char *this = *string;

    if (this != NULL)
    {
        *string += strcspn(*string, delim);/*verifica a localização do "," do ficheiro e atualiza o ponteiro da string*/
        if (**string)
        {
            *(*string)++ = '\0';;
        }
        else
            *string = 0;
    }
    return this;
}
/**
 * @brief Create a country dat object: função que converte um ficheiro .dat e organiza-o numa lista para postrior reprodução do ficheiro .csv
 *        funcionalidade e estruturação semelhate à função create_country
 *
 * @param file_in FILE* ficheiro .dat a ser lido
 * @return country* retorno da cabeça da lista
 */
country *create_country_dat(FILE *file_in)
{
    country *new_country = NULL, *send = NULL;
    data *data_head = NULL, *data_node = NULL, *data_tail = NULL;
    char country_check[30];
    int n = 0, i = 0;
    bool save = true, end = false, first = false;
    /*pretendemos ignorar o valor de retorno da função fread*/
    fread(&n, sizeof(n), 1, file_in); //número de países (nós) a serem criados, primeiro elemento do ficheiro .dat

    while (i != n)
    {
        i++;

        if (country_check == NULL)
        {
            break;
        }

        first = true;
        end = false;
        if ((new_country = (country *)calloc(1, sizeof(country))) == NULL)
        {
            printf("Não foi possível alocar o bloco de memória\n");
            exit(EXIT_FAILURE);
        }
        insert_end_list(send, new_country);
        if (save == true)
        {
            send = new_country;
            save = false;
        }
        fread(&new_country->country, sizeof(new_country->country), 1, file_in);
        fread(&new_country->country_ID, sizeof(new_country->country_ID), 1, file_in);
        fread(&new_country->continent, sizeof(new_country->continent), 1, file_in);
        fread(&new_country->population, sizeof(new_country->population), 1, file_in);

        while (end == false)
        {
            if ((data_node = (data *)calloc(1, sizeof(data))) == NULL)
            {
                printf("Não foi possível alocar o bloco de memória\n");
                exit(EXIT_FAILURE);
            }

            fread(&data_node->indicator, sizeof(data_node->indicator), 1, file_in);
            fread(&data_node->weekly_count, sizeof(data_node->weekly_count), 1, file_in);
            fread(&data_node->year_week, sizeof(data_node->year_week), 1, file_in);
            fread(&data_node->rate_14_day, sizeof(data_node->rate_14_day), 1, file_in);
            fread(&data_node->cumulative_count, sizeof(data_node->cumulative_count), 1, file_in);
            fread(&data_node->next, sizeof(data_node->next), 1, file_in);

            if (data_node->next == NULL)
            {
                end = true;
            }

            if (first == true)
            {
                first = false;
                data_head = data_node;
                data_tail = data_node;

                new_country->nextD = data_head;
            }
            else
            {
                data_node->next = NULL;
                data_tail->next = data_node;
                data_tail = data_node;
            }
        }
    }
    return send;
}
/**
 * @brief Função de verificação do ficheiro de entrada, se existirem incongruências nos dados submetidos termina imediatamente o programa
 *        são exemplos: letras no lugar de números, números no lugar de letras, espaços nulos (assumindo que não estamos no campo do rácio).
 *
 * @param string char* linha do ficheiro a ser analisada
 * @param send char*   ponteiro para a cabeça da lista de listas para posterior libertação
 * @param file_in FILE*ponteiro para o ficheiro para posterior fecho
 */
void token_check(char *string, country *send, FILE *file_in)
{
    char *dummy = NULL, *token = NULL, *freeable_dummy = NULL;
    bool warning = false;
    int i = 0, len = 0, n = 0;
    dummy = my_strdup(string);/*a utilização de uma cópia da string original permite que ao regressar à função mãe, se não houverem erros, a mesma não seja alterada*/
    freeable_dummy = dummy;

    for (i = 0; i < 9; i++)
    {
        token = strsep(&dummy, ",");

        if ((i < 3 || i == 4))
        {
            len = strlen(token);
            if (len == 0)
            {
                warning = true;
                break;
            }
            for (n = 0; n < len; n++)
            {
                if (isalpha(token[n]) == 0 && isblank(token[n])==0)
                {
                    warning = true;
                }
            }
        }
        else if (i == 6)
        {
            len = strlen(token);
            if (len == 0)
            {
                warning = true;
                break;
            }
            for (n = 0; n < len && token[n] != '-'; n++)/*verifica as datas, não considerando o '-'*/
            {
                if (isdigit(token[n]) == 0)
                {
                    warning = true;
                }
            }
            n = n + 1;
            while (n < len)
            {
                if (isdigit(token[n]) == 0)
                {
                    warning = true;
                }
                n++;
            }
        }
        else if (i == 7)
        {
            len = strlen(token);
            for (n = 0; n < len && token[n] != '.'; n++)/*verifica o rácio, não considerando o '.'*/
            {
                if (isdigit(token[n]) == 0)
                {
                    warning = true;
                }
            }
            n = n + 1;
            while (n < len)
            {
                if (isdigit(token[n]) == 0)
                {
                    warning = true;
                }
                n++;
            }
        }
        else
        {
            len = strlen(token);
            if (len == 0)
            {
                warning = true;
                break;
            }
            else if (i == 8)
            {
               token[len-1] = '\0';/*de modo a conseguir se analisar o último token de ficheiros em formato UNIX*/
               len = strlen(token);
               if(strstr(token,"\r"))
               {
                   len = len - 1;
               }
                if(len == 0)
                   {
                       warning = true;
                   }
            }

            for (n = 0; n < len; n++)
            {
                if (isdigit(token[n]) == 0)
                {
                    warning = true;
                }
            }
        }
    }

    free(freeable_dummy); /*como a string dummy é overwritten pelo strsep é necessária a sua libertação através de um outro ponteiro que aponte para a cópia*/
    if (warning == true)
    {
        printf("> -1 erro de leitura: parâmetro nulo ou inválido no ficheiro de entrada\n");
        free_list(send);
        fclose(file_in);
        exit(EXIT_FAILURE);
    }
}
/**
 * @brief Fução strdup da GNU Library, como não se encontra na standart library
 *         teve de ser colocada nativamente. Realiza uma cópia da string de um ponteiro para outro, com alocação de memória
 *
 * @param s char* string a ser copiada
 * @return char* ponteiro para a nova cópia, NULL se a string original for nula
 */
char *my_strdup (char *s)
{
  size_t length = strlen (s)+1; /*para alocar espaço para o terminador da string, na implementação corrente não é necessário já que os terminadores de cada string são \r\n*/
  void *new = malloc (length); /*ponteiro para tipo indefinido */
  if (new == NULL)
  {
      return NULL;
  }
  return (char *) memcpy (new, s, length); /*copia s para new no espaço alocado length e retorna o ponteiro*/
}

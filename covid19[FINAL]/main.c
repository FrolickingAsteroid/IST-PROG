#include "projectHeader.h"

/**
 * @brief programa principal que recolhe os vários parametros das funcionalidades do jogo.
 *
 * @param argc int número de argumentos na linha de comando
 * @param argv [] char* vector de strings com os argumentos na linha de comando
 * @return int sucesso do programa
 */
int main(int argc, char **argv)
{
    char *read = "all", *select = NULL, *restri[3], *in = NULL, *out = NULL, *order[2];
    int opt = 0, i = 0;
    /*parametros de entrada, não sendo especificadas quaisquer comandos (assumindo que -i e -o são sempre especificados)*/
    opterr = 0;
    order[0] = "alfa", order[1] = NULL;
    restri[0] = NULL, restri[1] = NULL, restri[2] = NULL;

    while ((opt = getopt(argc, argv, LISTA_OPCOES)) != -1)
    {
        switch (opt)
        {
        case 'L':
            read = optarg;
            break;
        case 'S':
            i = 0;
            optind--;
            for (; optind < argc && *argv[optind] != '-'; optind++)
            {
                order[i] = argv[optind];
                i++;
            }
            break;
        case 'D':
            select = optarg;
            break;
        case 'P':
            i = 0;
            optind--;
            for (; optind < argc && *argv[optind] != '-'; optind++)
            {
                restri[i] = argv[optind];
                i++;
            }
            break;
        case 'i':
            in = optarg;
            break;
        case 'o':
            out = optarg;
            break;
        case 'h':
            usage();
            break;
        case '?':
            if (isprint(optopt))
            {
                fprintf(stderr, "> opcao `-%c' desconhecida.\n", optopt);
            }
            else
            {
                fprintf(stderr, "> opcao desconhecida.\n");
            }
            usage();
            exit(EXIT_FAILURE);
        }
    }

    verify(read, select, restri, in, out, order);
    list_modes(read, select, restri, in, out, order);

    return 0;
}
/**
 * @brief Função explicativa: Indica os vários comandos e funcionalidades do programa, é chamada sempre que o utilizador comente um erro nos parâmetros de entrada
 *
 */
void usage()
{
    printf("Utilização: [opções] covid19\n");
    printf("Opções válidas:\n");
    printf("[-h]\t\t   Mostrar instruções\n\n");
    printf("Opções de leitura de dados:\n");
    printf("[-L all]\t   leitura integral do ficheiro\n");
    printf("[-L continente]\t apenas lê os dados dos países de um continente indicado\n\n");
    printf("Opções de ordenação de dados:\n");
    printf("[-S alfa]\t  ordenar os países pela ordem alfabética\n");
    printf("[-S pop]\t   ordenação pela ordem decrescente de população de países\n");
    printf("[-S inf yyyy-ww]\t   ordenação pela ordem decrescente de número de total de infetados por países numa determinada data yyyy-ww\n");
    printf("[-S dea yyyy-ww]\t   ordenação pela ordem decrescente de número de total de mortes por países numa determinada data yyyy-ww\n\n");
    printf("Opções de seleção de dados:\n");
    printf("[-D inf]\t seleciona para cada país a semana com mais infetados\n");
    printf("[-D dea]\t seleciona para cada país a semana com mais mortes\n");
    printf("[-D racioinf]\t seleciona para cada país a semana com maior rácio de infetados por 100000 habitantes\n");
    printf("[-D raciodea]\t seleciona para cada país a semana com maior rácio de infetados por milhão de habitantes\n\n");
    printf("Opções de restrição de dados:\n");
    printf("[-P min n]\t seleciona apenas dados de países com mais de n mil habitantes (sendo n um inteiro)\n");
    printf("[-P max n]\t seleciona apenas dados de países com menos de n mil habitantes (sendo n um inteiro)\n");
    printf("[-P date yyyy-ww]\t seleciona apenas dados da semana indicada\n");
    printf("[-P date yyyy-ww yyyy-ww]\t seleciona apenas dados entre as semanas indicada\n\n");
    printf("Opções de leitura e escrita de ficheiros:\n");
    printf("[-i filename.csv]\t leitura de dados de um ficheiro de texto\n");
    printf("[-i filename.dat]\t leitura de dados de um ficheiro binário. Esta opção funciona apenas para impostação de dados sem qualquer especificação de outra opção do tipo -L -S -D ou -P \n");
    printf("[-o filename.csv]\t escrita de dados de um ficheiro de texto\n");
    printf("[-o filename.dat]\t escrita de dados de um ficheiro binário\n");
    printf("É necessário expecificar sempre o formato de escrita e leitura de dados de um ficheiro\n\n");
    printf("Exemplos:\n\n");
    printf("prog$ ./covid19 -i f_texto1.csv -o f_dados.dat\n");
    printf("O ficheiro de texto deve ser lido e criado um ficheiro binário exatamente com a mesma informação\n\n");
    printf("prog$ ./covid19 -i f_texto1.csv -L Africa -S pop -D dea -P min 100 -o f_texto2.csv\n");
    printf("Devem ser lidos dados do ficheiro de texto f_texto1.csv relativos aos países de Africa. Os dados devem considerar as linhas relativas a mortes em zonas com mais de 100 mil habitantes.\n");
    printf("Os dados devem ser ordenados por ordem decrescente de população. O conjunto de dados deve ser escrito no ficheiro de texto f_texto2.csv\n");
}
/**
 * @brief Função que verifica se os parâmetros de entrada são válidos
 *
 * @param read char* apontador para o parâmetro -L
 * @param select char* apontador para o parâmetro -D
 * @param restri char** apontador para o parâmetro -P
 * @param in char* apontador para o parâmetro -i
 * @param out char* apontador para o parâmetro -o
 * @param order char** apontador para o parâmetro -S
 */
void verify(char *read, char *select, char **restri, char *in, char *out, char **order)
{
    int n = 0, len = 0;
    /*=======================verifica se o parâmetro introduzido em -D é válido=========================================*/
    if(select != NULL)
    {
    if(strcmp(select,"inf")!=0 && strcmp(select,"dea")!=0 && strcmp(select,"racioinf")!=0 && strcmp(select,"raciodea")!=0)
    {
		fprintf(stderr, "> comando inexistente.\n");
        usage();
        exit(EXIT_FAILURE);
	}
    }

    /*=======================verifica se o parâmetro introduzido em -S é válido=========================================*/
    if(strcmp(order[0],"alfa")!=0 && strcmp(order[0],"pop")!=0 && strcmp(order[0],"inf")!=0 && strcmp(order[0],"dea")!=0)
    {
		fprintf(stderr, "> comando inexistente.\n");
        usage();
        exit(EXIT_FAILURE);
	}

    /*=======================verifica se o parâmetro introduzido em -P é válido=========================================*/
    if(restri[0]!= NULL)
    {
	if(strcmp(restri[0],"min")!=0 && strcmp(restri[0],"max")!=0 && strcmp(restri[0],"date")!=0 && strcmp(restri[0],"dates")!=0)
    {
		fprintf(stderr, "> comando inexistente.\n");
        usage();
        exit(EXIT_FAILURE);
	}
    }

    /*=======================verifica se foram introduzidos ficheiros de entrada e saída=========================================*/
    if ((in == NULL || out == NULL))
    {
        fprintf(stderr, "> ficheiro de entrada ou saída não especificada.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    /*=======================verifica se o segundo parâmetro introduzido em -S e -P é especificado=========================================*/
    if (((strstr(order[0], "inf") != NULL || strstr(order[0], "dea") != NULL) && order[1] == NULL) || (restri[0] != NULL && restri[1] == NULL))
    {
        fprintf(stderr, "> segundo argumento não especificado.\n");
        usage();
        exit(EXIT_FAILURE);
    }
    /*=======================verifica se o terceiro parâmetro introduzido em -P é especificado=========================================*/
    if (restri[0] != NULL)
    {
        if (strstr(restri[0], "dates") != NULL && restri[2] == NULL)
        {
            fprintf(stderr, "> terceiro argumento não especificado.\n");
            usage();
            exit(EXIT_FAILURE);
        }
    }
    /*=======================verifica se o  parâmetro introduzido em -L é correto (não é um número, pode conter espaços brancis ex.: South Africa=========================================*/
    len = strlen(read);
    for (n = 0; n < len; n++)
    {
        if (isalpha(read[n]) == 0 && isblank(read[n]) == 0)
        {
            fprintf(stderr, "> parâmetro mal especificado.\n");
            usage();
            exit(EXIT_FAILURE);
        }
    }

    /*=======================verifica se parâmetros são introduzidos quando o ficheiro de entrada é .dat =========================================*/
    len = strlen(in);
    if (in[len-4] == '.' && in[len-3]== 'd' && in[len-2] == 'a' && in[len-1] == 't' )
    {   /*embora  não seja possível a especificação de quaisquer parâmetros no que toca a leitura de ficheiros de data, considerando que os valores base de*/
        /*-L e -D já são estipulados, assim mesmo que introduzidos os standart "all" e "alfa", não terão qualquer significado*/

        if((select != NULL || restri[0] != NULL || restri[1] != NULL || restri[2] != NULL ||  strcmp(read,"all") !=0 || strcmp(order[0],"alfa") !=0))
        {
            fprintf(stderr, "> Execução não válida.\n");
            usage();
            exit(EXIT_FAILURE);
        }
    }

/*=======================verifica se o segundo parâmetro introduzido em -S é correto=========================================*/
    if (order[0] != NULL)
    {
        if (((strstr(order[0], "inf") != NULL || strstr(order[0], "dea") != NULL)))
        {
            len = strlen(order[1]);
            for (n = 0; n < len && order[1][n] != '-'; n++)
            {
                if (isdigit(order[1][n]) == 0)
                {
                    fprintf(stderr, "> parâmetro mal especificado.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }
            }
            n = n + 1;
            while (n < len)
            {
                if (isdigit(order[1][n]) == 0)
                {
                    fprintf(stderr, "> parâmetro mal especificado.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }
                n++;
            }
        }
    }

    /*=======================verifica se o segundo ou terceiro parâmetro introduzido em -P é correto=========================================*/
    if (restri[0] != NULL)
    {
        if (((strstr(restri[0], "date"))))
        {
            len = strlen(restri[1]);
            for (n = 0; n < len && restri[1][n] != '-'; n++)
            {
                if (isdigit(restri[1][n]) == 0)
                {
                    fprintf(stderr, "> parâmetro mal especificado.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }
            }
            n = n + 1;
            while (n < len)
            {
                if (isdigit(restri[1][n]) == 0)
                {
                    fprintf(stderr, "> parâmetro mal especificado.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }
                n++;
            }
            if (restri[2] != NULL)
            {
                for (n = 0; n < len && restri[2][n] != '-'; n++)
                {
                    if (isdigit(restri[2][n]) == 0)
                    {
                        fprintf(stderr, "> parâmetro mal especificado.\n");
                        usage();
                        exit(EXIT_FAILURE);
                    }
                }
                n = n + 1;
                while (n < len)
                {
                    if (isdigit(restri[2][n]) == 0)
                    {
                        fprintf(stderr, "> parâmetro mal especificado.\n");
                        usage();
                        exit(EXIT_FAILURE);
                    }
                    n++;
                }
            }
        }
        else
        {
            len = strlen(restri[1]);
            for (n = 0; n < len; n++)
            {
                if (isdigit(restri[1][n]) == 0)
                {
                    fprintf(stderr, "> parâmetro mal especificado.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }

        }
    }

}
}

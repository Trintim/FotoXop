#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>

#include "Imagem.h" 

struct imagem {
    int largura;   /* número de colunas (largura) da imagem em pixels */
    int altura;    /* número de linhas (altura) da imagem em pixels */
    Pixel **pixel; /* matriz altura x largura de pixels com os níveis RGB */
};

void *mallocSafe(size_t nbytes);

Imagem *alocaImagem(int largura, int altura){
    
    Imagem *imgPPM = mallocSafe(sizeof(Imagem));

    imgPPM->altura = altura;
    imgPPM->largura = largura;
    imgPPM->pixel = mallocSafe(altura * sizeof(Pixel *));
    
    for(int h = 0; h < altura; h++){
        imgPPM->pixel[h] = mallocSafe(largura * sizeof(Pixel));
    }    

    return imgPPM;
}

void liberaImagem(Imagem *img){

    if (img == NULL)
    {
        return;
    }
    
    if (img->pixel != NULL){
        for (int h = 0; h < img->altura; h++){
            free(img->pixel[h]);
        }
    }
    free(img->pixel);
    free(img);
}

int obtemLargura(Imagem *img)
{
    return img->largura;
}

int obtemAltura(Imagem *img)
{
    return img->altura;
}

Pixel obtemPixel(Imagem *img, int l, int c)
{
    return img->pixel[l][c];
}

Imagem *copiaImagem(Imagem *origem){
    
    if(origem == NULL){
        printf("A alocação não foi feita");
        return NULL;
    }

    Imagem *imagemCopiada = alocaImagem(origem->largura, origem->altura);
    
    for(int h = 0; h < imagemCopiada->altura; h++){
        for (int w = 0; w < imagemCopiada->largura; w++){
            imagemCopiada->pixel[h][w] = origem->pixel[h][w];
        }        
    }

    return imagemCopiada;
}
/*
 * A função 'recolorePixel' recebe um ponteiro para uma Imagem,
 * uma posicao [l][c] de um pixel e um 'pixel'. A função é responsável
 * por recolorir o pixel da posição [l][c] com as informações (cores)
 * presentes em 'pixel'.
 */
void recolorePixel(Imagem *img, int l, int c, Pixel pixel){
    img->pixel[l][c].cor[RED]   = pixel.cor[RED];
    img->pixel[l][c].cor[GREEN] = pixel.cor[GREEN];
    img->pixel[l][c].cor[BLUE]  = pixel.cor[BLUE];
}

/*
 * A função 'carregaImagem' recebe o nome do arquivo que contém uma imagem no 
 * formato PPM. A função é responsável por carregar os dados desse arquivo em
 * uma Imagem e retornar o ponteiro dessa imagem.  
 * 
 * Para saber mais sobre leitura/escrita de arquivos em C:
 * https://www.geeksforgeeks.org/basics-file-handling-c/ 
 * https://www.guru99.com/c-file-input-output.html
 * 
 * Você não deve modificar essa função 
 */
Imagem *carregaImagem(const char *nomeArquivo){
    char buff[16];
    Imagem *img;
    FILE *fp;
    int c;
    //Tenta abrir o arquivo em modo de leitura ("r" - reading)
    fp = fopen(nomeArquivo, "r");
    //Verifica se foi retornado um ponteiro válido para o arquivo.
    if (fp == NULL) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    if (!fgets(buff, sizeof(buff), fp)) {
        perror("stdin");
        exit(1);
    }
    //Faz a leitura do formato da imagem. 
    //Nesse EP, iremos trabalhar apenas com o formato P3 (colorido e não binário)
    if (buff[0] != 'P' || buff[1] != '3') {
        fprintf(stderr, "Formato da imagem inválido (deve ser 'P3')\n");
        exit(1);
    }
    //Faz a leitura do comentário presente no arquivo da imagem. 
    //Essa informação é ignorada na estrutura Imagem.
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp); /* coloca último caractere devolta no buffer */
    int largura, altura, threshold;
    //Faz a leitura da 'largura' e 'altura'
    if (fscanf(fp, "%d %d", &largura, &altura) != 2) {
        fprintf(stderr, "Tamanho da imagem invalido\n");
        exit(1);
    }
    //Faz a leitura do 'threshold' (vamor máximo que uma cor pode receber)
    //Normalmente, esse valor é 255
    if (fscanf(fp, "%d", &threshold) != 1) {
        fprintf(stderr, "Componente rgb invalido\n");
        exit(1);
    }
    //Ignora as possíveis linhas em branco
    while (fgetc(fp) != '\n') ;

    //Aloca a imagem de acordo com a 'largura' e 'altura' da imagem do arquivo
    img = alocaImagem(largura, altura);
    if (!img) {
        fprintf(stderr, "Erro na alocacao da memoria\n");
        exit(1);
    }
    img->altura = altura;
    img->largura = largura;

    int r, g, b;
    //Faz a leitura das cores de cada pixel
    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            c = fscanf(fp, "%d %d %d", &r, &g, &b);
            img->pixel[i][j].cor[RED]   = (Byte)r; /* intensidade vermelho */
            img->pixel[i][j].cor[GREEN] = (Byte)g; /* intensidade verde */
            img->pixel[i][j].cor[BLUE]  = (Byte)b; /* intensidade azul */
        }
    }
    //Fecha o arquivo.
    fclose(fp);

    //Retorna o ponteiro com as informações da Imagem
    return img;
}

/*
 * A função 'salvaImagem' recebe um ponteiro para uma Imagem e o nome de um 
 * arquivo. A função é responsável por criar um arquivo com nome 'nomeArquivo' e
 * escrever os dados de 'img' nesse arquivo. Ao fim, se tudo estiver saido como
 * esperado, o arquivo gerado pode ser aberto por algum editor de imagem 
 * (GIMP, por exemplo) e/ou pelo próprio programa gerado pelo EP.   
 * 
 * Dica: para testar a função, você pode salvar o arquivo com algum nome .ppm e, 
 * em seguida, abrir o EP carregando o arquivo recém criado.  
 * 
 * Para saber mais sobre leitura/escrita de arquivos em C:
 * https://www.geeksforgeeks.org/basics-file-handling-c/ 
 * https://www.guru99.com/c-file-input-output.html
 */
void salvaImagem(Imagem *img, const char *nomeArquivo){

    FILE *arquivo;
    int c, rgb_comp_color;
    //Tenta abrir o arquivo em modo de escrita ("w" - writing)
    arquivo = fopen(nomeArquivo, "w");
    //Verifica se foi retornado um ponteiro válido para o arquivo.
    if (arquivo == NULL) {
        fprintf(stderr, "salvaImagem: ERRO: arquivo '%s' nao pode ser criado\n", nomeArquivo);
        return;
    }
    //Começa a escrita no arquivo 'nomeArquivo'
    //Escreve o formato da imagem
    fprintf(arquivo, "P3\n");
    //Escreve um comentário na imagem
    fprintf(arquivo, "#EP1 - Estrutura de Dados 1 - SEU NOME\n");
    //Escreve a 'largura', 'altura' e 'threshold'
    fprintf(arquivo, "%d %d %d\n", img->largura, img->altura, 255);
    //Escreve os dados de cada pixel

    /* Agora é com você. Só escrever as cores (RBG) de cada um dos pixels */
    for (int h = 0; h < img->altura; h++){
        for (int w = 0; w < img->largura; w++){
            fprintf(arquivo, "%d\n", img->pixel[h][w].cor[GREEN]);
            fprintf(arquivo, "%d\n", img->pixel[h][w].cor[BLUE]);
            fprintf(arquivo, "%d\n", img->pixel[h][w].cor[RED]);
        }
    }

    fclose(arquivo);
    printf("salvaImagem: A imagem foi salva no arquivo: '%s'\n", nomeArquivo);
}

/*  
 * A função 'mallocSafe' é responsável por fazer a alocação "segura"
 * de 'nbytes' de memória. Ela retorna o endereção de memória alocado, 
 * em caso de sucesso, ou exibe uma mensagem de erro e encerra o programa
 * se a alocação não for possível.
 * 
 * Exemplo de utilização da função para alocar um int e um double:
 * int *x = mallocSafe(sizeof(int));
 * double *y = mallocSafe(sizeof(double));
 * 
 * Note que não há diferença na utilização da 'mallocSafe' e do 'malloc'.
 * A diferença é que a 'mallocSafe' já faz a verificação se a memória 
 * foi alocada ou não. 
 * 
 * Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
*/
void *mallocSafe(size_t nbytes) {
    void *ptr = malloc(nbytes);
    if (ptr == NULL) {
        printf("Sem memória!\n");
        exit(1);
    }
    return ptr;
}

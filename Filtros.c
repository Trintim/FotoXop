#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Imagem.h"  /* Tipos e protótipos de funções que manipulam imagens */
#include "Filtros.h" /* Protótipo das funções responsáveis pelos filtros das imagens */

/* Função responsável por escurecer uma imagem
 * A função recebe um ponteiro para um struct Imagem e altera/escurece 
 * cada um de seus pixels em cada canal de cor (RGB).
 * 
 * Analise essa função com bastante atenção. Ela irá te auxiliar no entendimento 
 * do TAD Imagem e como outros filtros podem ser implementados.
 *
 * Você não precisa modificar essa função 
 */
void escurecerImagem(Imagem *img){
    int v;

    printf("Digite o fator de escurecimento entre 0 e 255: ");
    scanf("%d", &v);

    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            //Obtém o pixel da posição (h, w) da imagem 
            Pixel pixel = obtemPixel(img, h, w);
            //Modifica cada canal de cor do pixel
            pixel.cor[RED]   = (((int)pixel.cor[RED] - v) >= 0 ? (pixel.cor[RED] - v) : 0);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] - v) >= 0 ? (pixel.cor[GREEN] - v) : 0);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE] - v) >= 0 ? (pixel.cor[BLUE] - v) : 0);
            //Grava o novo pixel na posição (h, w) da imagem
            recolorePixel(img, h, w, pixel);
        }
    }
}

/* Função responsável por clarear uma imagem
 * A função recebe um ponteiro para um struct Imagem e altera/clareia 
 * cada um de seus pixels em cada canal de cor (RGB).
 */
void clarearImagem(Imagem *img){
    
    int v;
    printf("Digite o fator de clareamento entre 0 e 255: ");
    scanf("%d", &v);

    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            //Obtém o pixel da posição (h, w) da imagem 
            Pixel pixel = obtemPixel(img, h, w);
            //Modifica cada canal de cor do pixel
            pixel.cor[RED]   = (((int)pixel.cor[RED] + v) > 255 ? 255 : (pixel.cor[RED] + v));
            
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] + v) > 255 ? 255 : (pixel.cor[GREEN] + v));
            
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE] + v) > 255 ? 255 : (pixel.cor[BLUE] + v));
            //Grava o novo pixel na posição (h, w) da imagem
            recolorePixel(img, h, w, pixel);
        }
    }
}

/* Função responsável por converter uma imagem colorida em escala de cinza
 * A função recebe um ponteiro para um struct Imagem e converte 
 * cada um de seus pixels em cada canal de cor (RGB) em uma tonalidade de cinza.
 * 
 * No pdf de descrição do EP existem duas estretégias para a implementação 
 * desse filtro. Você deve implementar uma delas ou pesquisar por outras alternativas. 
 */
void escalaDeCinzaImagem(Imagem *img){
    
    int media;

    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            
            Pixel pixel = obtemPixel(img, h, w);

            media = ((int)pixel.cor[GREEN] + (int)pixel.cor[RED] + (int)pixel.cor[BLUE]) / 3;

            pixel.cor[GREEN] = media;
            pixel.cor[RED] = media;
            pixel.cor[BLUE] = media;

            recolorePixel(img, h, w, pixel);
        }
    }

}

/* Função responsável por aplicar o filtro de Sobel na imagem. 
 * Leia o pdf do EP para obter mais informações de como o filtro é implementado. 
 */
void filtroSobel(Imagem *img){

    /* Dicas:
     * 1) Se quiser, pode ignorar as bordas das imagens
     * 2) Para o pixel na posição (h, w), obtenha os outros (-1) pixels vizinhos e aplique a matriz gx e gy 
     *    em cada banda de cor do pixel. Combine o valor obtido por gx e gy e modifique o pixel de img.
     *    Lembre-se que a obtenção dos pixels deve ser a partir de uma cópia da imagem. 
     * 3) Verifique se o novo valor obtido para a banda de cor é um valor válido (entre 0 e 255).
    */

    Imagem *copiaImagemFiltroSobel = copiaImagem(img);

    for (int h = 1; h < obtemAltura(copiaImagemFiltroSobel) - 1; h++) {
        for (int w = 1; w < obtemLargura(copiaImagemFiltroSobel) - 1; w++) {
            /*variaveis de cada pixel das matrizes Gx e Gy*/
            int xRED = 0, yRED = 0;
            int xBLUE = 0, yBLUE = 0;
            int xGREEN = 0, yGREEN = 0;
            int filtroVermelho = 0,  filtroAzul = 0, filtroVerde = 0;

            /* Matriz Gx = filtro de Sobel pela borda esquerda
                  1  0 -1
                  2  0 -2
                  1  0 -1
            */
            Pixel novoPixel = obtemPixel(copiaImagemFiltroSobel, h-1, w-1);
            xRED += (int)novoPixel.cor[RED] * 1;
            xGREEN += (int)novoPixel.cor[GREEN] * 1;
            xBLUE += (int)novoPixel.cor[BLUE] * 1;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h-1, w);
            xRED += (int)novoPixel.cor[RED] * 0;
            xGREEN += (int)novoPixel.cor[GREEN] * 0;
            xBLUE += (int)novoPixel.cor[BLUE] * 0;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h-1, w+1);
            xRED += (int)novoPixel.cor[RED] * (-1);
            xGREEN += (int)novoPixel.cor[GREEN] * (-1);
            xBLUE += (int)novoPixel.cor[BLUE] * (-1);
            //-------------------------------------------------------------------
            novoPixel = obtemPixel(copiaImagemFiltroSobel, h, w-1);
            xRED += (int)novoPixel.cor[RED] * 2;
            xGREEN += (int)novoPixel.cor[GREEN] * 2;
            xBLUE += (int)novoPixel.cor[BLUE] * 2;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h, w);
            xRED += (int)novoPixel.cor[RED] * 0;
            xGREEN += (int)novoPixel.cor[GREEN] * 0;
            xBLUE += (int)novoPixel.cor[BLUE] * 0;
            
            novoPixel = obtemPixel(copiaImagemFiltroSobel, h, w+1);
            xRED += (int)novoPixel.cor[RED] * (-2);
            xGREEN += (int)novoPixel.cor[GREEN] * (-2);
            xBLUE += (int)novoPixel.cor[BLUE] * (-2);
            //------------------------------------------------------------------
            novoPixel = obtemPixel(copiaImagemFiltroSobel, h+1, w-1);
            xRED += (int)novoPixel.cor[RED] * 1;
            xGREEN += (int)novoPixel.cor[GREEN] * 1;
            xBLUE += (int)novoPixel.cor[BLUE] * 1;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h+1, w);
            xRED += (int)novoPixel.cor[RED] * 0;
            xGREEN += (int)novoPixel.cor[GREEN] * 0;
            xBLUE += (int)novoPixel.cor[BLUE] * 0;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h+1, w+1);
            xRED += (int)novoPixel.cor[RED] * (-1);
            xGREEN += (int)novoPixel.cor[GREEN] * (-1);
            xBLUE += (int)novoPixel.cor[BLUE] * (-1);

            /*Matriz Gy para borda superior de sobel
            
              1  2  1
              0  0  0
             -1 -2 -1

            */

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h-1, w-1);
            yRED += (int)novoPixel.cor[RED] * 1;
            yGREEN += (int)novoPixel.cor[GREEN] * 1;
            yBLUE += (int)novoPixel.cor[BLUE] * 1;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h-1, w);
            yRED += (int)novoPixel.cor[RED] * 2;
            yGREEN += (int)novoPixel.cor[GREEN] * 2;
            yBLUE += (int)novoPixel.cor[BLUE] * 2;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h-1, w+1);
            yRED += (int)novoPixel.cor[RED] * 1;
            yGREEN += (int)novoPixel.cor[GREEN] * 1;
            yBLUE += (int)novoPixel.cor[BLUE] * 1;
            //-------------------------------------------------------------------
            novoPixel = obtemPixel(copiaImagemFiltroSobel, h, w-1);
            yRED += (int)novoPixel.cor[RED] * 0;
            yGREEN += (int)novoPixel.cor[GREEN] * 0;
            yBLUE += (int)novoPixel.cor[BLUE] * 0;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h, w);
            yRED += (int)novoPixel.cor[RED] * 0;
            yGREEN += (int)novoPixel.cor[GREEN] * 0;
            yBLUE += (int)novoPixel.cor[BLUE] * 0;

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h, w+1);
            yRED += (int)novoPixel.cor[RED] * 0;
            yGREEN += (int)novoPixel.cor[GREEN] * 0;
            yBLUE += (int)novoPixel.cor[BLUE] * 0;
            //------------------------------------------------------------------            
            novoPixel = obtemPixel(copiaImagemFiltroSobel, h+1, w-1);
            yRED += (int)novoPixel.cor[RED] * (-1);
            yGREEN += (int)novoPixel.cor[GREEN] * (-1);
            yBLUE += (int)novoPixel.cor[BLUE] * (-1);

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h+1, w);
            yRED += (int)novoPixel.cor[RED] * (-2);
            yGREEN += (int)novoPixel.cor[GREEN] * (-2);
            yBLUE += (int)novoPixel.cor[BLUE] * (-2);

            novoPixel = obtemPixel(copiaImagemFiltroSobel, h+1, w+1);
            yRED += (int)novoPixel.cor[RED] * (-1);
            yGREEN += (int)novoPixel.cor[GREEN] * (-1);
            yBLUE += (int)novoPixel.cor[BLUE] * (-1);

            /* valores para indicar o operador de sobel */
            filtroAzul = sqrt(pow(yBLUE, 2) + pow(xBLUE, 2));
            filtroVermelho = sqrt(pow(yRED, 2) + pow(xRED, 2));
            filtroVerde = sqrt(pow(yGREEN, 2) + pow(xGREEN, 2));
            
            //pintando os valores dos novos pixel de cada cor
            if(filtroAzul < 0){
                novoPixel.cor[BLUE] = 0;
            }

            else if(filtroAzul > 255) {
                novoPixel.cor[BLUE] = 255;
            }

            else{
                novoPixel.cor[BLUE] = filtroAzul;
            }

            if(filtroVermelho < 0){
                novoPixel.cor[RED] = 0;
            }

            else if(filtroVermelho > 255){
                novoPixel.cor[RED] = 255;
            }

            else{
                novoPixel.cor[RED] = filtroVermelho;
            }

            if(filtroAzul < 0){
                novoPixel.cor[BLUE] = 0;
            }
    
            else if(filtroVerde > 255){
                novoPixel.cor[GREEN] = 255;
            }

            else{ 
                novoPixel.cor[GREEN] = filtroVerde;
            }

            recolorePixel(img, h, w, novoPixel);
        }
    }

    liberaImagem(copiaImagemFiltroSobel);    
}

/* Função responsável por aplicar a detecçõa de bordas de Laplace na imagem. 
 * Leia o pdf do EP para obter mais informações de como o filtro é implementado. 
 */
void deteccaoBordasLaplace(Imagem *img) {

    Imagem *copiaImagemLaplace = copiaImagem(img);

    for (int h = 1; h < obtemAltura(copiaImagemLaplace) - 1; h++) {
        for (int w = 1; w < obtemLargura(copiaImagemLaplace) - 1; w++) {
            
            int laplaceVermelho = 0;
            int laplaceVerde = 0;
            int laplaceAzul = 0;

            Pixel pixelLaplace = obtemPixel(copiaImagemLaplace, h, w);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * 4;
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * 4;
            laplaceVermelho += (int)pixelLaplace.cor[RED] * 4;

            pixelLaplace = obtemPixel(copiaImagemLaplace, h-1, w-1);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * 0;
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * 0;
            laplaceVermelho += (int)pixelLaplace.cor[RED] * 0;

            pixelLaplace = obtemPixel(copiaImagemLaplace, h+1, w-1);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * 0;
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * 0;
            laplaceVermelho += (int)pixelLaplace.cor[RED] * 0;

            pixelLaplace = obtemPixel(copiaImagemLaplace, h-1, w+1);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * 0;
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * 0;
            laplaceVermelho += (int)pixelLaplace.cor[RED] * 0;

            pixelLaplace = obtemPixel(copiaImagemLaplace, h+1, w+1);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * 0;
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * 0;
            laplaceVermelho += (int)pixelLaplace.cor[RED] * 0;

            pixelLaplace = obtemPixel(copiaImagemLaplace, h-1, w);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * (-1);
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * (-1);
            laplaceVermelho += (int)pixelLaplace.cor[RED] * (-1);

            pixelLaplace = obtemPixel(copiaImagemLaplace, h+1, w);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * (-1);
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * (-1);
            laplaceVermelho += (int)pixelLaplace.cor[RED] * (-1);

            pixelLaplace = obtemPixel(copiaImagemLaplace, h, w-1);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * (-1);
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * (-1);
            laplaceVermelho += (int)pixelLaplace.cor[RED] * (-1);

            pixelLaplace = obtemPixel(copiaImagemLaplace, h, w+1);
            laplaceAzul += (int)pixelLaplace.cor[BLUE] * (-1);
            laplaceVerde += (int)pixelLaplace.cor[GREEN] * (-1);
            laplaceVermelho += (int)pixelLaplace.cor[RED] * (-1);

            if(laplaceVerde < 0){
                pixelLaplace.cor[GREEN] = 0;
            }

            else if(laplaceVerde > 255){
                pixelLaplace.cor[GREEN] = 255;
            }

            else {
                pixelLaplace.cor[GREEN] = laplaceVerde;
            }

            if(laplaceAzul < 0){
                pixelLaplace.cor[BLUE] = 0;
            }

            else if(laplaceAzul > 255){
                pixelLaplace.cor[BLUE] = 255;
            }

            else {
                pixelLaplace.cor[BLUE] = laplaceAzul;
            }

            if(laplaceVermelho < 0){
                pixelLaplace.cor[RED] = 0;
            }

            else if(laplaceVermelho > 255){
                pixelLaplace.cor[RED] = 255;
            }

            else {
                pixelLaplace.cor[RED] = laplaceVermelho;
            }

            recolorePixel(img, h , w, pixelLaplace);
        }    
    }
    liberaImagem(copiaImagemLaplace);
}

/* Função responsável por aplicar na imagem o filtro definido por você. Sewa criativo! 
 * Filtros que apenas fazem algumas modificações simples em cada pixel 
 * (por exemplo, negativo, espelhar a imagem) não serão considerados.
 */


void meuFiltro(Imagem *img){
    
    Imagem *copiaImagemMeuFiltro = copiaImagem(img);
    

    for (int h = 1; h < obtemAltura(copiaImagemMeuFiltro) - 1; h++){
        for (int w = 1; w < obtemLargura(copiaImagemMeuFiltro) - 1; w++){
            int myRed = 0, myGreen = 0, myBlue = 0;

            Pixel myPixel = obtemPixel(copiaImagemMeuFiltro, h, w);
            myRed += (int)myPixel.cor[RED] * 8;
            myBlue += (int)myPixel.cor[BLUE] * 8;
            myGreen += (int)myPixel.cor[GREEN] * 8;

            myPixel = obtemPixel(copiaImagemMeuFiltro, h-1, w-1);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h-1, w);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h-1, w+1);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h, w-1);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h, w+1);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h+1, w-1);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h+1, w);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);

            myPixel = obtemPixel(copiaImagemMeuFiltro, h+1, w+1);
            myRed += (int)myPixel.cor[RED] * (-1);
            myBlue += (int)myPixel.cor[BLUE] * (-1);
            myGreen += (int)myPixel.cor[GREEN] * (-1);


            if(myBlue < 0){
                myPixel.cor[BLUE] = 0;
            }
            
            else if(myBlue > 255){
                myPixel.cor[BLUE] = 255;
            }

            else{
                myPixel.cor[BLUE] = myBlue;
            }

             if(myRed < 0){
                myPixel.cor[RED] = 0;
            }
            
            else if(myBlue > 255){
                myPixel.cor[RED] = 255;
            }

            else{
                myPixel.cor[RED] = myRed;
            }

             if(myGreen < 0){
                myPixel.cor[GREEN] = 0;
            }
            
            else if(myGreen > 255){
                myPixel.cor[GREEN] = 255;
            }

            else{
                myPixel.cor[GREEN] = myGreen;
            }

            recolorePixel(img, h, w, myPixel);
        }
    }
    liberaImagem(copiaImagemMeuFiltro);
}
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct lista{
	float dado;
	int linha;
	int coluna;
	struct lista *proxColuna;
	struct lista *proxLinha;
};

struct matriz{
	struct lista **colunas;
	struct lista **linhas;
	int linha;
	int coluna;
};


float consultaValor(struct matriz *matriz, int i, int j){ // i - linha   j - coluna
	if(i <= matriz->linha && j <= matriz->coluna){
		struct lista *listaAux = matriz->linhas[i-1];
		while(listaAux->proxColuna != NULL && listaAux->proxColuna->coluna <= j){ // percorre as listas de linha e de coluna
			listaAux = listaAux->proxColuna;
		}
		if(listaAux->coluna == j){
			return(listaAux->dado);
		}
	}
	return (0); // se os valores fornecidos nao existirem na matriz ele retorna 0
}


float consultaSomaLinha(struct matriz *matriz, int i){ // i linha
	if(i <= matriz->linha){
		float s = 0;
		struct lista *listaAux = matriz->linhas[i-1];
		while(listaAux != NULL){ // percorre a lista de linhas e soma os valores dele
			s = s + listaAux->dado;
			listaAux = listaAux->proxColuna;
		}
		return (s);
	}
	return (0); // se o valor da linha nao existe na matriz retorna 0
}


float consultaSomaColuna(struct matriz *matriz, int j){ // j coluna
	if(j <= matriz->coluna){
		float s = 0;
		struct lista *listaAux = matriz->colunas[j-1];
		while(listaAux != NULL){ // percorre a lista de colunas e soma todos os valores
			s = s + listaAux->dado;
			listaAux = listaAux->proxLinha;
		}
		return (s);
	}
	return (0); // se o a coluna fornecida nao existe retorna 0
}


void adicionaValor(struct matriz *matriz, int i, int j, float dado){ // i - linha    j - coluna
	if(i <= matriz->linha && j <= matriz->coluna){
		struct lista *listaAux = matriz->linhas[i-1];
		while(listaAux->proxColuna!=NULL && listaAux->proxColuna->coluna <= j){
			listaAux = listaAux->proxColuna;
		} // percorre as colunas pra chegar no lugar desejado
		if(listaAux->coluna == j){
			listaAux->dado = dado;
		}
		else{
			struct lista *novo = (struct lista *)malloc(sizeof(struct lista)); // cria um novo para ser adicionado nessa posicao
			novo->linha = i;
			novo->coluna = j;
			novo->dado = dado;
			novo->proxColuna = listaAux->proxColuna;
			listaAux->proxColuna = novo;
			listaAux = matriz->colunas[j-1];
			while(listaAux->proxLinha != NULL && listaAux->proxLinha->linha <= i){
				listaAux = listaAux->proxLinha;
			} // faz o mesmo com a linha, percorre a lista de linha pra achar a posicao certa
			novo->proxLinha = listaAux->proxLinha;
			listaAux->proxLinha = novo;
		}
	}
}

struct matriz * criarMatriz(int i, int j){
	struct matriz *matriz = (struct matriz *)malloc(sizeof(struct matriz));
	int l,c;
	matriz->linha = i;
	matriz->coluna = j;
	matriz->linhas = (struct lista **)malloc(sizeof(struct lista *)*i); // a matriz eh formada pelas duas listas
	matriz->colunas = (struct lista **)malloc(sizeof(struct lista *)*j); // a de linha e a de coluna
	for (l=0 ; l < i ; l++){ // inicia com os ponteiros NULL
		matriz->linhas[l] = (struct lista *)malloc(sizeof(struct lista));
		matriz->linhas[l]->proxLinha = NULL;
		matriz->linhas[l]->proxColuna = NULL;
	}
	for (c = 0 ; c < j ; c++){ // inicia com os ponteiros NULL
		matriz->colunas[c] = (struct lista *)malloc(sizeof(struct lista));
		matriz->colunas[c]->proxLinha = NULL;
		matriz->colunas[c]->proxColuna = NULL;
	}
	return (matriz);
}

void excluirMatriz(struct matriz *matriz){
	int i;
	for (i = 0 ; i < matriz->linha ; i++){
		struct lista *aux = matriz->linhas[i]->proxColuna;
		while (aux != NULL){
			struct lista *excluir = aux;
			aux = aux->proxColuna;
			free(excluir);
		}
		free(matriz->linhas[i]);
	}
	free(matriz->colunas); // para excluir a matriz eh necessario primeiro
	free(matriz->linhas); // dar free nas listas que estao relacionado com a matriz
	free(matriz); // entao podemos dar free na matriz
	matriz = NULL;
}

void menu(){
     printf("MENU\n");
     printf("1 - Criar a matriz\n"); 
	 printf("2 - Adicionar um valor\n"); 
	 printf("3 - Consulta de valor\n"); 
	 printf("4 - Consulta da soma de uma coluna\n");
	 printf("5 - Consulta da soma de uma linha\n");
 	 printf("6 - Excluir matriz\n");
	 printf("7 - Sair\n");
}

int main(){
	int opcao,i,j;
	struct matriz *matrizEsparsa;
	float dado;
	int exit = 0;
	
	while(exit == 0){
		menu();
    	scanf("%d",&opcao);
    	if(opcao>=1 && opcao <=7){
    		switch(opcao){
    			case 1:
	            	printf("Forneca as dimensoes da matriz\n");
	            	printf("Numero de linhas: ");
	            	scanf("%d",&i);
	            	printf("Numero de colunas: ");
	            	scanf("%d",&j);
	            	matrizEsparsa = criarMatriz(i,j);
	            	printf("Matriz criada\n\n");
					break;
	        	case 2:
	        		printf("Digite o valor que deseja adiconar: ");
            		scanf("%f",&dado);
	        		printf("Forneca a posicao que deseja adicionar: \n");
            		printf("linha: ");
            		scanf("%d",&i);
            		printf("coluna: ");
            		scanf("%d",&j);
            		adicionaValor(matrizEsparsa,i,j,dado);
            		printf("Valor adicionado com sucesso!\n\n");
		        	break;
            	case 3:
            		printf("Digite a posicao que deseja consultar:\n");
            		printf("Linha: ");
            		scanf("%d",&i);
            		printf("Coluna: ");
            		scanf("%d",&j);
            		printf("O valor nesse posicao eh %f",consultaValor(matrizEsparsa,i,j));
    	        	printf("\n\n");
					break;
           	    case 4:
           	 		printf("Digite a coluna a ser consultada: ");
            		scanf("%d",&j);
            		printf("A soma dos valores da coluna eh %f",consultaSomaColuna(matrizEsparsa,j));
					printf("\n\n");
					break;
				case 5:
            		printf("Digite a linha a ser consultada: ");
            		scanf("%d",&i);
            		printf("A soma dos valores da linha eh %f",consultaSomaLinha(matrizEsparsa,i));
					printf("\n\n");
					break;
				case 6:
					excluirMatriz(matrizEsparsa);
					printf("A matriz foi excluida com sucesso!\n\n");
					break;
    	     	case 7:
    	        	exit = 1;
    	        	break;
	        }
        }
	}
	return 0;
}


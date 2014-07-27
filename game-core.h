#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

// IFSP - São Paulo
// Análise e Desenvolvimento de Sistemas - Linguagem C (1)
// Projeto Didático - 1º Semestre

// Simulação: Roda-a-Roda
// Desenvolvido por Lucas Barcellos (1363417), Pedro Chagas (1363981) e Thiago Nascimento ().

// ----- CÓDIGO FONTE - NÚCLEO (CORE) -----
// Lista de funções auxiliares gerais. O código-fonte MAIN se utiliza delas para certas ações.

FILE * fword;
FILE * fprize;

int loadsrc(char vetor1[][4][17], char vetor2[][17], int id[]);								// Carrega PALAVRAS.DAT e PREMIOS.DAT para uso.
int resultado(int p1, int p2, int p3);												// Recebe o montante dos 3 jogadores, verifica o maior e retorna o número dos jogadores vitoriosos.
int comparar(char v_in[][4][17], char v_out[][17], char letraInput, int pista);		// Pesquisará dentre as palavras em jogo por indícios da letra escolhida. word_buf[] será modificada de acordo.
int pcomparar(char v_in[][4][17], char v_out[][17], char palavra[][17], int pista, int id);
int contador_(char v_out[][17]);

int loadsrc(char vetor1[][4][17], char vetor2[][17], int id[]) {
	int i, j;
	char buffer[17];														// Este vetor guarda a leitura atual antes de enviá-la à matriz, por questões de verificação.

	// Carregando dados dentro de 'palavras.dat'. Ao detectar um @, o leitor avançará para a próxima linha da matriz.
	fword = fopen("src\\palavras.dat", "r");
	fprize = fopen("src\\premios.dat", "r");
	for (i=0; i<5; i++) {
		id[i] = -1;
		for (j=0; j<5; j++) {
			fscanf(fword, "%s", buffer);
			if (strcmp(buffer, "@") == 0) {
				break;
			}
			strcpy(vetor1[i][j], buffer);
			id[i]++;
		}
	}

	// Carregando dados dentro de 'premios.dat'.
	for (i=0; i<12; i++) {
		fscanf(fprize, "%s", buffer);
		strcpy(vetor2[i], buffer);
	}
	return 0;
}

int resultado(int p1, int p2, int p3) {
	if (p1 == p2 && p1 == p3) return 123;
	else if (p1 == p2 && p1 > p3) return 12;
	else if (p1 == p3 && p1 > p2) return 13;
	else if (p2 == p3 && p2 > p1) return 23;
	else if (p1 > p2 && p1 > p3) return 1;
	else if (p2 > p1 && p2 > p3) return 2;
	else return 3;
}

int comparar(char v_in[][4][17], char v_out[][17], char letraInput, int pista) {
	// Exemplo de uso: comparar(palavras, pista, <char da letra pedida>, <pista sorteada>);
	// 0 = correto, 1 = errado, 2 = já usado.
	bool correto;
	int j, k;
	int bonus = 0;
	correto = false;
	for (j=1; j<4; j++) {
		for (k=0; k<strlen(v_in[pista][j]); k++) {
			if (v_in[pista][j][k] == letraInput) {
				if (v_out[j][k] == letraInput) {
					return -1;
				}
				v_out[j][k] = letraInput;
				correto = true;
				bonus++;
			}
		}
	}
	if (correto) {
		return bonus;
	} else {
		return 0;
	}
}

int pcomparar(char v_in[][4][17], char v_out[][17], char palavra[][17], int pista, int id) {
	int correto;
	int i, j;
	correto = 0;
	for (i=0; i<id; i++) {
		for (j=1; j<4; j++) {
			if (strcmp(palavra[i], v_in[pista][j]) == 0) {
				correto++;
			}
		}
	}
	if (correto == id) {
		for (i=0; i<id; i++) {
			for (j=1; j<4; j++) {
				if (strcmp(palavra[i], v_in[pista][j]) == 0) {
					strcpy(v_out[j], palavra[i]);
				}
			}
		}
		return 1;
	} else {
		return 0;
	}
}

int contador_(char v_out[][17]){		// checa quantidade de underlines restantes no jogo...
	int count=0;
	int j, k;
	for (j=1; j<4; j++) {
		for (k=0; k<strlen(v_out[j]); k++) {
			if (v_out[j][k] == '_') count++;
		}
	}
	return count;
}

//int verificaLetra(char letra){			// Não faz mais parte por deixar o usuário ser "burro" [=
//	letra = tolower(letra); //65 e 91
//	if (letra < 65 || letra > 91){
//		return 1;
//	}
//	return 0;
//}

int gotoxy(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
	return 0;
}

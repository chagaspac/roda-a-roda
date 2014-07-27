#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "game-core.h"

// IFSP - São Paulo
// Análise e Desenvolvimento de Sistemas - Linguagem C (1)
// Projeto Didático - 1º Semestre

// Simulação: Roda-a-Roda
// Desenvolvido por Lucas Barcellos (1363417), Pedro Chagas (1363981).

// ----- CÓDIGO FONTE - PRINCIPAL (MAIN) -----
// Código-fonte principal do programa, o que contém main(). Possui também funções de exibição de telas.

// Funções com prefixo 'scr' apagam a tela atual e imprimem sua tela correspondente, além de conterem as ações respectivas de sua tela.
int main(void);
int scrMenu(void);
int loadDebug(void);						// Função de debug. Imprime conteúdo carregado de palavra.dat e de premios.dat, junto com os nomes dos jogadores.
int rodada(void);
int sorteio(void);

char palavras[5][4][17];			// Matriz onde 'palavras.dat' é carregado. 0 = nome da pista, 1+ = palavra.
char premios[12][17];				// Matriz onde 'premios.dat' é carregado. Contém todos os prêmios possíveis.
int id[5];							// Vetor que guarda a quantidade de palavras de cada pista.
char jogador[3][11];				// Matriz que guarda o nome dos três jogadores.
int  grana[3];						// Vetor que guarda a pontuação dos jogadores.
char pista[4][17];					// Matriz que, após sorteio da pista, guarda as palavras da pista em sublinhas '_', e quaisquer letras acertadas.
char menu_opt;						// Opção do menu.
bool leave = false;					
bool primeiraRodada = true;
int endgame = 0;
int vezJogador = 0;
int randomPista;
int randomPremio;
int premioGrana;
char letrasUsadas[24];
bool tenteDenovo = false;
int dump[256];

int main() {
	int i, x;
	setlocale(LC_ALL, "Portuguese");		// Locale define o set válido de caracteres. "Portuguese" permite o uso livre de letras acentuadas.
	loadsrc(palavras, premios, id);
	while (scrMenu() != 1) {				// scrMenu retornará 0 se uma seleção do usuário for necessária. 1 quebra o loop, 2 sairá do programa.
		if (leave) return 0;
		else menu_opt = getch();
	}
	for (i=0; i<3; i++) {
		printf("\n\t JOGADOR %d >> ", i+1);
		fgets(jogador[i], 11, stdin);
		jogador[i][strcspn(jogador[i], "\n")] = '\0';
		fflush(stdin);
	}

	srand(time(NULL));						// srand() define a 'seed' utilizada para geração de valores aleatórios. time(NULL) sempre passará uma seed diferente.
	randomPista = rand() % 4;				// Sorteio da pista.

	primeiraRodada = true;
	vezJogador = 0;
	endgame = 0;
	grana[0] = 0; grana[1] = 0; grana[2] = 0; 

	while (x!=1){							// Se x=1 então acabou o jogo, 
		if (vezJogador == 3)				// Se x=0 continua cada rodada.
			vezJogador = 0;
		x = rodada();    //imprimi tela do jogo e enquanto não retornar 1, continua rodando
		vezJogador++;						
	}
	menu_opt = 0;
	main();
	return 0;
}

int scrMenu() {
	system("cls");
	// A variável 'menu_opt' exibirá uma tela para cada opção escolhida. Ao retornar das outras, menu_opt = 0 para exibir default.
	switch(menu_opt) {
		case '1': {
			system("cls");
			printf("\n\t\t\t     +-----=$$=-----+"
				   "\n\t\t\t     | Começar Jogo |"
				   "\n\t\t\t     +--------------+"
				   "\n\n     Digite o nome dos três participantes (máx. 10 letras, sem acentos).\n");
			return 1;
		} break;
		case '2': {
			printf("\n\t\t\t\t+----=$$=----+"
				   "\n\t\t\t\t| Instruções |"
				   "\n\t\t\t\t+------------+"
				   "\n\n     Roda-a-Roda é uma simulação em C do game-show Roletrando, apresentado\n     por Sílvio Santos."
				   "\n\n     O objetivo é acertar as palavras da pista que for sorteada. Durante seu\n     turno, você fará duas ações:"
				   "\n\n\t1 - Girar a roda de prêmios;\n\t2 - Escolher uma letra que possa existir nas palavras;"
				   "\n\n     Caso a sua letra faça parte das palavras, você pontuará a quantia\n     sorteada na roda. Mas cuidado: a roda não possui apenas prêmios! As\n     opções 'Passe a Vez' e 'Perde Tudo' também circulam na roda!"
				   "\n\n     Ao final do jogo (todas as palavras foram acertadas), o jogador com a\n     maior pontuação vence! Porém, isto é apenas uma simulação, portanto\n     você não receberá o valor em dinheiro. Desculpe-nos. :P"
				   "\n\n     >> Pressione qualquer tecla para voltar.");
			return 0;
		} break;
		case '3': {
			printf("\n\t\t\t\t+---=$$=---+"
				   "\n\t\t\t\t| Créditos |"
				   "\n\t\t\t\t+----------+"
				   "\n\n\n\t\t          -= Obrigado por Jogar! =-"
				   "\n\n\n     IFSP - Análise e Desenvolvimento de Sistemas - 1° Semestre"
				   "\n     Projeto Didático - Linguagem de Programação - C"
				   "\n\n     Programa desenvolvido por:"
				   "\n\n\t\tLucas Barcellos (1363417)\n\t\tPedro Augusto das Chagas (1363981)"
				   "\n\n     >> Pressione qualquer tecla para voltar.");
		} break;
		case '4': {
			leave = true;
			return 0;
		} break;
		default: {
			// Um pouco de arte em ASCII não machunca ninguém. ;P

			printf("\n\t                         *_______*"
				   "\n\t                         /\\     /\\"
				   "\n\t                        /  \\ $ /  \\"
				   "\n\t                       /  $ \\ / X  \\"
				   "\n\t                      *|-----@-----|*"
				   "\n\t                       \\  X / \\ $  /"
				   "\n\t                        \\  / $ \\  /"
				   "\n\t                         \\/_____\\/"
				   "\n\t        __               *       *  __               "
				   "\n\t        | \\        |                | \\        |     "
				   "\n\t        |_/   _   _|  _      _      |_/   _   _|  _  "
				   "\n\t        | \\  / \\ / | / \\  _ / \\  _  | \\  / \\ / | / \\ "
				   "\n\t        |  \\ \\_/ \\_| \\_|\\   \\_|\\    |  \\ \\_/ \\_| \\_|\\"
				   "\n\n\t      <=-------------------=< $ >=-------------------=>");
			printf("\n\n\t     Bem-vindo!  Selecione o número de uma opção abaixo:"
				   "\n\n\t\t\t       1. Jogar!\n\t\t\t       2. Instruções\n\t\t\t       3. Créditos\n\t\t\t       4. Sair");
		} break;
	}
}
int loadDebug() {
	// Função de debug para verificar se os arquivos carregaram corretamente.
	int i;
 	printf("\n%s %s %s", palavras[0][0], palavras[0][1], palavras[0][2]);
	printf("\n%s %s %s %s", palavras[1][0], palavras[1][1], palavras[1][2], palavras[1][3]);
	printf("\n%s %s %s %s", palavras[2][0], palavras[2][1], palavras[2][2], palavras[2][3]);
	printf("\n%s %s", palavras[3][0], palavras[3][1]);
	printf("\n%s %s %s\n\n", palavras[4][0], palavras[4][1], palavras[4][2]);
	for (i=0; i<12; i++) {
		printf("%s ", premios[i]);
	}
	printf("\n\n");
	for (i=0; i<3; i++) {
		printf("%s ", jogador[i]);
	}
	getch();
}

int rodada() {
	char letra;
	char palavraEnd[3][17];
	int i,j;
	int cmp;
	int progresso;
	int result;
	system("cls");

	gotoxy(18,3); printf("-- A pista está associada a: %s! --\n\n", palavras[randomPista][0]);

	//Sorteia o premio!!
	if (primeiraRodada == true){			//se for a primeira vez jogando, então ele só sorteia entre 0 a 9 e carrega os "_"
		for(i=1;i<4;i++){
			printf("\n\t\t\t     ");
			for (j=0; j<strlen(palavras[randomPista][i]); j++) {
				pista[i][j] = '_';
				printf("%c ", pista[i][j]);
			}
		}
	} else {									//senao ele sorteia entre 0 a 11 e printa os "_" + o que já foi acertado
		for(i=1;i<4;i++) {
			printf("\n\t\t\t     ");
			for (j=0; j<strlen(palavras[randomPista][i]); j++) {
				printf("%c ", pista[i][j]);
			}
		}
	}
	progresso = contador_(pista);
	switch (vezJogador) {
		case 0: gotoxy(4,11); break;
		case 1: gotoxy(32,11); break;
		case 2: gotoxy(60,11); break;
	}
	printf("       \\/");
	gotoxy(4,13);printf("[P1] %s",jogador[0]);  gotoxy(32,13);printf("[P2] %s",jogador[1]);   gotoxy(60,13);printf("[P3] %s",jogador[2]);
	gotoxy(4,14);printf("=--------------=");    gotoxy(32,14);printf("=--------------=");     gotoxy(60,14);printf("=--------------=");
	gotoxy(4,15);printf("R$ %d,00",grana[0]);   gotoxy(32,15);printf("R$ %d,00",grana[1]);    gotoxy(60,15);printf("R$ %d,00",grana[2]);
	printf("\n\n");
	// Final do jogo
	if (progresso <= 3 && progresso > 0 && endgame == 0) {
		endgame = 1;
		gotoxy(0,18);
		printf(	   "\t\t       @_____@    \n"
				   "\t\t       /\\   /\\    \n"
				   "\t\t     @/__\\ /__\\@  \n"
				   "\t\t      \\  / \\  /   \n"
				   "\t\t       \\/___\\/    \n"
				   "\t\t       @     @    ");
		gotoxy(36,19); printf("._.-=> RODADA ESPECIAL! <=-._.");
		gotoxy(36,20); printf("                   ");
		gotoxy(36,21); printf("Consegue adivinhar a palavra?");
		gotoxy(36,22);
		for (i=5; i>0; i--) {
			printf("%d! ", i);
			Sleep(1000);
		}
		gotoxy(36,19); printf("                                ");
		gotoxy(36,20); printf("                                ");
		gotoxy(36,21); printf("                                ");
		gotoxy(36,22); printf("                                ");
	} else if (progresso == 0) {
		gotoxy(0,18);
		printf(	   "\t\t       *_____*    \n"
				   "\t\t       /\\   /\\    \n"
				   "\t\t     */__\\ /__\\*  \n"
				   "\t\t      \\  / \\  /   \n"
				   "\t\t       \\/___\\/    \n"
				   "\t\t       *     *    ");
		gotoxy(36,19); printf("._.-=> FIM DE JOGO! <=-._.");
		gotoxy(36,20); printf("                ");
		gotoxy(4,11); printf("                                                                   ");
		Sleep(2000);
		gotoxy(36,21);
		if (endgame == 1) {
			result = vezJogador;
		} else {
			result = resultado(grana[0], grana[1], grana[2]);
		}
		switch (result) {
			case 0: {
				gotoxy(36,21); printf("Vencedor: [P%i] %s! Parabéns!", 3, jogador[3]);
			} break;
			case 1: case 2: case 3: {
				gotoxy(36,21); printf("Vencedor: [P%i] %s! Parabéns!", result, jogador[result-1]);
			} break;
			case 12: printf("Empate entre %s e %s!", jogador[0], jogador[1]); break;
			case 13: printf("Empate entre %s e %s!", jogador[0], jogador[2]); break;
			case 23: printf("Empate entre %s e %s!", jogador[1], jogador[2]); break;
			case 123: printf("Empate geral! Todos perderam!"); break;
		}
		gotoxy(36,22); printf(">> Sair");
		getch();
		return 1;
	}


	if (tenteDenovo == true) {
		gotoxy(0,18);
		printf(	   "\t\t       *_____*    \n"
				   "\t\t       /\\   /\\    \n"
				   "\t\t     */__\\ /__\\*  \n"
				   "\t\t      \\  / \\  /   \n"
				   "\t\t       \\/___\\/    \n"
				   "\t\t       *     *    ");
	}
	else{
		sorteio();
	}
	tenteDenovo = false;

	if (randomPremio < 10 && endgame == 0) {
		premioGrana = strtol(premios[randomPremio], NULL, 10); //converte string para integer
		gotoxy(36,19); printf("Valendo R$ %d.00!",premioGrana);
		gotoxy(36,20); printf("Escolha uma letra: "); 
		letra = getch(); letra = toupper(letra); printf("%c",letra);
		
		//if (verificaLetra(letra) == 1)
		//	gotoxy(15,20);printf("Letra já usada");
		cmp = comparar(palavras, pista, letra, randomPista);
		if (cmp > 0) {
			grana[vezJogador] = grana[vezJogador] + (premioGrana * cmp);
			
			gotoxy(36,22); printf("--=> LETRA CERTA! <=--");
			Sleep(2500);
			return 0;
		} else if (cmp == -1) {
			gotoxy(36,22); printf("Esta letra já foi usada! Tente de novo.");
			vezJogador--;
			Sleep(2500);
			tenteDenovo = true;
			return 0;
		} else {
			gotoxy(36,22); printf("--=> LETRA ERRADA! <=--");
			Sleep(2500);
			return 0;
		}
	} else if (randomPremio < 10 && endgame == 1) {
		premioGrana = strtol(premios[randomPremio], NULL, 10); //converte string para integer
		gotoxy(36,19); printf("Valendo R$ %d.00!",premioGrana);
		for (i=0; i<id[randomPista]; i++) {
			gotoxy(36, 20+i);
			printf("Qual a %dº palavra? >> ", i+1);
			fgets(palavraEnd[i], 17, stdin);
			palavraEnd[i][strcspn(palavraEnd[i], "\n")] = '\0';
			for (j=0; j<strlen(palavraEnd[i]); j++) {
				palavraEnd[i][j] = toupper(palavraEnd[i][j]);
			}
		}
		
		cmp = pcomparar(palavras, pista, palavraEnd, randomPista, id[randomPista]);
		if (cmp == 1) {
			grana[vezJogador] = grana[vezJogador] + premioGrana;
			gotoxy(36,23); printf("--=> ACERTOU! <=--");
			Sleep(2500);
			return 0;
		} else if (cmp == -1) {
			gotoxy(36,23); printf("Esta palavra já foi terminada.");
			vezJogador--;
			Sleep(2500);
			tenteDenovo = true;
			return 0;
		} else {
			gotoxy(36,23); printf("--=> ERROU! <=--");
			Sleep(2500);
			return 0;
		}
	} else if (randomPremio == 10) {
		gotoxy(36,20); printf("--=> PASSA A VEZ! <=--");
		Sleep(2500);
		return 0;
	} else if (randomPremio == 11) {
		grana[vezJogador] = 0;
		gotoxy(36,20); printf("--=> PERDE TUDO... <=--");
		Sleep(2500);
		return 0;
	}
	return 1;
}

int sorteio() {
	int spin;
	if (primeiraRodada) {
		randomPremio = rand() % 10;
		primeiraRodada = false;				//seta primeiraRodada para false, indicando que não é mais a primeira vez jogando...
	} 
	else randomPremio = rand() % 12;
	for (spin=0; spin<13; spin++) {
		randomPremio++;
		if (randomPremio == 12) randomPremio = 0;
		if (spin % 2 == 0) {
			gotoxy(0,18);
			printf("\t\t       *_____*    \n"
				   "\t\t       /\\   /\\    \n"
				   "\t\t     */__\\ /__\\*  \n"
				   "\t\t      \\  / \\  /   \n"
				   "\t\t       \\/___\\/    \n"
				   "\t\t       *     *    ");
		} else {
			gotoxy(0,18);
			printf("\t\t        __*__     \n"
				   "\t\t      */  |  \\*   \n"
				   "\t\t      / \\_|_/ \\   \n"
				   "\t\t      \\ / | \\ /   \n"
				   "\t\t      *\\__|__/*   \n"
				   "\t\t          *       ");
		}
		gotoxy(36,19); printf("Rodando...",premioGrana);
		gotoxy(36,20);
		if (randomPremio == 10) printf("PASSE A VEZ ");
		else if (randomPremio == 11) printf("PERDE TUDO  ");
		else printf("R$ %s,00    ", premios[randomPremio]);
		Sleep(100);
	}
}

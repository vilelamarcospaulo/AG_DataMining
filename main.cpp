#include "AG.h"
#include <iostream>
#include <iomanip> 
#include <random>
#include <chrono>

#define N_REGISTROS_TESTE 120
#define N_REGISTROS_TREINO 238

void LoadData(int **treino, int **teste) {
	FILE * pFile;
	
	pFile = fopen("Treino_DM.txt", "r");
	if (pFile != NULL)
	{
		for (int i = 0; i < N_REGISTROS_TREINO; i++) {
			for (int j = 0; j < individuo::N_ATRIBUTOS; j++) 
				fscanf(pFile, "%d,", &treino[i][j]);
			fscanf(pFile, "%d", &treino[i][individuo::N_ATRIBUTOS]);
		}
		fclose(pFile);
	}

	pFile = fopen("Teste_DM.txt", "r");
	if (pFile != NULL)
	{
		for (int i = 0; i < N_REGISTROS_TESTE; i++) {
			for (int j = 0; j < individuo::N_ATRIBUTOS; j++)
				fscanf(pFile, "%d,", &teste[i][j]);
			fscanf(pFile, "%d", &teste[i][individuo::N_ATRIBUTOS]);
		}
		fclose(pFile);
	}
}

int main(int argc, char** argv)
{
	/* APENAS LOAD DOS ARQUIVOS DE TREINO E TESTE*/
	int **treino, **teste;
	teste = new int*[N_REGISTROS_TESTE];
	treino = new int*[N_REGISTROS_TREINO];
	
	for (int i = 0; i < N_REGISTROS_TREINO; i++)
		treino[i] = new int[individuo::N_ATRIBUTOS + 1];
	for (int i = 0; i < N_REGISTROS_TESTE; i++)
		teste[i] = new int[individuo::N_ATRIBUTOS + 1];

	LoadData(treino, teste);

	std::minstd_rand0 random_generator = std::minstd_rand0(std::chrono::system_clock::now().time_since_epoch().count());
	AG::random_generator = random_generator;
	individuo::random_generator = random_generator;
	
	/*DAQUI PRA BAIXO IMPORTA*/
	int n_exec = 10; // NUMERO DE VEZES QUE O AG IRA RODAR
	AG ag = AG(50, 49, 50, modo_reinsercao::elitismo, 30, 6); //parametros = TAMANHO_POP, N_FILHOS, N_GERAÇÕES, MODO_REINSERÇÃO, PORCENTAGEM_MUTACAO, CLASSE DE TREINAMENTO

	double time_elapsed = 0;

	for (int i = 1; i <= n_exec; i++)
	{
		/*seta os parametros para treino*/
		individuo::base = treino;
		individuo::tamanho_base = N_REGISTROS_TREINO;

		std::clock_t start_aux = std::clock(); /* INICIA O RELOGIO PARA CRONOMETRAR O TEMPO GASTO */
		ag.run(); /* RODA O AG */
		double t = ((std::clock() - start_aux) / (double)(CLOCKS_PER_SEC / 1000)); /*tempo gasto para a execução*/

		individuo result = ag.melhorIndividuo(); //RECUPERA O MELHOR INDIVIDUO DA 
		std::cout << "Iteracao " << i << " resultado " << std::endl << result.toString() << std::endl;		
		std::cout << "Fitness treinamento = " << result.aptidao() << std::endl;

		individuo::base = teste;
		individuo::tamanho_base = N_REGISTROS_TESTE;

		std::cout << "Fitness teste = " << result.resetAptidao() << std::endl;
		std::cout << "Tempo gasto: " << t << " ms" << std::endl << std::endl;		
	}
	
	getchar();
	return 0;
}	
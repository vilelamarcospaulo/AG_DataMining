#include "individuo.h"
#include <iostream>
#include <iomanip>
#include <random>

enum modo_reinsercao {elitismo, ordenacao};

class AG
{
	private:
		modo_reinsercao mreinsercao;

		int i_geracao;
		int n_geracoes;
	
		int tam_populacao;         //Tamanho da população inicial
		int n_filhos;			   //Tamanho da população dos filhos
		int probabilidade_mutacao; //

		individuo* tour(int sum, int tour_size);
		individuo* roleta(int sum);
		void selecao(individuo **pai1, individuo **pai2, int aptidao_total);

		void ordenacao();
		void elitismo(int porcentagem_elitismo);
		void reinsercao();

		individuo** populacao;  //populacao do AG
	
	public:
		static std::minstd_rand0 random_generator;

		AG(int tam_populacao, int n_filhos, int n_geracoes, modo_reinsercao reinsercao, int p_mutacao, int classe); //constructor
	
		bool CondicaoParada();

		void gerarPopulacaoAleatoria();
		void run();
		void gerarNovaPopulacao();
		individuo melhorIndividuo();

		void printPopulacao();	
};

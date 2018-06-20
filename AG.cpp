#include "AG.h"

int time_elapsed = 0;
std::minstd_rand0 AG::random_generator;

AG::AG(int tam_populacao, int n_filhos, int n_geracoes, modo_reinsercao reinsercao, int p_mutacao, int classe)
{
	individuo::classe = classe;
	individuo::P_MUTACAO = p_mutacao;

	this->mreinsercao = reinsercao;

	this->n_geracoes = n_geracoes;
	this->tam_populacao = tam_populacao;
	this->n_filhos = n_filhos;
	this->probabilidade_mutacao = probabilidade_mutacao; 

	this->populacao = new individuo*[tam_populacao + n_filhos];
	for (int i = 0; i < this->tam_populacao + this->n_filhos; i++)
		this->populacao[i] = NULL;
}

void AG::gerarPopulacaoAleatoria()
{
	if (tam_populacao <= 0)
		return;

	for (int i = 0; i < this->tam_populacao; i++)
		this->populacao[i] = individuo::IndividuoAleatorio();

	std::sort(this->populacao, this->populacao + this->tam_populacao, [](individuo* a, individuo* b) { 
		return (*a).aptidao() > (*b).aptidao(); 
	});
}

void AG::printPopulacao(){
	std::cout << "POPULACAO ATUAL" << std::endl;
	for (int i = 0; i < this->tam_populacao; i++)
		std::cout << " Individuo: " << i << " - " << this->populacao[i]->toString() << std::endl;
}

bool AG::CondicaoParada(){
	return this->melhorIndividuo().aptidao() == 0 || ++this->i_geracao == this->n_geracoes + 1;
}

individuo* AG::roleta(int sum){
	int dice = random_generator() % sum;

	sum = 0;
	for (int i = 0; i < this->tam_populacao; i++)
	{
		sum += this->populacao[i]->aptidao() * 1000;
		if (dice < sum)
			return this->populacao[i];
	}

	return NULL;
}

individuo* AG::tour(int sum, int tour_size){
	individuo *best = NULL, *current = NULL;
	for (int i = 0; i < tour_size; i++) {
		current = this->roleta(sum);
		best = (best == NULL || (*current).aptidao() > (*best).aptidao()) ? current : best;
	}

	return best;
}

void AG::selecao(individuo **pai1, individuo **pai2, int aptidao_total)
{
	*pai1 = tour(aptidao_total, 3)->clone();
	*pai2 = tour(aptidao_total, 3)->clone();
}

void AG::ordenacao()
{
	std::sort(this->populacao, this->populacao + this->tam_populacao + this->n_filhos, 
		[](individuo* a, individuo* b) {
			return (*a).aptidao() > (*b).aptidao(); 
	});
}

void AG::elitismo(int porcentagem_elitismo)
{
	std::sort(this->populacao, this->populacao + this->tam_populacao, 
		[](individuo* a, individuo* b) {
			return (*a).aptidao() > (*b).aptidao();
	});  //ORDENA OS PAIS

	std::sort(this->populacao + this->tam_populacao, this->populacao + this->tam_populacao + this->n_filhos, 
		[](individuo* a, individuo* b) {
			return (*a).aptidao() > (*b).aptidao(); 
	});  //ORDENA OS FILHOS

	int posicao_filhos = ((porcentagem_elitismo * this->tam_populacao) / 100);
	int n_filhos_sobreviventes = this->tam_populacao - posicao_filhos;

	for (int i = 0; i < n_filhos_sobreviventes; i++)
	{
		/*REMOVE O Iesimo PAI*/
		this->populacao[posicao_filhos + i]->dispose();
		delete(this->populacao[posicao_filhos + i]);
		/*END*/

	
		this->populacao[posicao_filhos + i] = this->populacao[this->tam_populacao + i]; /*COLOCA O Iesimo filho no lugar do iesimo pai*/
		this->populacao[this->tam_populacao + i] = NULL; /*SETA NULL PARA O FILHO SÃO SER PEGO NOVAMENTE EM ALGUM MOMENTO*/
	}
}

void AG::reinsercao()
{
	this->mreinsercao == modo_reinsercao::ordenacao ? this->ordenacao() : this->elitismo(2);
}

void AG::gerarNovaPopulacao()
{
	int sum = 0;
	for (int i = 0; i < this->tam_populacao; i++)
	{
		sum += this->populacao[i]->aptidao() * 1000;
	}

	for (int i = this->tam_populacao; i < this->tam_populacao + this->n_filhos; i+=2)
	{
		individuo *filho1 = NULL;
		individuo *filho2 = NULL;
		this->selecao(&filho1, &filho2, sum);
		
		individuo::crossover(filho1, filho2);

		filho1->mutacao();
		filho2->mutacao();
		
		//libera a memoria dos filhos
		
		if (this->populacao[i] != NULL)
			this->populacao[i]->dispose();
		
		delete(this->populacao[i]);
		this->populacao[i] = filho1;

		if (this->tam_populacao + this->n_filhos <= i + 1) 
			continue;

		if (this->populacao[i + 1] != NULL)
			this->populacao[i + 1]->dispose();

		delete(this->populacao[i + 1]);
		this->populacao[i + 1] = filho2;
	}

	this->reinsercao();
}

void AG::run()
{
	this->i_geracao = 0;
	this->gerarPopulacaoAleatoria();
	
	while (!this->CondicaoParada())
		this->gerarNovaPopulacao();
}

individuo AG::melhorIndividuo()
{
	return *this->populacao[0];
}
#include "individuo.h"

int individuo::P_MUTACAO = 30;
int **individuo::base = NULL;
int individuo::tamanho_base = -1;
int individuo::classe = -1;

std::minstd_rand0 individuo::random_generator;

const int individuo::N_ATRIBUTOS = 34;
const float individuo::PESO_MAXIMO = .3;

individuo::individuo() 
{
	this->DNA = new gene[individuo::N_ATRIBUTOS];
}

individuo* individuo::clone(){ 
	individuo* ind = new individuo();
	
	for (int i = 0; i < individuo::N_ATRIBUTOS; i++)
	{
		(*ind).DNA[i].peso = this->DNA[i].peso;
		(*ind).DNA[i].op = this->DNA[i].op;
		(*ind).DNA[i].valor = this->DNA[i].valor;
	}			
	
	return ind;
}

bool individuo::evaluate(operador op, int value1, int value2) {
	if (op == operador::IGUAL) {
		return value1 == value2;
	} else if (op == operador::DIFERENTE) {
		return value1 != value2;
	} else if (op == operador::MENOR) {
		return value1 < value2;
	} else if (op == operador::MAIOR_IGUAL) {
		return value1 >= value2;
	}

	return false;
}

float individuo::resetAptidao() {
	this->_aptidao = -1;
	return this->aptidao();
}

float individuo::aptidao()
{
	if (this->_aptidao == -1)
	{
		int Vp = 0, Vn = 0, Fp = 0, Fn = 0;

		for (int reg = 0; reg < tamanho_base; reg++) {
			bool classificado = true;

			for (int i = 0; i < individuo::N_ATRIBUTOS && classificado; i++)
			{
				if (this->DNA[i].peso >= individuo::PESO_MAXIMO) continue; //IGNORA O GENE CASO TENHA PESO >= PESO_MAXIMO = .3
				classificado = this->evaluate(this->DNA[i].op, base[reg][i], this->DNA[i].valor);
			}

			Vp += classificado && (classe == base[reg][individuo::N_ATRIBUTOS]);
			Vn += !classificado && (classe != base[reg][individuo::N_ATRIBUTOS]);
			Fp += classificado && (classe != base[reg][individuo::N_ATRIBUTOS]);
			Fn += !classificado && (classe == base[reg][individuo::N_ATRIBUTOS]);
		}

		float Sensitividade = (Vp + 1.0) / (Vp + Fn + 1.0);
		float Especificidade = (Vn + 1.0) / (Vn + Fp + 1.0);

		_aptidao = Sensitividade * Especificidade;
	}

	return _aptidao;
}

void individuo::mutacao()
{
	for (int i = 0; i < individuo::N_ATRIBUTOS; i++) {
		if ((random_generator() % 100) >= individuo::P_MUTACAO) //SE for sorteado no intervalo [0, 2] faz mutação, caso contrario pula.
			continue;

		//Mutação dos pesos
		float dice = (random_generator() % 1001) / 1000.0;
		if ((random_generator() % 10) >= 5)
			dice *= -1;

		this->DNA[i].peso += dice;

		//Mutação dos operadores e valores
		if (i == 10 || i == 33) {
			this->DNA[i].op = (operador)!this->DNA[i].op; //É POSSIVEL APENAS O OPERADOR 0 E 1 logo basta NOT no atual teremos um op diferente;
			this->DNA[i].valor = i == 10 ? !this->DNA[i].valor : random_generator() % 100;

		} else {
			//ACHA UM OPERADOR QUE SORTEADO ENTRE 0 e 4 e diferente do atual;
			dice = (random_generator() % 4);
			for (; (operador)(int)dice == this->DNA[i].op; dice = (random_generator() % 4));
			this->DNA[i].op = (operador)(int)dice;

			//ACHA UM VALOR ENTRA [0, 3] não utilizado
			dice = (random_generator() % 4);
			for (; dice == this->DNA[i].valor; dice = (random_generator() % 4));
			this->DNA[i].valor = dice;
		}
	}
}

std::string individuo::toString()
{
	std::string str = "";
	for (int i = 0; i < individuo::N_ATRIBUTOS; i++)
	{
		if (this->DNA[i].peso >= individuo::PESO_MAXIMO) continue; //IGNORA O GENE CASO TENHA PESO >= PESO_MAXIMO = .3
		str += std::to_string(i);
		
		switch (this->DNA[i].op)
		{
			case operador::IGUAL:
				str += "==";
				break;

			case operador::DIFERENTE:
				str += "!=";
				break;

			case operador::MAIOR_IGUAL:
				str += ">=";
				break;
			
			case operador::MENOR:
				str += "<";
				break;

			default:
				break;
		}

		str += std::to_string(this->DNA[i].valor) + " // ";
	}

	return str;
}

individuo* individuo::IndividuoAleatorio(){
	individuo* ind = new individuo();
	
	for(int i = 0; i < individuo::N_ATRIBUTOS; i++){
		(*ind).DNA[i].peso = (random_generator() % 1001) / 1000.0;
		
		if (i == 10 || i == 33) {/*HISTORICO FAMILIAR [=, !=]*/
			(*ind).DNA[i].op = (operador)(random_generator() % 2);
			(*ind).DNA[i].valor = (i == 10) ? random_generator() % 2 : random_generator() % 101;
		} else {
			(*ind).DNA[i].op = (operador)(random_generator() % 4);
			(*ind).DNA[i].valor = random_generator() % 4;
		}
	}

	return ind;
}

void individuo::dispose(){
	delete[] DNA;
}

void individuo::crossover(individuo *individuo1, individuo *individuo2)
{
	int pto1 = random_generator() % individuo::N_ATRIBUTOS;
	int pto2 = random_generator() % individuo::N_ATRIBUTOS;

	if (pto2 > pto1) {
		int aux = pto1;
		pto1 = pto2;
		pto2 = aux;
	}

	for (int i = pto1; i <= pto2; i++)
	{
		gene aux = individuo1->DNA[i];
		individuo1->DNA[i] = individuo2->DNA[i];
		individuo2->DNA[i] = aux;
	}
}

bool operator==(individuo lhs, individuo rhs)
{
	for (int i = 0; i < individuo::N_ATRIBUTOS; i++)
	{
		if (lhs.DNA[i].peso	 != rhs.DNA[i].peso || lhs.DNA[i].op != rhs.DNA[i].op || lhs.DNA[i].valor != rhs.DNA[i].valor)
			return false;
	}
	return true;
}

bool operator!=(individuo lhs, individuo rhs)
{
	return !(lhs == rhs);
}

individuo individuo::ResultadoClasse1_Fidelis() {
	individuo ind = individuo();
	for (int i = 0; i < individuo::N_ATRIBUTOS; i++) {
		ind.DNA[i].peso = 1;
		ind.DNA[i].op = operador::IGUAL;
		ind.DNA[i].valor = 0;
	}

	ind.DNA[11].peso = 0;
	ind.DNA[11].op = operador::IGUAL;
	ind.DNA[11].valor = 0;

	ind.DNA[19].peso = 0;
	ind.DNA[19].op = operador::MAIOR_IGUAL;
	ind.DNA[19].valor = 1;
	
	return ind;
}

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>

class individuo
{
	private:
		const static float PESO_MAXIMO;
		
		enum operador { IGUAL, DIFERENTE, MAIOR_IGUAL, MENOR };

		float _aptidao = -1;
		bool evaluate(operador op, int value1, int value2);
				
		typedef struct Struct { float peso; operador op; int valor; } gene;
		gene* DNA;

	public:
		static int P_MUTACAO; //Probabilidade de chance de haver mutação
		const static int N_ATRIBUTOS; // = 34; 

		static std::minstd_rand0 random_generator;
	
		static int **base;
		static int tamanho_base;
		static int classe;

		individuo(); //Construtor
		individuo* clone();
		static individuo* IndividuoAleatorio();
		static individuo ResultadoClasse1_Fidelis();
		static individuo ResultadoClasse2_Fidelis();
		static individuo ResultadoClasse3_Fidelis();
		static individuo ResultadoClasse4_Fidelis();
		static individuo ResultadoClasse5_Fidelis();
		static individuo ResultadoClasse6_Fidelis();

		float resetAptidao();
		float aptidao();
		void mutacao(); //Faz a mutacao em um invididuo

		std::string toString(); //Methodo gera uma string de representa��o do objeto	
		static void crossover(individuo *individuo1, individuo *individuo2);


		void dispose();
		friend bool operator== (individuo lhs, individuo rhs);
		friend bool operator!= (individuo lhs, individuo rhs);
};
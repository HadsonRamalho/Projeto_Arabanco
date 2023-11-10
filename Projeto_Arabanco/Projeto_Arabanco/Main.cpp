#include <iostream>
#include <locale.h>
#include <string.h>

using namespace std;

/// 
/// Constantes
/// 

const int MAX_CLIENTES = 50;
int quantidadeDeClientes = 0;
 
/// 
/// Structs
/// 

struct Cliente {
	char numeroDaConta[12];
	char numeroDaAgencia[6];
	char nomeDoTitular[50];
	char cpfDoTitular[19];
	float saldoInicial;
	float saldoAtual;
};
Cliente Clientes[MAX_CLIENTES];

/// 
/// Prot�tipos
/// 
 
inline void cabecalho_menuPrincipal();
void menuPrincipal();
void switch_menuPrincipal(Cliente Clientes[], int opcaoMenu);
void cadastrarConta();
int busca(Cliente Clientes[], char campoDeBusca[], int opcaoDeBusca);
void atribuiChar(Cliente Clientes[], int indice, Cliente Temp);
void exibeConta(Cliente Clientes[], int indice);
void alterarConta(Cliente Clientes[]);

////////////////////////////////////////////////////////////////

int main() {
	// Configura a localiza��o para pt_BR (permite usar acentos no terminal)
	setlocale(LC_ALL, "pt_BR");
	menuPrincipal();
	return 0;
}

// Cabe�alho inline
inline void cabecalho_menuPrincipal() {
	cout << "**************************************" << endl
		<< "*   ARABANCO - SEMPRE COM VOC� =P    *" << endl
		<< "**************************************" << endl;
}

void menuPrincipal() {
	int opcaoMenu;
	do {
		system("CLS");
		cabecalho_menuPrincipal(); 
		cout << " Menu de op��es: " << endl
			<< " 1 | Cadastrar Conta Corrente" << endl
			<< " 2 | Alterar dados de uma Conta Corrente" << endl
			<< " 3 | Excluir uma Conta Corrente" << endl
			<< " 4 | Consultar Saldo" << endl
			<< " 5 | Fazer Dep�sito" << endl
			<< " 6 | Realizar saque" << endl
			<< " 7 | Transferir valores" << endl
			<< " 8 | Mostrar todas as contas e saldos" << endl
			<< " 9 | Emitir extrato de conta" << endl
			<< " 10 | Sair" << endl
			<< " Escolha uma op��o: ";
		cin >> opcaoMenu;
		if (opcaoMenu < 1 || opcaoMenu > 10) { // Validando a op��o escolhida
			cerr << "Op��o inv�lida. Digite novamente." << endl;
			system("PAUSE");
		}
		else {
			switch_menuPrincipal(Clientes, opcaoMenu);
		}
	} while (opcaoMenu != 10);
}

// Acessando fun��es do programa
void switch_menuPrincipal(Cliente Clientes[], int opcaoMenu) {
	system("CLS");
	switch (opcaoMenu) {
	case 1:
		cadastrarConta();
		break;

	case 2:
		alterarConta(Clientes);
		system("PAUSE");
		break;
	case 8:
		for (int i = 0; i < quantidadeDeClientes; i++) {
			exibeConta(Clientes, i);
		}
		system("PAUSE");
		break;
	}
}

// Substituir pela do Jefferson
void cadastrarConta() {
	Cliente Temp; // Tipo temporario, armazena dados para serem verificados antes de serem salvos no sistema
	cout << " 1 | Cadastrar Conta Corrente" << endl << endl;
	cout << "N�mero da Conta: ";
	cin >> Temp.numeroDaConta;
	while (busca(Clientes, Temp.numeroDaConta, 1) != -1) { //  Se retornar -1, significa que nao existe um cadastro com o campo escolhido
		cerr << "J� existe uma Conta Corrente com esse n�mero. Digite novamente: ";
		cin >> Temp.numeroDaConta;
	}
	cout << "N�mero da Ag�ncia: ";
	cin >> Temp.numeroDaAgencia;
	cout << "Nome do Titular: ";
	cin >> Temp.nomeDoTitular;
	cout << "CPF do Titular: ";
	cin >> Temp.cpfDoTitular;
	cout << "Saldo inicial: ";
	cin >> Temp.saldoInicial;
	while (Temp.saldoInicial < 0) {
		cerr << "Saldo inv�lido: O valor n�o pode ser negativo." << endl << "Digite novamente: ";
		cin >> Temp.saldoInicial;
	}
	Temp.saldoAtual = Temp.saldoInicial;
	atribuiChar(Clientes, quantidadeDeClientes, Temp); // Faz a c�pia dos dados inseridos em Temp para Clientes[]
	quantidadeDeClientes++;
}

// Usada para verificar se existem cadastros com campos iguais

/// <summary>
///  Valores para 'opcaoDeBusca': 
///		1 = N�mero da Conta
///		2 = N�mero da Ag�ncia
///		3 = Nome do Titular
///		4 = CPF do Titular
/// </summary>

int busca(Cliente Clientes[], char campoDeBusca[], int opcaoDeBusca) {
	for (int i = 0; i < quantidadeDeClientes; i++) {
		switch (opcaoDeBusca) {
		case 1:
			if (strcmp(Clientes[i].numeroDaConta, campoDeBusca) == 0) {
				return i; // Retorna a posição em que foi encontrado um cadastro
			}
			break;

		case 2:
			if (strcmp(Clientes[i].numeroDaAgencia, campoDeBusca) == 0) {
				return i;
			}
			break;
		case 3:
			if (strcmp(Clientes[i].nomeDoTitular, campoDeBusca) == 0) {
				return i;
			}
			break;
		case 4:
			if (strcmp(Clientes[i].cpfDoTitular, campoDeBusca) == 0) {
				return i;
			}
			break;
		}
	}
	return -1; //Retorna -1 se não for encontrado um cadastro com os campos buscados
}


// Faz a c�pia de campos de Temp para campos de Clientes[indice]
void atribuiChar(Cliente Clientes[], int indice, Cliente Temp) {
	strcpy(Clientes[indice].numeroDaConta, Temp.numeroDaConta);
	strcpy(Clientes[indice].numeroDaAgencia, Temp.numeroDaAgencia);
	strcpy(Clientes[indice].nomeDoTitular, Temp.nomeDoTitular);
	strcpy(Clientes[indice].cpfDoTitular, Temp.cpfDoTitular);
	Clientes[indice].saldoInicial = Temp.saldoInicial;
	Clientes[indice].saldoAtual = Temp.saldoAtual;
}

// Exibe todos os campos de uma determinada conta (pode ser usado dentro de um loop)
void exibeConta(Cliente Clientes[], int indice) {
	cout << " | N�mero da Conta: " << Clientes[indice].numeroDaConta << endl
		<< " | N�mero da Ag�ncia: " << Clientes[indice].numeroDaAgencia << endl
		<< " | Nome do Titular: " << Clientes[indice].nomeDoTitular << endl
		<< " | CPF do Titular: " << Clientes[indice].cpfDoTitular << endl
		<< " | Saldo Atual: " << Clientes[indice].saldoAtual << endl
		<< "---------------------------------" << endl;
}

void alterarConta(Cliente Clientes[]) {
	char numConta[12], numAgencia[6]; // Os campos que ser�o buscados
	int resultadoDaBusca; // Armazena o resultado da fun��o de busca
	bool validacao1 = false, validacao2 = false; // Faz a valida��o de cada um dos campos buscados
	int indiceResultado1 = -1, indiceResultado2 = -2; // Armazena o �ndice em que foi encontrado uma conta com o campo buscado

	cout << " | Insira o N�mero da Conta Corrente: ";
	cin >> numConta;
	cout << " | Insira o N�mero da Ag�ncia da Conta Corrente: ";
	cin >> numAgencia;

	resultadoDaBusca = busca(Clientes, numConta, 1); // Verifica se h� uma conta com o n�mero procurado, e armazena o resultado
	if (resultadoDaBusca != -1) {
		validacao1 = true; // Valida a busca pelo n�mero da conta
		indiceResultado1 = resultadoDaBusca; // Armazena o �ndice em que foi encontrado a conta
	}

	resultadoDaBusca = busca(Clientes, numAgencia, 2); // Faz o mesmo que o de cima, mas buscando o n�mero da ag�ncia
	if (resultadoDaBusca != -1) {
		validacao2 = true;
		indiceResultado2 = resultadoDaBusca;
	} 

	if (!validacao1 || !validacao2) { // Se alguma das valida��es falhar, exibe um erro na tela e retorna ao menu principal
		cerr << " Conta n�o encontrada! " << endl;
		return;
	}

	if (validacao1 && validacao2 && indiceResultado1 == indiceResultado2) { // Se todas as valida��es passarem, e os �ndices das contas forem os mesmos, segue com a altera��o dos dados
		cout << " -  Conta encontrada  -" << endl
			<< " | Titular Atual: " << Clientes[indiceResultado1].nomeDoTitular << endl
			<< " | CPF do Titular: " << Clientes[indiceResultado1].cpfDoTitular << endl << endl;

		cout << " | Insira o novo Nome do Titular: ";
		cin >> Clientes[indiceResultado1].nomeDoTitular;
		cout << " | Insira o novo CPF do Titular: ";
		cin >> Clientes[indiceResultado1].cpfDoTitular;
		cout << endl << " - Dados da Conta - " << endl
			<< " | N�mero da Conta Corrente: " << Clientes[indiceResultado1].numeroDaConta << endl
			<< " | N�mero da Ag�ncia: " << Clientes[indiceResultado1].numeroDaAgencia << endl
			<< " | [NOVO] Nome do Titular: " << Clientes[indiceResultado1].nomeDoTitular << endl
			<< " | [NOVO] CPF do Titular: " << Clientes[indiceResultado1].cpfDoTitular << endl;
	}
}
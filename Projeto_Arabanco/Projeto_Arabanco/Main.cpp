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
/// Protótipos
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
	// Configura a localização para pt_BR (permite usar acentos no terminal)
	setlocale(LC_ALL, "pt_BR");
	menuPrincipal();
	return 0;
}

// Cabeçalho inline
inline void cabecalho_menuPrincipal() {
	cout << "**************************************" << endl
		<< "*   ARABANCO - SEMPRE COM VOCÊ =P    *" << endl
		<< "**************************************" << endl;
}

void menuPrincipal() {
	int opcaoMenu;
	do {
		system("CLS");
		cabecalho_menuPrincipal(); 
		cout << " Menu de opções: " << endl
			<< " 1 | Cadastrar Conta Corrente" << endl
			<< " 2 | Alterar dados de uma Conta Corrente" << endl
			<< " 3 | Excluir uma Conta Corrente" << endl
			<< " 4 | Consultar Saldo" << endl
			<< " 5 | Fazer Depósito" << endl
			<< " 6 | Realizar saque" << endl
			<< " 7 | Transferir valores" << endl
			<< " 8 | Mostrar todas as contas e saldos" << endl
			<< " 9 | Emitir extrato de conta" << endl
			<< " 10 | Sair" << endl
			<< " Escolha uma opção: ";
		cin >> opcaoMenu;
		if (opcaoMenu < 1 || opcaoMenu > 10) { // Validando a opção escolhida
			cerr << "Opção inválida. Digite novamente." << endl;
			system("PAUSE");
		}
		else {
			switch_menuPrincipal(Clientes, opcaoMenu);
		}
	} while (opcaoMenu != 10);
}

// Acessando funções do programa
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
	cout << "Número da Conta: ";
	cin >> Temp.numeroDaConta;
	while (busca(Clientes, Temp.numeroDaConta, 1) != -1) { //  Se retornar -1, significa que nao existe um cadastro com o campo escolhido
		cerr << "Já existe uma Conta Corrente com esse número. Digite novamente: ";
		cin >> Temp.numeroDaConta;
	}
	cout << "Número da Agência: ";
	cin >> Temp.numeroDaAgencia;
	cout << "Nome do Titular: ";
	cin >> Temp.nomeDoTitular;
	cout << "CPF do Titular: ";
	cin >> Temp.cpfDoTitular;
	cout << "Saldo inicial: ";
	cin >> Temp.saldoInicial;
	while (Temp.saldoInicial < 0) {
		cerr << "Saldo inválido: O valor não pode ser negativo." << endl << "Digite novamente: ";
		cin >> Temp.saldoInicial;
	}
	Temp.saldoAtual = Temp.saldoInicial;
	atribuiChar(Clientes, quantidadeDeClientes, Temp); // Faz a cópia dos dados inseridos em Temp para Clientes[]
	quantidadeDeClientes++;
}

// Usada para verificar se existem cadastros com campos iguais

/// <summary>
///  Valores para 'opcaoDeBusca': 
///		1 = Número da Conta
///		2 = Número da Agência
///		3 = Nome do Titular
///		4 = CPF do Titular
/// </summary>

int busca(Cliente Clientes[], char campoDeBusca[], int opcaoDeBusca) {
	for (int i = 0; i < quantidadeDeClientes; i++) {
		switch (opcaoDeBusca) {
		case 1:
			if (strcmp(Clientes[i].numeroDaConta, campoDeBusca) == 0) {
				return i; // Retorna a posiÃ§Ã£o em que foi encontrado um cadastro
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
	return -1; //Retorna -1 se nÃ£o for encontrado um cadastro com os campos buscados
}


// Faz a cópia de campos de Temp para campos de Clientes[indice]
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
	cout << " | Número da Conta: " << Clientes[indice].numeroDaConta << endl
		<< " | Número da Agência: " << Clientes[indice].numeroDaAgencia << endl
		<< " | Nome do Titular: " << Clientes[indice].nomeDoTitular << endl
		<< " | CPF do Titular: " << Clientes[indice].cpfDoTitular << endl
		<< " | Saldo Atual: " << Clientes[indice].saldoAtual << endl
		<< "---------------------------------" << endl;
}

void alterarConta(Cliente Clientes[]) {
	char numConta[12], numAgencia[6]; // Os campos que serão buscados
	int resultadoDaBusca; // Armazena o resultado da função de busca
	bool validacao1 = false, validacao2 = false; // Faz a validação de cada um dos campos buscados
	int indiceResultado1 = -1, indiceResultado2 = -2; // Armazena o índice em que foi encontrado uma conta com o campo buscado

	cout << " | Insira o Número da Conta Corrente: ";
	cin >> numConta;
	cout << " | Insira o Número da Agência da Conta Corrente: ";
	cin >> numAgencia;

	resultadoDaBusca = busca(Clientes, numConta, 1); // Verifica se há uma conta com o número procurado, e armazena o resultado
	if (resultadoDaBusca != -1) {
		validacao1 = true; // Valida a busca pelo número da conta
		indiceResultado1 = resultadoDaBusca; // Armazena o índice em que foi encontrado a conta
	}

	resultadoDaBusca = busca(Clientes, numAgencia, 2); // Faz o mesmo que o de cima, mas buscando o número da agência
	if (resultadoDaBusca != -1) {
		validacao2 = true;
		indiceResultado2 = resultadoDaBusca;
	} 

	if (!validacao1 || !validacao2) { // Se alguma das validações falhar, exibe um erro na tela e retorna ao menu principal
		cerr << " Conta não encontrada! " << endl;
		return;
	}

	if (validacao1 && validacao2 && indiceResultado1 == indiceResultado2) { // Se todas as validações passarem, e os índices das contas forem os mesmos, segue com a alteração dos dados
		cout << " -  Conta encontrada  -" << endl
			<< " | Titular Atual: " << Clientes[indiceResultado1].nomeDoTitular << endl
			<< " | CPF do Titular: " << Clientes[indiceResultado1].cpfDoTitular << endl << endl;

		cout << " | Insira o novo Nome do Titular: ";
		cin >> Clientes[indiceResultado1].nomeDoTitular;
		cout << " | Insira o novo CPF do Titular: ";
		cin >> Clientes[indiceResultado1].cpfDoTitular;
		cout << endl << " - Dados da Conta - " << endl
			<< " | Número da Conta Corrente: " << Clientes[indiceResultado1].numeroDaConta << endl
			<< " | Número da Agência: " << Clientes[indiceResultado1].numeroDaAgencia << endl
			<< " | [NOVO] Nome do Titular: " << Clientes[indiceResultado1].nomeDoTitular << endl
			<< " | [NOVO] CPF do Titular: " << Clientes[indiceResultado1].cpfDoTitular << endl;
	}
}
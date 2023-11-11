#include <iostream>
#include <locale.h>
#include <string.h>

using namespace std;

/////////////////////////// TO-DO ////////////////////////////////
/*
         (Funcao)  |  Tarefa
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  (cadastrarConta) | Verificar se ja existe um cadastro com os dados inseridos
  (cadastrarConta) | Alterar a atribuicao direta dos valores de Temp para Cliente

*/
//////////////////////////////////////////////////////////////////

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
void realizarSaque();

////////////////////////////////////////////////////////////////

int main() {
	// Configura a localização para pt_BR.utf8
	setlocale(LC_ALL, "pt_BR");
	menuPrincipal();
	return 0;
}

// Cabeçalho inline
inline void cabecalho_menuPrincipal() {
	cout << "**************************************" << endl
		<< "*   ARABANCO - SEMPRE COM VOCE =P    *" << endl
		<< "**************************************" << endl;
}

void menuPrincipal() {
	int opcaoMenu;
	do {
		system("CLS");
		cabecalho_menuPrincipal(); 
		cout << " Menu de opcoes: " << endl
			<< " 1 | Cadastrar Conta Corrente" << endl
			<< " 2 | Alterar dados de uma Conta Corrente" << endl
			<< " 3 | Excluir uma Conta Corrente" << endl
			<< " 4 | Consultar Saldo" << endl
			<< " 5 | Fazer Deposito" << endl
			<< " 6 | Realizar saque" << endl
			<< " 7 | Transferir valores" << endl
			<< " 8 | Mostrar todas as contas e saldos" << endl
			<< " 9 | Emitir extrato de conta" << endl
			<< " 10 | Sair" << endl
			<< " Escolha uma opcao: ";
		cin >> opcaoMenu;
		if (opcaoMenu < 1 || opcaoMenu > 10) { // Validando a opção escolhida
			cerr << "Opção invalida. Digite novamente." << endl;
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
		
	case 6:
		realizarSaque();
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
	cout << "Numero da Conta: ";
	cin >> Temp.numeroDaConta;
	while (busca(Clientes, Temp.numeroDaConta, 1) != -1) { //  Se retornar -1, significa que nao existe um cadastro com o campo escolhido
		cerr << "Ja existe uma Conta Corrente com esse numero. Digite novamente: ";
		cin >> Temp.numeroDaConta;
	}
	cout << "Numero da Agencia: ";
	cin >> Temp.numeroDaAgencia;
	cout << "Nome do Titular: ";
	cin >> Temp.nomeDoTitular;
	cout << "CPF do Titular: ";
	cin >> Temp.cpfDoTitular;
	cout << "Saldo inicial: ";
	cin >> Temp.saldoInicial;
	while (Temp.saldoInicial < 0) {
		cerr << "Saldo invalido: O valor nao pode ser negativo." << endl << "Digite novamente: ";
		cin >> Temp.saldoInicial;
	}
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
	switch (opcaoDeBusca) {
	case 1:
		for (int i = 0; i < quantidadeDeClientes; i++) {
			if (strcmp(Clientes[i].numeroDaConta, campoDeBusca) == 0) {
				return i; // Retorna a posição em que foi encontrado um cadastro
			}
		}
		break;
	case 2:
		for (int i = 0; i < quantidadeDeClientes; i++) {
			if (strcmp(Clientes[i].numeroDaAgencia, campoDeBusca) == 0) {
				return i;
			}
		}
		break;
	case 3:
		for (int i = 0; i < quantidadeDeClientes; i++) {
			if (strcmp(Clientes[i].nomeDoTitular, campoDeBusca) == 0) {
				return i;
			}
		}
		break;
	case 4:
		for (int i = 0; i < quantidadeDeClientes; i++) {
			if (strcmp(Clientes[i].cpfDoTitular, campoDeBusca) == 0) {
				return i;
			}
		}
		break;
	}
	return -1; // Retorna -1 se não for encontrado um cadastro com os campos buscados
}


// Faz a cópia de campos de Temp para campos de Clientes[indice]
void atribuiChar(Cliente Clientes[], int indice, Cliente Temp) {
	/*strcpy(Clientes[indice].numeroDaConta, Temp.numeroDaConta);
	strcpy(Clientes[indice].numeroDaAgencia, Temp.numeroDaAgencia);
	strcpy(Clientes[indice].nomeDoTitular, Temp.numeroDaAgencia);
	strcp*/
	Clientes[indice] = Temp;
	Clientes[indice].saldoAtual = Temp.saldoInicial;
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

void realizarSaque(){
	//Variaveis para pegar os dados digitados pelo usuario:
	char numeroDaConta[12];
	char numeroDaAgencia[6];
	float valorSaque;
	//Variaveis para pegar retorno de funções
	int indice;
	bool valido;
	//O laço abaixo se repete até que seja digitado uma conta válida
	do{
		system("CLS");
		cout << "Por favor, digite o numero da conta: ";
		cin >> numeroDaConta;
		cout << "Por favor, digite o numero da agencia: ";
		cin >> numeroDaAgencia;
		//Abaixo a expressão lógica para saber se a conta digitada é valida ou não
		valido = busca(Clientes, numeroDaConta, 1) == busca(Clientes, numeroDaAgencia, 2) && busca(Clientes, numeroDaAgencia, 2) != -1;
		if(valido){
			//Caso seja válida, é salvo o indice da conta.
			indice = busca(Clientes, numeroDaConta, 1);
		}
		else{
			cerr << "Numero da conta e/ou da agencia incorreto! Digite novamente: " << endl;
			system("pause");
		}
	}while(!valido);
	
	//O laço abaixo se repete até que o valor sacado seja válido(maior que zero e menor que o saldo disponivel na conta)
	do{
		system("CLS");
		exibeConta(Clientes, indice);//Exibe os dados da conta digitada pelo usuario
		cout << "Digite o valor a ser sacado: ";
		cin >> valorSaque;
		//Abaixo é feita uma série de verificações para checar se o valor é valido, caso não seja, exibe uma mensagem informando o erro
		//ocorrido
		if(valorSaque < 0){
			cerr << "O valor a ser sacado nao pode ser negativo! Digite novamente: " << endl;
			system("pause");
		}
		else if(valorSaque == 0){
			cerr << "O valor a ser sacado deve ser maior que 0" << endl;
			system("pause");
		}
		else if(valorSaque > Clientes[indice].saldoAtual){
			cerr << "Valor nao disponivel na conta!" << endl;
			system("pause");
		}
	}while(valorSaque <= 0 || valorSaque > Clientes[indice].saldoAtual);
	
	//Caso passe pelas verificações acima, é decrementado do saldo atual o saque e, por fim, exibe o valor sacado e o valor restante.
	Clientes[indice].saldoAtual -= valorSaque;
	cout << "Saque de " << valorSaque << " realizado com sucesso! Valor restante na conta: " << Clientes[indice].saldoAtual << endl;
	system("pause");
}

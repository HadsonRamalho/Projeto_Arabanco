#include <iostream>
#include <locale.h>

using namespace std;

struct Cliente {
	char numeroDaConta[12];
	char numeroDaAgencia[6];
	char nomeDoTitular[50];
	char cpfDoTitular[19];
	float saldoInicial;
};

const int MAX_CLIENTES = 50;
int quantidadeDeClientes = 0;

Cliente Clientes[MAX_CLIENTES];

inline void cabecalho_menuPrincipal() {
	cout << "**************************************" << endl
		<< "*   ARABANCO - SEMPRE COM VOCÊ =P    *" << endl
		<< "**************************************" << endl;
}

void switch_menuPrincipal(int opcaoMenu) {
	switch (opcaoMenu) {
	case 1:
		system("CLS");
		Cliente Temp;
		cout << " 1 | Cadastrar Conta Corrente" << endl << endl;
		cout << "Número da Conta: ";
		cin >> Temp.numeroDaConta;
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
		Clientes[quantidadeDeClientes] = Temp;
		break;
	}
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
		if (opcaoMenu < 1 || opcaoMenu > 10) {
			cerr << "Opção inválida. Digite novamente." << endl;
			system("PAUSE");
		}
		else {
			switch_menuPrincipal(opcaoMenu);
		}
	} while (opcaoMenu != 10);
}

int main() {
	// Configura a localização para pt_BR.utf8
	setlocale(LC_ALL, "pt_BR");
	menuPrincipal();
	return 0;
}
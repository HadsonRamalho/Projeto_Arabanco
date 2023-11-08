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
		<< "*   ARABANCO - SEMPRE COM VOC� =P    *" << endl
		<< "**************************************" << endl;
}

void switch_menuPrincipal(int opcaoMenu) {
	switch (opcaoMenu) {
	case 1:
		system("CLS");
		Cliente Temp;
		cout << " 1 | Cadastrar Conta Corrente" << endl << endl;
		cout << "N�mero da Conta: ";
		cin >> Temp.numeroDaConta;
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
		Clientes[quantidadeDeClientes] = Temp;
		break;
	}
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
		if (opcaoMenu < 1 || opcaoMenu > 10) {
			cerr << "Op��o inv�lida. Digite novamente." << endl;
			system("PAUSE");
		}
		else {
			switch_menuPrincipal(opcaoMenu);
		}
	} while (opcaoMenu != 10);
}

int main() {
	// Configura a localiza��o para pt_BR.utf8
	setlocale(LC_ALL, "pt_BR");
	menuPrincipal();
	return 0;
}
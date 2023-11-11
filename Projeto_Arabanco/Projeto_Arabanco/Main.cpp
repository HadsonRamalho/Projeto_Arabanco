#include <iostream>
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

///
/// Valores para tipoDoLancamento:
///  0 = Saldo Inicial
///  1 = Transferência Enviada
///  2 = Transferência Recebida
///  3 = Depósito
///  4 = Saque
/// 

struct Cliente {
	char numeroDaConta[12];
	char numeroDaAgencia[6];
	char nomeDoTitular[50];
	char cpfDoTitular[19];
	float saldoAtual;

	int qtdLancamentos = 0;
	int EXT_quantidadeDeLancamentos[50];
	float EXT_valorDoLancamento[50];
	float EXT_saldoPosLancamento[50];
	int EXT_tipoDoLancamento[50];
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
int selecionaConta(Cliente Clientes[]);
void alterarConta(Cliente Clientes[]);
void emitirExtrato(Cliente Clientes[]);
void depositar(Cliente Clientes[]);
void realizarSaque();

////////////////////////////////////////////////////////////////

int main() {
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
			cerr << "Opcao invalida. Digite novamente." << endl;
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
	case 5:
		depositar(Clientes);
		system("PAUSE");
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
	case 9:
		emitirExtrato(Clientes);
		system("PAUSE");
		break;
	}
}

void atualizaExtrato(Cliente Clientes[], int indice, char tipoLancamento, float valorLancamento) {
	Clientes[indice].EXT_tipoDoLancamento[Clientes[indice].qtdLancamentos] = tipoLancamento;
	Clientes[indice].EXT_quantidadeDeLancamentos[Clientes[indice].qtdLancamentos] = Clientes[indice].qtdLancamentos;
	Clientes[indice].EXT_valorDoLancamento[Clientes[indice].qtdLancamentos] = valorLancamento;
	Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].qtdLancamentos] = Clientes[indice].saldoAtual;
	Clientes[indice].qtdLancamentos++;
}

// Substituir pela do Jefferson
void cadastrarConta() {
	Cliente Temp; // Tipo temporario, armazena dados para serem verificados antes de serem salvos no sistema
	cout << " 1 | Cadastrar Conta Corrente" << endl << endl;
	cout << "Numero da Conta: ";
	cin >> Temp.numeroDaConta;
	while (busca(Clientes, Temp.numeroDaConta, 1) != -1) { //  Se retornar -1, significa que nao existe um cadastro com o campo escolhido
		system("CLS");
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
	cin >> Temp.saldoAtual;
	while (Temp.saldoAtual < 0) {
		cerr << "Saldo invalido: O valor nao pode ser negativo." << endl << "Digite novamente: ";
		cin >> Temp.saldoAtual;
	}
	atribuiChar(Clientes, quantidadeDeClientes, Temp); // Faz a cópia dos dados inseridos em Temp para Clientes[]
	atualizaExtrato(Clientes, quantidadeDeClientes, 0, Temp.saldoAtual);
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
	Clientes[indice].saldoAtual = Temp.saldoAtual;
}

// Exibe todos os campos de uma determinada conta (pode ser usado dentro de um loop)
void exibeConta(Cliente Clientes[], int indice) {
	cout << " | Numero da Conta: " << Clientes[indice].numeroDaConta << endl
		<< " | Numero da Agencia: " << Clientes[indice].numeroDaAgencia << endl
		<< " | Nome do Titular: " << Clientes[indice].nomeDoTitular << endl
		<< " | CPF do Titular: " << Clientes[indice].cpfDoTitular << endl
		<< " | Saldo Atual: " << Clientes[indice].saldoAtual << endl
		<< "---------------------------------" << endl;
}

int selecionaConta(Cliente Clientes[]) {
	char numConta[12], numAgencia[6]; // Os campos que serão buscados
	int resultadoDaBusca; // Armazena o resultado da função de busca
	bool validacao1 = false, validacao2 = false; // Faz a validação de cada um dos campos buscados
	int indiceResultado1 = -1, indiceResultado2 = -2; // Armazena o índice em que foi encontrado uma conta com o campo buscado

	cout << " | Insira o Numero da Conta Corrente: ";
	cin >> numConta;
	cout << " | Insira o Numero da Agencia da Conta Corrente: ";
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
	if (!validacao1 || !validacao2) { // Se alguma das validações falhar, retorna -1a
		return -1;
	}

	if (validacao1 && validacao2 && indiceResultado1 == indiceResultado2) {
		return indiceResultado1;
	}// Se todas as validações passarem e os índices das contas forem os mesmos, segue com a alteração dos dados

	return -1;
}

void alterarConta(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes);
	while (indice == -1){
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente." << endl;
		indice = selecionaConta(Clientes);
	}
	cout << " ---  Conta encontrada  ---" << endl
		<< " | Titular Atual: " << Clientes[indice].nomeDoTitular << endl
		<< " | CPF do Titular: " << Clientes[indice].cpfDoTitular << endl << endl;
	cout << " | Insira o novo Nome do Titular: ";
	cin >> Clientes[indice].nomeDoTitular;
	cout << " | Insira o novo CPF do Titular: ";
	cin >> Clientes[indice].cpfDoTitular;
	cout << endl << " - Dados da Conta - " << endl
		<< " | Número da Conta Corrente: " << Clientes[indice].numeroDaConta << endl
		<< " | Número da Agencia: " << Clientes[indice].numeroDaAgencia << endl
		<< " | [NOVO] Nome do Titular: " << Clientes[indice].nomeDoTitular << endl
		<< " | [NOVO] CPF do Titular: " << Clientes[indice].cpfDoTitular << endl;
}

void depositar(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes);
	float valorDeposito;
	if (indice == -1) {
		cerr << " Conta nao encontrada! " << endl;
		return; // Sai da função se não encontrar
	}
	exibeConta(Clientes, indice);
	cout << " | Digite o valor a ser depositado: ";
	cin >> valorDeposito;
	if (valorDeposito > 0) {
		Clientes[indice].EXT_tipoDoLancamento[Clientes[indice].qtdLancamentos] = 3;
		Clientes[indice].saldoAtual += valorDeposito;
		Clientes[indice].EXT_quantidadeDeLancamentos[Clientes[indice].qtdLancamentos] = Clientes[indice].qtdLancamentos;
		Clientes[indice].EXT_valorDoLancamento[Clientes[indice].qtdLancamentos] = valorDeposito;
		Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].qtdLancamentos] = Clientes[indice].saldoAtual;
		Clientes[indice].qtdLancamentos++;
		cout << " --- Deposito efetuado com sucesso --- " << endl;
	}
	else {
		cerr << " Valor do deposito nao pode ser igual ou menor do que zero!" << endl;
	}
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
		cout << " | Insira o Numero da Conta Corrente: ";
		cin >> numeroDaConta;
		cout << " | Insira o Numero da Agencia: ";
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
		cout << " | Digite o valor a ser sacado: ";
		cin >> valorSaque;
		//Abaixo é feita uma série de verificações para checar se o valor é valido, caso não seja, exibe uma mensagem informando o erro
		//ocorrido
		if(valorSaque < 0){
			cerr << " O valor a ser sacado nao pode ser negativo! Digite novamente: " << endl;
			system("pause");
		}
		else if(valorSaque == 0){
			cerr << " O valor a ser sacado deve ser maior que 0" << endl;
			system("pause");
		}
		else if(valorSaque > Clientes[indice].saldoAtual){
			cerr << " Valor nao disponivel na conta!" << endl;
			system("pause");
		}
	}while(valorSaque <= 0 || valorSaque > Clientes[indice].saldoAtual);

	//Caso passe pelas verificações acima, é decrementado do saldo atual o saque e, por fim, exibe o valor sacado e o valor restante.
	Clientes[indice].saldoAtual -= valorSaque;
	cout << " --- Saque de " << valorSaque << " realizado com sucesso!  Valor restante na conta: " << Clientes[indice].saldoAtual << " --- " << endl;

	Clientes[indice].EXT_tipoDoLancamento[Clientes[indice].qtdLancamentos] = 4;
	Clientes[indice].EXT_quantidadeDeLancamentos[Clientes[indice].qtdLancamentos] = Clientes[indice].qtdLancamentos;
	Clientes[indice].EXT_valorDoLancamento[Clientes[indice].qtdLancamentos] = valorSaque;
	Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].qtdLancamentos] = Clientes[indice].saldoAtual;
	Clientes[indice].qtdLancamentos++;
}

void emitirExtrato(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes);
	if (indice == -1) {
		cerr << " Conta nao encontrada! " << endl;
		return;
	}
	exibeConta(Clientes, indice);
	cout << " - Extrato da Conta - " << endl;
	int tipoLancamento = 0;
	for (int i = 0; i < Clientes[indice].qtdLancamentos; i++) {
		cout << " | Tipo de Lancamento: ";
		switch (Clientes[indice].EXT_tipoDoLancamento[i]) {
		case 0:
			cout << " Saldo Inicial";
			break;
		case 1:
			cout << " Transferencia Enviada";
			break;
		case 2:
			cout << " Transferencia Recebida";
			break;
		case 3:
			cout << " Deposito";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 4:
			cout << " Saque";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		}
		cout << " | Valor do Lancamento: ";
		switch (tipoLancamento) {
		case 4:
			cout << "-";
			break;
		}
		cout << "R$";
		cout << Clientes[indice].EXT_valorDoLancamento[Clientes[indice].EXT_quantidadeDeLancamentos[i]];
		cout << " | Saldo: " << Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].EXT_quantidadeDeLancamentos[i]] << endl;
	}
}
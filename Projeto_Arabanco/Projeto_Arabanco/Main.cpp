#include <iostream>
#include <string.h>
#include <fstream>
#include <windows.h>

using namespace std;

/// 
/// Constantes
/// 

const int MAX_CLIENTES = 50;
int quantidadeDeClientes = 0;

#ifndef UNLEN
#define UNLEN 256
#endif

extern "C" {
	WINBASEAPI BOOL WINAPI GetUserNameA(LPSTR, LPDWORD);
}

/// 
/// Structs
/// 

///
/// Valores para tipoDoLancamento:
///  0 = Saldo Inicial
///  1 = Transfer�ncia Enviada
///  2 = Transfer�ncia Recebida
///  3 = Dep�sito
///  4 = Saque
/// 

struct Cliente {
	char numeroDaConta[12];
	char numeroDaAgencia[6];
	char nomeDoTitular[50];
	char cpfDoTitular[19];
	float saldoAtual;

	int qtdLancamentos = 0; // Quantidade total de lan�amentos em uma conta
	int EXT_quantidadeDeLancamentos[50]; // ATEN��O: Esses vetores sincronizam os valores e tipos do extrato
	float EXT_valorDoLancamento[50];
	float EXT_saldoPosLancamento[50];
	int EXT_tipoDoLancamento[50];
};

/// 
/// Prot�tipos
/// 

inline void cabecalho_menuPrincipal();
void menuPrincipal();
void switch_menuPrincipal(Cliente Clientes[], int opcaoMenu);
void cadastrarConta(Cliente Clientes[]);
int busca(Cliente Clientes[], char campoDeBusca[], int opcaoDeBusca);
void atribuiChar(Cliente Clientes[], int indice, Cliente Temp);
void exibeConta(Cliente Clientes[], int indice);
int selecionaConta(Cliente Clientes[]);
void alterarConta(Cliente Clientes[]);
void emitirExtrato(Cliente Clientes[]);
void depositar(Cliente Clientes[]);
void realizarSaque(Cliente Clientes[]);
void atualizaExtrato(Cliente Clientes[], int indice, char tipoLancamento, float valorLancamento);
void tranferirValores(Cliente Clientes[]);
void userName(char diretorioDeCriacao[]);
void geraDiretorio(char username[], char diretorioDeCriacao[]);
void preparaExtrato(Cliente Clientes[]);

ofstream fout;

////////////////////////////////////////////////////////////////



int main() {
	menuPrincipal();
	return 0;
}

// Cabe�alho inline
inline void cabecalho_menuPrincipal() {
	cout << "**************************************" << endl
		<< "*   ARABANCO - SEMPRE COM VOCE =P    *" << endl
		<< "**************************************" << endl;
}

void menuPrincipal() {
	int opcaoMenu;
	Cliente Clientes[MAX_CLIENTES];
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
		if (opcaoMenu < 1 || opcaoMenu > 11) { // Validando a op��o escolhida
			cerr << "Opcao invalida. Digite novamente." << endl;
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
		cadastrarConta(Clientes);
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
		realizarSaque(Clientes);
		break;
	case 7:
		tranferirValores(Clientes);
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
	case 11:
		cout << "Extrato em arquivo";
		preparaExtrato(Clientes);
		break;
	}
}

void cadastrarConta(Cliente Clientes[]) {
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
	while (busca(Clientes, Temp.numeroDaAgencia, 2) != -1) { //  Se retornar -1, significa que nao existe um cadastro com o campo escolhido
		system("CLS");
		cerr << "Ja existe uma conta com esse numero de Agencia. Digite novamente: ";
		cin >> Temp.numeroDaAgencia;
	}
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
	atribuiChar(Clientes, quantidadeDeClientes, Temp); // Faz a c�pia dos dados inseridos em Temp para Clientes[]
	atualizaExtrato(Clientes, quantidadeDeClientes, 0, Temp.saldoAtual);
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
				return i; // Retorna a posi��o em que foi encontrado um cadastro
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
	return -1; //Retorna -1 se n�o for encontrado um cadastro com os campos buscados
}

// Faz a c�pia de campos de Temp para campos de Clientes[indice]
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
	char numConta[12], numAgencia[6]; // Os campos que ser�o buscados
	int resultadoDaBusca; // Armazena o resultado da fun��o de busca
	bool validacao1 = false, validacao2 = false; // Faz a valida��o de cada um dos campos buscados
	int indiceResultado1 = -1, indiceResultado2 = -2; // Armazena o �ndice em que foi encontrado uma conta com o campo buscado

	cout << " | Insira o Numero da Conta Corrente: ";
	cin >> numConta;
	cout << " | Insira o Numero da Agencia da Conta Corrente: ";
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
	if (!validacao1 || !validacao2) { // Se alguma das valida��es falhar, retorna -1
		return -1;
	}

	if (validacao1 && validacao2 && indiceResultado1 == indiceResultado2) {
		return indiceResultado1;
	}// Se todas as valida��es passarem e os �ndices das contas forem os mesmos, segue com a altera��o dos dados

	return -1;
}

void alterarConta(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes);
	while (indice == -1) {
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
		<< " | N�mero da Conta Corrente: " << Clientes[indice].numeroDaConta << endl
		<< " | N�mero da Agencia: " << Clientes[indice].numeroDaAgencia << endl
		<< " | [NOVO] Nome do Titular: " << Clientes[indice].nomeDoTitular << endl
		<< " | [NOVO] CPF do Titular: " << Clientes[indice].cpfDoTitular << endl;
}

void depositar(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes);
	float valorDeposito;
	while (indice == -1) {
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente. " << endl;
		indice = selecionaConta(Clientes);
	}
	exibeConta(Clientes, indice);
	cout << " | Digite o valor a ser depositado: ";
	cin >> valorDeposito;
	while (valorDeposito <= 0) { // Proibindo depositos iguais a zero ou negativos
		system("CLS");
		cerr << " Valor do deposito nao pode ser menor ou igual a zero! Digite novamente." << endl;
		cin >> valorDeposito;
	}
	Clientes[indice].saldoAtual += valorDeposito;
	atualizaExtrato(Clientes, indice, 3, valorDeposito); // Atualizando o extrato do cliente
	cout << " --- Deposito efetuado com sucesso --- " << endl;
}

void realizarSaque(Cliente Clientes[]) {
	//Variaveis para pegar os dados digitados pelo usuario:
	char numeroDaConta[12];
	char numeroDaAgencia[6];
	float valorSaque;
	//Variaveis para pegar retorno de fun��es
	int indice;
	bool valido;
	//O la�o abaixo se repete at� que seja digitado uma conta v�lida
	do {
		system("CLS");
		cout << " | Insira o Numero da Conta Corrente: ";
		cin >> numeroDaConta;
		cout << " | Insira o Numero da Agencia: ";
		cin >> numeroDaAgencia;
		//Abaixo a express�o l�gica para saber se a conta digitada � valida ou n�o
		valido = busca(Clientes, numeroDaConta, 1) == busca(Clientes, numeroDaAgencia, 2) && busca(Clientes, numeroDaAgencia, 2) != -1;
		if (valido) {
			//Caso seja v�lida, � salvo o indice da conta.
			indice = busca(Clientes, numeroDaConta, 1);
		}
		else {
			cerr << "Numero da conta e/ou da agencia incorreto! Digite novamente: " << endl;
			system("pause");
		}
	} while (!valido);

	//O la�o abaixo se repete at� que o valor sacado seja v�lido(maior que zero e menor que o saldo disponivel na conta)
	do {
		system("CLS");
		exibeConta(Clientes, indice);//Exibe os dados da conta digitada pelo usuario
		cout << " | Digite o valor a ser sacado: ";
		cin >> valorSaque;
		//Abaixo � feita uma s�rie de verifica��es para checar se o valor � valido, caso n�o seja, exibe uma mensagem informando o erro
		//ocorrido
		if (valorSaque < 0) {
			cerr << " O valor a ser sacado nao pode ser negativo! Digite novamente: " << endl;
			system("pause");
		}
		else if (valorSaque == 0) {
			cerr << " O valor a ser sacado deve ser maior que 0" << endl;
			system("pause");
		}
		else if (valorSaque > Clientes[indice].saldoAtual) {
			cerr << " Valor nao disponivel na conta!" << endl;
			system("pause");
		}
	} while (valorSaque <= 0 || valorSaque > Clientes[indice].saldoAtual);

	//Caso passe pelas verifica��es acima, � decrementado do saldo atual o saque e, por fim, exibe o valor sacado e o valor restante.
	Clientes[indice].saldoAtual -= valorSaque;
	cout << " --- Saque de " << valorSaque << " realizado com sucesso!  Valor restante na conta: " << Clientes[indice].saldoAtual << " --- " << endl;

	atualizaExtrato(Clientes, indice, 4, valorSaque);
}

// Atualiza o extrato de acordo com os tipos de lan�amentos definidos no topo do c�digo
void atualizaExtrato(Cliente Clientes[], int indice, char tipoLancamento, float valorLancamento) {
	Clientes[indice].EXT_tipoDoLancamento[Clientes[indice].qtdLancamentos] = tipoLancamento;
	Clientes[indice].EXT_quantidadeDeLancamentos[Clientes[indice].qtdLancamentos] = Clientes[indice].qtdLancamentos;
	Clientes[indice].EXT_valorDoLancamento[Clientes[indice].qtdLancamentos] = valorLancamento;
	Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].qtdLancamentos] = Clientes[indice].saldoAtual;
	Clientes[indice].qtdLancamentos++;
}

// Cria o extrato em um arquivo HTML
void geraHtml(Cliente Clientes[], int indice) {
	int Lancamento = 0;
	float valLancamento = 0;
	float salLancamento = 0;
	if (!fout.is_open()) {
		cerr << "Erro ao abrir o arquivo." << endl;
		return;
	}
	fout << "<html lang=\"pt-br\">";
	fout << "<head>"
		<< "<title> Extrato </title>";
	
	// O estilo da p�gina gerada, incluindo fontes e cores
	fout << "<style>"
		<< "body {"
		<< "font-family: 'Arial', 'Helvetica', sans-serif;"
		<< "margin: 0;"
		<< "display: flex;"
		<< "flex-wrap: wrap;"
		<< "justify-content: center;"
		<< "align-items: center;"
		<< "height: 100vh;"
		<< "background-color: #80c2a0;"
		<< "}"

		<< ".card {"
		<< "background-color: #ffffff; /* Cor de fundo branco */"
		<< "border-radius: 10px;"
		<< "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
		<< "margin: 10px;"
		<< "padding: 20px;"
		<< "box-sizing: border-box;"
		<< "}"

		<< ".auto-width {"
		<< "width: auto !important;"
		<< "}"

		<< "table {"
		<< "width: 100%;"
		<< "background-color: #ffffff; /* Cor de fundo branco */"
		<< "margin-top: 20px;"
		<< "}"

		<< "td {"
		<< "padding: 16px;"
		<< "}"
		<< "</style>";



	fout << "</head>";
	fout << "<body>"
		<< "<div class='card  auto-width'>" // Alterar isso pra ficar do tamanho certo
		<< "<h1 style='color: rgb(61, 130, 90);'>" << "Nome: " << Clientes[indice].nomeDoTitular << "    Agencia/Conta:  " << Clientes[indice].numeroDaAgencia << "/" << Clientes[indice].numeroDaConta
		<< " <p> Data        Horario"  "</h1>"
		<< "<table border='2'>";

	fout << "<thead>" <<
		"<tr>" <<
		"<th>Data</th>" <<
		"<th>Hora</th>" <<
		"<th>Tipo de Lan�amento</th>" <<
		"<th>Valor(R$) </th>" <<
		"<th>Saldo(R$) </th>" <<
		"</tr>" <<
		"</thead>";
	fout << "<tbody>";
	for (int i = 0; i < Clientes[indice].qtdLancamentos; i++) {
		switch (Clientes[indice].EXT_tipoDoLancamento[i]) {
		case 0: case 2: case 3:
			fout << "<tr style = 'font-weight: bold; color: green;'>";
			break;
		case 1: case 4:
			fout << "<tr style='font-weight: bold; color: red;'>";
		}

		fout << "<td>" <<  "DATA" << "</td>"; // Alterar para data
		fout << "<td>" << "HORA " << "</td>"; // Alterar para hora

		switch (Clientes[indice].EXT_tipoDoLancamento[i]) {
		case 0:
			fout << "<td> Saldo Inicial </td>";
			break;
		case 1:
			fout << "<td> Transfer�ncia Enviada </td>";
			break;
		case 2:
			fout << "<td>  Transfer�ncia Recebida </td>";
			break;
		case 3:
			fout << "<td>  Dep�sito</td>";
			break;
		case 4:
			fout << "<td> Saque </td>";
			break;
		default:
			fout << "<td> ? </td>";
		}

		fout << "<td>" << Clientes[indice].EXT_valorDoLancamento[i] << "</td>";
		fout << "<td>" << Clientes[indice].EXT_saldoPosLancamento[i] << "</td>"; 
		fout << "</tr>";
	}
	fout << "</tbody>"
		<< "</table>"
		<< "</div>"
		<< "</body>";
	fout << "</html>";
}

// Altera a string, adicionando o nome do usu�rio
void getUserName(char diretorioDeCriacao[]) {
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;

	if (!GetUserNameA(username, &username_len)) {
		cerr << "Erro ao obter o nome do usuario" << endl;
		return;
	}
	strcpy(diretorioDeCriacao, username);
	geraDiretorio(username, diretorioDeCriacao);
}

// Define todo o caminho onde o arquivo ser� salvo
void geraDiretorio(char username[], char diretorioDeCriacao[]) {
	strcpy(diretorioDeCriacao, "");
	strcat(diretorioDeCriacao, "C:\\Users\\");
	strcat(diretorioDeCriacao, username);
	strcat(diretorioDeCriacao, "\\Desktop\\Extrato.html");
}

// Come�a a preparar a vari�vel que vai armazenar o diret�rio onde o arquivo ser� salvo
void preparaExtrato(Cliente Clientes[]) {

	int indice = selecionaConta(Clientes);
	while (indice == -1) {
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente." << endl;
		indice = selecionaConta(Clientes);
	}

	char diretorioDeCriacao[150];
	getUserName(diretorioDeCriacao); // A fun��o getUserName altera a string

	fout.open(diretorioDeCriacao);
	geraHtml(Clientes, indice); // A fun��o que vai escrever as informa��es no arquivo
	if (!fout) {
		cerr << "Erro ao abrir o arquivo" << endl;
		return;
	}
	fout.close(); // Fecha e salva o arquivo gerado

	system(diretorioDeCriacao); // Abre o Extrato no navegador padr�o 
}

// Exibe o extrato na tela
void emitirExtrato(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes); // Procurando a conta do cliente
	while (indice == -1) {
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente.";
		indice = selecionaConta(Clientes);
	}
	exibeConta(Clientes, indice);
	cout << " --- Extrato da Conta --- " << endl;
	int tipoLancamento = 0;
	for (int i = 0; i < Clientes[indice].qtdLancamentos; i++) {
		cout << " | Tipo de Lancamento: ";
		switch (Clientes[indice].EXT_tipoDoLancamento[i]) { // L� o vetor do extrato contendo o tipo de lan�amento
		case 0:
			cout << " Saldo Inicial";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 1:
			cout << " Transferencia Enviada";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 2:
			cout << " Transferencia Recebida";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
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
		switch (tipoLancamento) { // Caso seja decrementado um valor (saque ou transfer�ncia enviada)
		case 1:
			cout << "-";
			break;
		case 4:
			cout << "-";
			break;
		}
		cout << "R$";
		cout << Clientes[indice].EXT_valorDoLancamento[Clientes[indice].EXT_quantidadeDeLancamentos[i]]; // Exibe o valor do lan�amento
		cout << " | Saldo: R$" << Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].EXT_quantidadeDeLancamentos[i]] << endl; // Exibe o saldo ap�s o lan�amento ter sido realizado
	}
}

// Transfere de uma conta para outra
void tranferirValores(Cliente Clientes[]) {
	int indice1 = selecionaConta(Clientes);
	while (indice1 == -1) {
		system("CLS");
		cerr << "Conta nao encontrada! Digite novamente.";
		indice1 = selecionaConta(Clientes); // Armazena a localiza��o da conta de origem
	}
	cout << " Agora selecione a conta de destino: " << endl;
	int indice2 = selecionaConta(Clientes); // Localiza��o da conta de destino
	while (indice2 == -1) {
		system("CLS");
		cerr << "Conta nao encontrada! Digite novamente.";
		indice2 = selecionaConta(Clientes);
	}
	cout << "Conta de origem:";
	exibeConta(Clientes, indice1); // Exibe cada conta separadamente
	cout << "Conta de destino:";
	exibeConta(Clientes, indice2);
	cout << "Digite o valor a ser transferido:";
	float valorTransferencia;
	do { // Reutilizado da fun��o de Saque
		//Abaixo � feita uma s�rie de verifica��es para checar se o valor � valido, caso n�o seja, exibe uma mensagem informando o erro
		//ocorrido
		cin >> valorTransferencia;
		if (valorTransferencia < 0) {
			system("CLS");
			cerr << " O valor a ser transferido nao pode ser negativo! Digite novamente: " << endl;
		}
		else if (valorTransferencia == 0) {
			system("CLS");
			cerr << " O valor a ser transferido deve ser maior que 0" << endl;
		}
		else if (valorTransferencia > Clientes[indice1].saldoAtual) {
			cerr << " Valor nao disponivel na conta!" << endl;
			system("pause");
			system("CLS");
		}
	} while (valorTransferencia <= 0 || valorTransferencia > Clientes[indice1].saldoAtual); // Verificando se o valor inserido � v�lido
	Clientes[indice1].saldoAtual -= valorTransferencia; // Subtrai o valor transferido da conta atual
	Clientes[indice2].saldoAtual += valorTransferencia; // Soma o valor � conta de destino
	atualizaExtrato(Clientes, indice1, 1, valorTransferencia); // Atualiza o extrato com a op��o de transfer�ncia enviada
	atualizaExtrato(Clientes, indice2, 2, valorTransferencia); // Atualzia o extrato com a op��o de transfer�ncia recebida
	cout << " --- Transferencia concluida ---" << endl;
	system("PAUSE");
}
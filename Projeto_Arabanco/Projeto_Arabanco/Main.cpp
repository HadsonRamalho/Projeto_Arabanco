#include <iostream>
#include <string.h>
#include <fstream>
#include <windows.h>
#include <iomanip>

using namespace std;

/// 
/// Constantes
/// 

const int MAX_CLIENTES = 50;
int quantidadeDeClientes = 0;
bool existeCadastro = false;

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

	int qtdLancamentos = 0; // Quantidade total de lançamentos em uma conta
	int EXT_quantidadeDeLancamentos[MAX_CLIENTES]; // ATENÇÃO: Esses vetores sincronizam os valores e tipos do extrato
	float EXT_valorDoLancamento[MAX_CLIENTES];
	float EXT_saldoPosLancamento[MAX_CLIENTES];
	int EXT_tipoDoLancamento[MAX_CLIENTES];
};

/// 
/// Protótipos
/// 

inline void cabecalho_menuPrincipal();
void menuPrincipal();
void switch_menuPrincipal(Cliente Clientes[], int opcaoMenu);
void cadastrarConta(Cliente Clientes[]);
int busca(Cliente Clientes[], char campoDeBusca[], int opcaoDeBusca);
void excluirConta(Cliente Clientes[], int& quantidadeDeClientes);
void copiaDados(Cliente Clientes[], int indice, Cliente Temp);
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
void dataAtual(int tipo = 1);
void consultarSaldo(Cliente Clientes[]);
void menuExtrato(Cliente Clientes[]);

ofstream fout;

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
	Cliente *Clientes = new Cliente[MAX_CLIENTES];
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
	delete[] Clientes;
}

// Acessando funções do programa
void switch_menuPrincipal(Cliente Clientes[], int opcaoMenu) {
	system("CLS");
	switch (opcaoMenu) {
	case 1:
		cadastrarConta(Clientes);
		existeCadastro = true;
		break;
	case 2:
		alterarConta(Clientes);
		system("PAUSE");
		break;
	case 3:
		excluirConta(Clientes, quantidadeDeClientes);
		break;
	case 4:
		consultarSaldo(Clientes);
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
		if (!existeCadastro && quantidadeDeClientes == 0) {
			cerr << "Nao existem clientes cadastrados" << endl;			
			system("PAUSE");
			break;
		}
		for (int i = 0; i < quantidadeDeClientes; i++) {
			exibeConta(Clientes, i);
		}
		cout << quantidadeDeClientes;
		system("PAUSE");
		break;
	case 9:
		menuExtrato(Clientes);
		system("PAUSE");
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
	copiaDados(Clientes, quantidadeDeClientes, Temp); // Faz a cópia dos dados inseridos em Temp para Clientes[]
	atualizaExtrato(Clientes, quantidadeDeClientes, 0, Temp.saldoAtual);
	quantidadeDeClientes++;
}

// Usada para verificar se existem cadastros com campos iguais
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
void excluirConta(Cliente Clientes[], int& quantidadeDeClientes) {
	system("cls");
	char numeroDaConta[12];
	char numeroDaAgencia[6];

	cout << "Excluindo conta...\n";
	cout << "Informe o numero da conta: ";
	cin >> numeroDaConta;
	cout << "Agora informe o numero da agencia: ";
	cin >> numeroDaAgencia;

	int buscarNumero = busca(Clientes, numeroDaConta, 1);

	if (buscarNumero == -1) {
		cout << "Conta nao encontrada!\n";
		cout << "operaçao finalizada!";
		system("pause");
	}
	else {
		for (int i = buscarNumero; i < quantidadeDeClientes; i++) {
			Clientes[i] = Clientes[i + 1]; // Movendo as contas para trás
			quantidadeDeClientes--;
		}
		cout << "Conta Deletada com sucesso." << endl;
		system("pause");
	}
	if (quantidadeDeClientes == 0) {
		existeCadastro = false;
	}
}
// Faz a cópia de campos de Temp para campos de Clientes[indice]
void copiaDados(Cliente Clientes[], int indice, Cliente Temp) {
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
	if (!validacao1 || !validacao2) { // Se alguma das validações falhar, retorna -1
		return -1;
	}

	if (validacao1 && validacao2 && indiceResultado1 == indiceResultado2) {
		return indiceResultado1;
	}// Se todas as validações passarem e os índices das contas forem os mesmos, segue com a alteração dos dados

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
		<< " | Número da Conta Corrente: " << Clientes[indice].numeroDaConta << endl
		<< " | Número da Agencia: " << Clientes[indice].numeroDaAgencia << endl
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
	//Variaveis para pegar retorno de funções
	int indice;
	bool valido;
	//O laço abaixo se repete até que seja digitado uma conta válida
	do {
		system("CLS");
		cout << " | Insira o Numero da Conta Corrente: ";
		cin >> numeroDaConta;
		cout << " | Insira o Numero da Agencia: ";
		cin >> numeroDaAgencia;
		//Abaixo a expressão lógica para saber se a conta digitada é valida ou não
		valido = busca(Clientes, numeroDaConta, 1) == busca(Clientes, numeroDaAgencia, 2) && busca(Clientes, numeroDaAgencia, 2) != -1;
		if (valido) {
			//Caso seja válida, é salvo o indice da conta.
			indice = busca(Clientes, numeroDaConta, 1);
		}
		else {
			cerr << "Numero da conta e/ou da agencia incorreto! Digite novamente: " << endl;
			system("pause");
		}
	} while (!valido);

	//O laço abaixo se repete até que o valor sacado seja válido(maior que zero e menor que o saldo disponivel na conta)
	do {
		system("CLS");
		exibeConta(Clientes, indice);//Exibe os dados da conta digitada pelo usuario
		cout << " | Digite o valor a ser sacado: ";
		cin >> valorSaque;
		//Abaixo é feita uma série de verificações para checar se o valor é valido, caso não seja, exibe uma mensagem informando o erro
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

	//Caso passe pelas verificações acima, é decrementado do saldo atual o saque e, por fim, exibe o valor sacado e o valor restante.
	Clientes[indice].saldoAtual -= valorSaque;
	cout << " --- Saque de " << valorSaque << " realizado com sucesso!  Valor restante na conta: " << Clientes[indice].saldoAtual << " --- " << endl;

	atualizaExtrato(Clientes, indice, 4, valorSaque);
}

// Atualiza o extrato de acordo com os tipos de lançamentos definidos no topo do código
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

	// O estilo da página gerada, incluindo fontes e cores
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

		<< ".infoContas {"
		<< "padding: 10px;"
		<< "border: 1px solid #ccc;"
		<< "border - radius: 5px;"
		<< "cursor: pointer; }"

		<< "td {"
		<< "padding: 16px;"
		<< "}"
		<< "</style>";



	fout << "</head>";
	fout << "<body>"
		<< "<div class='card  auto-width'>"

		<< "<table>"
		<< "<tr>"
		<< "<h3 style='color: rgb(61, 130, 90); padding: 10px; font-size: 20px';> Extrato Bancário </h3>"
		<< "<td style='font-size: 20px;'>" << "Nome: " << Clientes[indice].nomeDoTitular
		<< "</td>"
		<< "<td style='font-size: 20px;'>" << "Agencia/Conta:  " << Clientes[indice].numeroDaAgencia << "/" << Clientes[indice].numeroDaConta
		<< "</td>"
		<< "</tr>"
		<< "<tr>"
		<< "<td style='font-size: 20px;'> Data: ";
		dataAtual();
		fout << "</td>"
		<< "<td style='font-size: 20px;'> Horário: ";
		dataAtual(2);
		fout << "</td>"
		<< "</tr>"
		<< "</table>"

		<< "<table border='2'>";

	fout << "<thead>" <<
		"<tr>" <<
		"<th>";
	fout << "Data </th>" <<
		"<th> Hora";
	fout << "</th>" <<
		"<th>Tipo de Lançamento</th>" <<
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

		fout << "<td>";
		dataAtual();
		fout << "</td>";
		fout << "<td>";
		dataAtual(2);
		fout << "</td>";

		switch (Clientes[indice].EXT_tipoDoLancamento[i]) {
		case 0:
			fout << "<td> Saldo Inicial </td>";
			break;
		case 1:
			fout << "<td> Transferência Enviada </td>";
			break;
		case 2:
			fout << "<td>  Transferência Recebida </td>";
			break;
		case 3:
			fout << "<td>  Depósito</td>";
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

// Altera o vetor de caracteres, adicionando o nome do usuário
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

// Define todo o caminho onde o arquivo será salvo
void geraDiretorio(char username[], char diretorioDeCriacao[]) {
	strcpy(diretorioDeCriacao, "");
	strcat(diretorioDeCriacao, "C:\\Users\\");
	strcat(diretorioDeCriacao, username);
	strcat(diretorioDeCriacao, "\\Desktop\\Extrato.html");
}

// Começa a preparar a variável que vai armazenar o diretório onde o arquivo será salvo
void preparaExtrato(Cliente Clientes[]) {

	int indice = selecionaConta(Clientes);
	while (indice == -1) {
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente." << endl;
		indice = selecionaConta(Clientes);
	}

	char diretorioDeCriacao[150];
	getUserName(diretorioDeCriacao); // A função getUserName altera a string

	fout.open(diretorioDeCriacao);
	geraHtml(Clientes, indice); // A função que vai escrever as informações no arquivo
	if (!fout) {
		cerr << "Erro ao abrir o arquivo" << endl;
		return;
	}
	fout.close(); // Fecha e salva o arquivo gerado

	system(diretorioDeCriacao); // Abre o Extrato no navegador padrão 
}

// Exibe o extrato na tela
void emitirExtrato(Cliente Clientes[]) {
	int indice = selecionaConta(Clientes); // Procurando a conta do cliente
	while (indice == -1) {
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente.";
		indice = selecionaConta(Clientes);
	}
	system("cls");

	exibeConta(Clientes, indice);
	cout << " --- Extrato da Conta --- " << endl;
	int tipoLancamento = 0;
	for (int i = 0; i < Clientes[indice].qtdLancamentos; i++) {
		cout << " | Tipo de Lancamento: " << endl;
		switch (Clientes[indice].EXT_tipoDoLancamento[i]) { // Lê o vetor do extrato contendo o tipo de lançamento
		case 0:
			cout << "   +Saldo Inicial\n";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 1:
			cout << "   -Transferencia Enviada\n";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 2:
			cout << "   +Transferencia Recebida\n ";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 3:
			cout << "   +Deposito\n ";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		case 4:
			cout << "   -Saque\n ";
			tipoLancamento = Clientes[indice].EXT_tipoDoLancamento[i];
			break;
		}
		cout << " |Valor:";
		switch (tipoLancamento) { // Caso seja decrementado um valor (saque ou transferência enviada)
		case 1:
			cout << "-";
			break;
		case 4:
			cout << "-";
			break;
		}
		cout << " R$ ";
		cout << Clientes[indice].EXT_valorDoLancamento[Clientes[indice].EXT_quantidadeDeLancamentos[i]]; // Exibe o valor do lançamento
		cout << "  | Saldo: R$ " << Clientes[indice].EXT_saldoPosLancamento[Clientes[indice].EXT_quantidadeDeLancamentos[i]] << endl; // Exibe o saldo após o lançamento ter sido realizado
	}
}

// Transfere de uma conta para outra
void tranferirValores(Cliente Clientes[]) {
	int indice1 = selecionaConta(Clientes);
	while (indice1 == -1) {
		system("CLS");
		cerr << "Conta nao encontrada! Digite novamente.";
		indice1 = selecionaConta(Clientes); // Armazena a localização da conta de origem
	}
	cout << " Agora selecione a conta de destino: " << endl;
	int indice2 = selecionaConta(Clientes); // Localização da conta de destino
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
	do { // Reutilizado da função de Saque
		//Abaixo é feita uma série de verificações para checar se o valor é valido, caso não seja, exibe uma mensagem informando o erro
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
	} while (valorTransferencia <= 0 || valorTransferencia > Clientes[indice1].saldoAtual); // Verificando se o valor inserido é válido
	Clientes[indice1].saldoAtual -= valorTransferencia; // Subtrai o valor transferido da conta atual
	Clientes[indice2].saldoAtual += valorTransferencia; // Soma o valor à conta de destino
	atualizaExtrato(Clientes, indice1, 1, valorTransferencia); // Atualiza o extrato com a opção de transferência enviada
	atualizaExtrato(Clientes, indice2, 2, valorTransferencia); // Atualzia o extrato com a opção de transferência recebida
	cout << " --- Transferencia concluida ---" << endl;
	system("PAUSE");
}

// Se  tipo == 1  o fout exibe a data
// Se  tipo == 2  o fout exibe a hora
void dataAtual(int tipo) {
	// Obtem o tempo atual
	time_t currentTime;
	time(&currentTime);
	// Estrutura de tm para armazenar a hora local
	struct tm timeInfo;
	// Converte o tempo para a hora local usando localtime_s
	if (localtime_s(&timeInfo, &currentTime) == 0) {
		int year = timeInfo.tm_year + 1900;
		int month = timeInfo.tm_mon + 1;
		int day = timeInfo.tm_mday;
		int hour = timeInfo.tm_hour;
		int min = timeInfo.tm_min;
		// Mostra a data atual
		if(tipo == 1)
			fout << day << "/" << month << "/" << year << endl;
		if (tipo == 2)
			fout << hour << ":" << setw(2) << setfill('0') << min;
	}
	else {
		// Ocorreu um erro ao obter a hora local
		cerr << "Erro ao obter a hora local." << endl;
		return;
	}
}

void consultarSaldo(Cliente Clientes[]) {
	char numeroDaConta[12], numeroDaAgencia[6];
	bool nvalido;

	int indice = selecionaConta(Clientes);
	float valorDeposito;
	while (indice == -1) {
		system("CLS");
		cerr << " Conta nao encontrada! Digite novamente. " << endl;
		indice = selecionaConta(Clientes);
	}
	cout << " | Numero da Conta: ";
	cout << Clientes[indice].numeroDaConta << endl;
	cout << " | Numero da Agencia: ";
	cout << Clientes[indice].numeroDaAgencia << endl;
	cout << " | Nome do Titular: ";
	cout << Clientes[indice].nomeDoTitular << endl;
	cout << " | CPF do Titular: ";
	cout << Clientes[indice].cpfDoTitular << endl;
	cout << " | Saldo Atual: ";
	cout << Clientes[indice].saldoAtual;
	cout << endl;
	system("pause");	
}

void menuExtrato(Cliente Clientes[]) {
	cout << " | Deseja gerar o extrato em tela ou em arquivo?" << endl
		 << " 1 - Extrato em Tela\t2 - Extrato em Arquivo" << endl;
	int opc;
	cin >> opc;
	switch (opc) {
	case 1:
		emitirExtrato(Clientes);
		break;
	case 2:
		preparaExtrato(Clientes);
		break;
	default:
		cerr << " Opcao invalida. Saindo." << endl;
	}
}
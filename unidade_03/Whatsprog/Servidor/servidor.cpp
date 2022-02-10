#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "servidor.h"

/// O flag que indica que o software deve encerrar todas as threads
extern bool fim;

/// Funcao auxiliar que imprime um comando tratado pelo servidor
static void imprimeComando(bool recebido, const string &user, ComandoWhatsProg cmd,
						   int id, const string &user2)
{
	cout << "CMD " << (recebido ? "REC DE" : "ENV P/") << ' '
		 << user << ": " << nome_cmd(cmd);
	if (id > 0)
		cout << ',' << id;
	if (user2.size() > 0)
		cout << ',' << user2;
	cout << endl;
}

/// Funcao auxiliar que imprime um comando enviado pelo servidor
inline static void imprimeComandoEnviado(const string &user, ComandoWhatsProg cmd,
										 int id = -1, const string &user2 = "")
{
	imprimeComando(false, user, cmd, id, user2);
}

/// Funcao auxiliar que imprime um comando recebido pelo servidor
inline static void imprimeComandoRecebido(const string &user, ComandoWhatsProg cmd,
										  int id = -1, const string &user2 = "")
{
	imprimeComando(true, user, cmd, id, user2);
}

/// Funcoes auxiliares para impressao
ostream &operator<<(ostream &O, const Mensagem &M)
{
	O << M.getId() << ';' << nome_status(M.getStatus()) << ';'
	  << M.getRemetente() << ';' << M.getDestinatario() << ';'
	  << M.getTexto();
	return O;
}

ostream &operator<<(ostream &O, const Usuario &U)
{
	O << U.getLogin() << ';' << U.getLastId();
	return O;
}

/// CLASSE USUARIO

/// Construtor default
Usuario::Usuario() : login(""), senha(""), s(), last_id(0) {}

/// Funcao de consulta ao valor para login
const string &Usuario::getLogin() const
{
	return login;
}

/// Fixa login e senha do usuario
/// Retorna true se OK; false em caso de erro
bool Usuario::setUsuario(const string &L, const string &S)
{
	if (!testarNomeUsuario(L) || !testarSenha(S))
	{
		return false;
	}
	login = L;
	senha = S;
	return true;
}

/// Testa se a senha eh valida
/// Aqui normalmente haveria criptografia.
/// Mas como eh um software didatico...
bool Usuario::validarSenha(const string &S) const
{
	return senha == S;
}

/// Funcoes de acesso ao socket do usuario
const tcp_mysocket &Usuario::getSocket() const
{
	return s;
}

/// Alteracao do socket de um usuario
void Usuario::swapSocket(tcp_mysocket &S)
{
	s.swap(S);
}

/// Consulta do estado do socket
bool Usuario::connected() const
{
	return s.connected();
}

bool Usuario::closed() const
{
	return s.closed();
}

/// Funcoes de envio de dados via socket
mysocket_status Usuario::read_int(int32_t &num, long milisec) const
{
	return s.read_int(num, milisec);
}

mysocket_status Usuario::write_int(int32_t num) const
{
	return s.write_int(num);
}

mysocket_status Usuario::read_string(string &msg, long milisec) const
{
	return s.read_string(msg, milisec);
}

mysocket_status Usuario::write_string(const string &msg) const
{
	return s.write_string(msg);
}

/// Fechamento do socket
void Usuario::close()
{
	s.close();
}

/// Consulta da ultima ID do usuario
int32_t Usuario::getLastId() const
{
	return last_id;
}

bool Usuario::setLastId(int32_t ID)
{
	if (ID <= 0 || ID <= last_id)
	{
		return false;
	}
	last_id = ID;
	return true;
}

/// CLASSE WHATSPROGDADOSSERVIDOR

/// Funcoes de acesso aas funcionalidades basicas dos sockets
mysocket_status WhatsProgDadosServidor::listen(const char *port, int nconex)
{
	return c.listen(port, nconex);
}

/// Fecha os sockets de todos os usuarios conectados
void WhatsProgDadosServidor::closeSockets()
{
	c.close();
	for (auto &it : user)
		it.close();
}

/// Envia para o usuario as mensagens que estejam arquivadas (paraUsuario[])
/// com status MSG_RECEBIDA e que sejam destinadas ao usuario.
/// Apos o envio, altera o status da msg enviada para MSG_ENTREGUE
/// No servidor real deveria ser:
/// Envia todas as mensagens que estejam no buffer com status MSG_RECEBIDA
/// e que sejam destinadas ao destinatario para o qual o parametro iDest aponta.
/// Apos o envio, altera o status das msgs enviadas para MSG_ENTREGUE
/// void WhatsProgDadosServidor::enviarRecebidas(IterUsuario iDest)
void WhatsProgDadosServidor::enviarMsgsParaUsuario()
{
}

/// Envia para o usuario as confirmacoes de visualizacao das mensagens
/// que estejam arquivadas (doUsuario[]) com status MSG_LIDA
/// e que tenham sido enviadas pelo usuario.
/// Apos o envio da confirmacao, apaga a msg
/// No servidor real deveria ser:
/// Envia todas as confirmacoes de visualizacao das mensagens
/// que estejam no buffer com status MSG_LIDA e que tenham sido enviadas
/// pelo remetente para o qual o parametro iRemet aponta.
/// Apos o envio das confirmacoes, remove as msgs do buffer
/// void WhatsProgDadosServidor::enviarConfirmacoes(IterUsuario iRemet)
void WhatsProgDadosServidor::enviarConfirmacoesParaUsuario()
{
}

/* **************************************************************************************
ATENCAO: a parte a seguir da implementacao do servidor FAKE pode ser parcialmente adaptada
para o servidor real, mas requer uma analise muito cuidadosa.
A fonte que contem informacao correta sobre como implementar o servidor eh
a especificacao. Alguns trechos de outros programas, como o servidor do mensageiro, tambem
podem servir de ideia ou inspiracao, embora ***CERTAMENTE*** precisem ser adaptados.
************************************************************************************** */
/// Funcao que efetivamente desempenha as tarefas do servidor
int WhatsProgDadosServidor::main_thread()
{
	/// A fila para select (esperar dados em varios sockets)
	mysocket_queue f;

	mysocket_status iResult;
	int32_t cmd;
	int32_t id;

	while (!fim)
	{
		f.clear();
		fim != c.accepting();

		if (!fim)
		{
			f.include(c);

			for (it_user = user.begin(); it_user != user.end(); ++it_user)
			{
				if (it_user->connected())
					f.include(it_user->getSocket());
			}
		}

		iResult = f.wait_read(TIMEOUT_WHATSPROG * 1000);
		if (iResult == mysocket_status::SOCK_ERROR)
		{
			if (!fim)
				cerr << "Erro de espera por alguma atividade\n";

			fim = true;
		}
		if (iResult == mysocket_status::SOCK_TIMEOUT)
		{
			// Saiu por timeout : nao houve atividade em nenhum socket da fila
			// Pode aproveitar para salvar o arquivo de dados no servidor real (opcional)
		}
		if (iResult == mysocket_status::SOCK_OK)
		{
			for (it_user = user.begin(); it_user != user.end(); ++it_user)
			{
				if (!fim && it_user->connected() && f.had_activity(it_user->getSocket()))
				{
					iResult = it_user->read_int(cmd, TIMEOUT_WHATSPROG * 1000);

					if (iResult == mysocket_status::SOCK_OK)
					{
					}
				}
			}
		}
	}//FIM DO WHILE

	cout << "\nServidor encerrado.\n";

	return 0;
}

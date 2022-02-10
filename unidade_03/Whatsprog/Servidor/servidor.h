#ifndef _DADOS_SERVIDOR_
#define _DADOS_SERVIDOR_

#include <string>
#include <list>
#include <vector>

#include "mysocket.h"
#include "whatsprog_dados.h"

using namespace std;

/// Os usuarios cadastrados no servidor
class Usuario
{
private:
	/// Identificacao do usuario
	string login;
	string senha;

	/// Socket de comunicacao (idle se nao conectado)
	tcp_mysocket s;

	/// Ultima id de mensagem recebida
	int32_t last_id;

public:
	/// Construtor default
	Usuario();

	/// Funcao de consulta ao valor para login
	const string &getLogin() const;

	/// Funcao de fixacao de valor para login e senha
	/// Retorna true em caso de sucesso, false se erro
	bool setUsuario(const string &L, const string &S);

	/// Testa se a senha eh valida
	bool validarSenha(const string &S) const;

	/// Funcoes de acesso ao socket do usuario
	const tcp_mysocket &getSocket() const;

	/// Alteracao do socket de um usuario
	void swapSocket(tcp_mysocket &S);

	/// Consulta do estado do socket
	bool connected() const;
	bool closed() const;

	/// Funcoes de envio de dados via socket
	mysocket_status read_int(int32_t &num, long milisec = -1) const;
	mysocket_status write_int(int32_t num) const;
	mysocket_status read_string(string &msg, long milisec = -1) const;
	mysocket_status write_string(const string &msg) const;

	/// Fechamento do socket
	void close();

	/// Consulta da ultima ID do usuario
	int32_t getLastId() const;

	/// Alteracao da ultima ID do usuario
	bool setLastId(int32_t ID);

	/// Reinicializa a ID do usuario
	void resetId() { last_id = 0; }

	/// Teste de igualdade com uma string (testa se a string eh igual ao login)
	bool operator==(const string &L) const { return login == L; }
};

class WhatsProgDadosServidor
{
private:
	/// O socket de conexoes
	tcp_mysocket_server c;

	list<Usuario> user;
	list<Usuario>::iterator it_user;

	list<Mensagem> buffer;

	/// Envia para o usuario as mensagens que estejam arquivadas em vector<Mensagem> paraUsuario;
	/// com status MSG_RECEBIDA e que seja destinada ao usuario.
	/// Apos o envio, altera o status da msg enviada para MSG_ENTREGUE
	void send_buffer_msgs();

	/// Envia para o usuario as confirmacoes de visualizacao das mensagens
	/// que estejam arquivadas (vector<Mensagem> doUsuario;) com status MSG_LIDA
	/// e que tenha sido enviada pelo usuario.
	/// Apos o envio da confirmacao, apaga a msg
	void send_confirmacoes();

public:
	/// Funcoes de acesso aas funcionalidades basicas dos sockets
	mysocket_status listen(const char *port, int nconex = 1);

	/// Fecha os sockets de conexao e de todos os usuarios conectados
	void closeSockets();

	/// Thread que efetivamente desempenha as tarefas do servidor
	int main_thread();
};

#endif // DADOS_SERVIDOR_

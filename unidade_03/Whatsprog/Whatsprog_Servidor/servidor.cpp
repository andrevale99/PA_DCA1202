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
	for (it_user = user.begin(); it_user != user.end(); it_user++)
	{
		cout << "DESCONECTANDO: " << it_user->getLogin() << '\n';
		it_user->close();
	}
}

/// Envia para o usuario as mensagens que estejam arquivadas (list<Mensagem>buffer)
/// com status MSG_RECEBIDA e que sejam destinadas ao usuario.
/// Apos o envio, altera o status da msg enviada para MSG_ENTREGUE
/// No servidor real deveria ser:
/// Envia todas as mensagens que estejam no buffer com status MSG_RECEBIDA
/// e que sejam destinadas ao destinatario para o qual o parametro iDest aponta.
/// Apos o envio, altera o status das msgs enviadas para MSG_ENTREGUE
/// void WhatsProgDadosServidor::enviarRecebidas(IterUsuario iDest)
void WhatsProgDadosServidor::enviarMsgsParaUsuario(list<Usuario>::iterator it_dest)
{
	it_buffer = buffer.begin();
	testDestStatus test_dest(it_dest->getLogin(), MsgStatus::MSG_RECEBIDA);

	while (it_dest->connected() && it_buffer != buffer.end())
	{
		it_buffer = find(it_buffer, buffer.end(), test_dest);
		if (it_buffer != buffer.end())
		{
			bool envioOK = (it_dest->write_int(CMD_NOVA_MSG) != mysocket_status::SOCK_ERROR);
			if (envioOK)
				envioOK = (it_dest->write_int(it_buffer->getId()) != mysocket_status::SOCK_ERROR);
			if (envioOK)
				envioOK = (it_dest->write_string(it_buffer->getRemetente()) != mysocket_status::SOCK_ERROR);
			if (envioOK)
				envioOK = (it_dest->write_string(it_buffer->getTexto()) != mysocket_status::SOCK_ERROR);
			if (envioOK)
			{
				imprimeComandoEnviado(it_dest->getLogin(), CMD_NOVA_MSG, it_buffer->getId(), it_buffer->getRemetente());
				// Mensagem entregue
				it_buffer->setStatus(MsgStatus::MSG_ENTREGUE);
				// Procura o usuario remetente
				it_user = find(user.begin(), user.end(), it_buffer->getRemetente());
				if (it_user != user.end())
				{
					// Remetente existe
					// Testa se o remetente estah concectado
					// Se sim, envia a confirmacao de entrega da mensagem
					if (it_user->connected())
					{
						envioOK = (it_user->write_int(CMD_MSG_ENTREGUE) != mysocket_status::SOCK_ERROR);
						if (envioOK)
							envioOK = (it_user->write_int(it_buffer->getId()) != mysocket_status::SOCK_ERROR);
						if (envioOK)
						{
							imprimeComandoEnviado(it_user->getLogin(), CMD_MSG_ENTREGUE, it_buffer->getId(), "");
						}
					}
				}
				else
				{
					// Nao encontrou o remetente na lista de usuarios
					cerr << "O remetente '" << it_buffer->getRemetente() << "' nao existe. Desconectando...\n";
					it_dest->close();
				}
			}
			else
			{
				// Nao conseguiu enviar para o destinatario
				cerr << "Erro no envio de msg para o destinatario '" << it_dest->getLogin() << "'. Desconectando...\n";
				it_dest->close();
				;
			}

			it_buffer++;
		}
	}
}

/// Envia para o usuario as confirmacoes de visualizacao das mensagens
/// que estejam arquivadas (list<Mensagem>buffer) com status MSG_LIDA
/// e que tenham sido enviadas pelo usuario.
/// Apos o envio da confirmacao, apaga a msg
/// No servidor real deveria ser:
/// Envia todas as confirmacoes de visualizacao das mensagens
/// que estejam no buffer com status MSG_LIDA e que tenham sido enviadas
/// pelo remetente para o qual o parametro iRemet aponta.
/// Apos o envio das confirmacoes, remove as msgs do buffer
/// void WhatsProgDadosServidor::enviarConfirmacoes(IterUsuario iRemet)
void WhatsProgDadosServidor::enviarConfirmacoesParaUsuario(list<Usuario>::iterator it_remet)
{

	it_buffer = buffer.begin();
	testRemetStatus remet(it_remet->getLogin(), MsgStatus::MSG_LIDA);
	testRemetStatus testRemet(it_remet->getLogin(), MsgStatus::MSG_ENTREGUE);
	bool envioOK;

	while (it_remet->connected() && it_buffer != buffer.end())
	{
		it_buffer = find(it_buffer, buffer.end(), testRemet);
		if (it_buffer != buffer.end())
		{
			envioOK = (it_remet->write_int(CMD_MSG_ENTREGUE) != mysocket_status::SOCK_ERROR);
			if (envioOK)
				envioOK = (it_remet->write_int(it_buffer->getId()) != mysocket_status::SOCK_ERROR);
			if (envioOK)
			{
				imprimeComandoEnviado(it_remet->getLogin(), CMD_MSG_ENTREGUE, it_buffer->getId(), "");
			}

			it_buffer++;
		}
	}

	it_buffer = buffer.begin();

	while (it_buffer != buffer.end() && it_remet->connected())
	{
		it_buffer = find(it_buffer, buffer.end(), remet);
		if (it_buffer != buffer.end())
		{
			envioOK = (it_remet->write_int(CMD_MSG_LIDA2) != mysocket_status::SOCK_ERROR);
			if (envioOK)
				envioOK = (it_remet->write_int(it_buffer->getId()) != mysocket_status::SOCK_ERROR);
			if (!envioOK)
			{
				cerr << "Erro no envio de confirmacao de visualizaco para o remetente '"
					 << it_remet->getLogin() << "'. Desconectando...\n";
				it_remet->close();
			}
			else
			{
				imprimeComandoEnviado(it_remet->getLogin(), CMD_MSG_LIDA2, it_buffer->getId());
				// A confirmacao de visualizacao foi enviada
				// Remove a msg do buffer
				it_buffer = buffer.erase(it_buffer);
			}
		}
	}
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
		fim != c.accepting();

		if (fim)
		{
			cerr << "Socket de conexoes nao estah aceitando conexoes. Encerrando\n";
			return -1;
		}

		// Inclui na fila de sockets para o select todos os sockets que eu
		// quero monitorar para ver se houve chegada de dados
		f.clear();
		f.include(c);

		// No servidor real, teria que percorrer a lista de usuarios e incluir
		// todos os que estiverem conectados
		for (it_user = user.begin(); it_user != user.end(); ++it_user)
		{
			if (it_user->connected())
				f.include(it_user->getSocket());
		}
		// Espera que chegue algum dado em qualquer dos sockets da fila
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
			cout << "BUFFER SIZE: " << buffer.size() << '\n';
			for (it_buffer = buffer.begin(); it_buffer != buffer.end(); it_buffer++)
			{
				cout << it_buffer->getId() << '\t' << it_buffer->getRemetente() << '\t'
					 << it_buffer->getDestinatario() << '\t' << (int)it_buffer->getStatus() << '\n';
			}

			cout << "USUARIOS CONECTADOS\n";
			for (it_user = user.begin(); it_user != user.end(); it_user++)
			{
				cout << it_user->getLogin() << '\t' << it_user->connected() << '\n';
			}
		}

		if (iResult == mysocket_status::SOCK_OK)
		{
			// Houve atividade em algum socket da fila
			// Testa em qual socket houve atividade.

			// Primeiro, testa os sockets dos clientes
			for (it_user = user.begin(); it_user != user.end(); it_user++)
			{
				if (!fim && it_user->connected() && f.had_activity(it_user->getSocket()))
				{
					// Leh o comando recebido do cliente
					iResult = it_user->read_int(cmd);

					if (iResult != mysocket_status::SOCK_OK)
					{
						if (iResult == mysocket_status::SOCK_ERROR || iResult == mysocket_status::SOCK_TIMEOUT)
							cerr << "Erro na leitura de comando do cliente " << it_user->getLogin() << ". Desconectando\n";
						it_user->close();
					}
					imprimeComandoRecebido(it_user->getLogin(), (ComandoWhatsProg)cmd);

					if (it_user->connected())
						switch (cmd)
						{
						case CMD_NEW_USER:
						case CMD_LOGIN_USER:
						{
							// Soh pode chegar comando de definicao de usuario em socket recem-criado
							cerr << "Tentativa de redefinicao de usuario no socket do cliente " << it_user->getLogin() << ". Desconectando\n";
							it_user->close();
							break;
						}
						case CMD_LOGIN_OK:
						case CMD_LOGIN_INVALIDO:
						case CMD_MSG_RECEBIDA:
						case CMD_MSG_ENTREGUE:
						case CMD_MSG_LIDA2:
						case CMD_ID_INVALIDA:
						case CMD_USER_INVALIDO:
						case CMD_MSG_INVALIDA:

						case CMD_NOVA_MSG:
						{
							Mensagem msg;
							string dest; // Destinatario
							string texto;

							// LER a ID do cliente
							iResult = it_user->read_int(id, TIMEOUT_WHATSPROG * 1000);

							// LER o DESTINATARIO do cliente
							if (iResult == mysocket_status::SOCK_OK)
							{
								iResult = it_user->read_string(dest, TIMEOUT_WHATSPROG * 1000);
							}

							// LER o TEXTO do cliente
							if (iResult == mysocket_status::SOCK_OK)
							{
								iResult = it_user->read_string(texto, TIMEOUT_WHATSPROG * 1000);
							}

							if (iResult != mysocket_status::SOCK_OK)
							{
								// Pode ser mysocket_status::SOCK_TIMEOUT, mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_ERRO
								if (iResult == mysocket_status::SOCK_ERROR || iResult == mysocket_status::SOCK_DISCONNECTED)
									cerr << "Erro na recepcao de parametros de CMD_NOVA_MSG do cliente "
										 << it_user->getLogin() << ". Desconectando\n";

								it_user->close();
							}

							bool msg_valida = (msg.setStatus(MsgStatus::MSG_RECEBIDA) && msg.setRemetente(it_user->getLogin()));

							// Testa se a id da msg estah correta
							if (msg_valida && it_user->connected() &&
								(!msg.setId(id) || (id <= it_user->getLastId())))
							{
								cerr << "Mensagem com ID invalida " << id << " recebida do cliente "
									 << it_user->getLogin() << ". Enviando cmd de erro\n";
								it_user->write_int(CMD_ID_INVALIDA);
								it_user->write_int(id);
								msg_valida = false;

								imprimeComandoEnviado(it_user->getLogin(), CMD_ID_INVALIDA);

								it_user->close();
							}

							// Procura se o destinatario eh um usuario cadastrado
							if (msg_valida && it_user->connected())
							{
								// Testa se o destinatario da msg estah correto
								achar_dest = find(user.begin(), user.end(), dest);

								if (achar_dest == user.end())
								{
									cerr << "Mensagem com destinatario invalido " << dest << " recebida do cliente "
										 << it_user->getLogin() << ". Enviando cmd de erro\n";
									it_user->write_int(CMD_USER_INVALIDO);
									it_user->write_int(id);
									msg_valida = false;

									imprimeComandoEnviado(it_user->getLogin(), CMD_USER_INVALIDO);

									it_user->close();
								}
							}

							// Testa se o texto da msg estah correto
							if (msg_valida && it_user->connected() &&
								!msg.setTexto(texto))
							{
								cerr << "Mensagem com texto invalido recebida do cliente "
									 << it_user->getLogin() << ". Enviando cmd de erro\n";
								it_user->write_int(CMD_MSG_INVALIDA);
								it_user->write_int(id);
								msg_valida = false;

								imprimeComandoEnviado(it_user->getLogin(), CMD_MSG_INVALIDA);

								it_user->close();
							}

							if (msg_valida && it_user->connected())
							{
								msg.setDestinatario(dest);

								imprimeComandoRecebido(it_user->getLogin(), CMD_NOVA_MSG, id, dest);

								buffer.push_back(msg);

								it_user->setLastId(id);

								// Envia a confirmacao de recebimento
								bool envioOK = (it_user->write_int(CMD_MSG_RECEBIDA) == mysocket_status::SOCK_OK);
								if (envioOK)
									envioOK = (it_user->write_int(id) == mysocket_status::SOCK_OK);
								if (!envioOK)
								{
									cerr << "Erro no envio de confirmacao de recebimento para remetente " << it_user->getLogin() << ". Desconectando\n";
									it_user->close();
								}
								else
								{
									imprimeComandoEnviado(it_user->getLogin(), CMD_MSG_RECEBIDA, id);
								}

								// Testa se o destinatario estah conectado
								// Se sim, envia a mensagem e muda status para MSG_ENTREGUE

								if (achar_dest->connected())
								{
									bool envioOk = (achar_dest->write_int(CMD_NOVA_MSG) == mysocket_status::SOCK_OK);

									if (envioOk)
										envioOk = (achar_dest->write_int(msg.getId()) == mysocket_status::SOCK_OK);

									if (envioOk)
										envioOk = (achar_dest->write_string(msg.getRemetente()) == mysocket_status::SOCK_OK);

									if (envioOk)
										envioOk = (achar_dest->write_string(msg.getTexto()) == mysocket_status::SOCK_OK);

									if (envioOK)
									{
										imprimeComandoEnviado(msg.getDestinatario(), CMD_NOVA_MSG, msg.getId(), msg.getRemetente());

										msg.setStatus(MsgStatus::MSG_ENTREGUE);

										if (it_user->connected())
										{
											bool envio = (it_user->write_int(CMD_MSG_ENTREGUE) == mysocket_status::SOCK_OK);

											if (envio)
												envio = (it_user->write_int(msg.getId()) == mysocket_status::SOCK_OK);

											if (!envio)
											{
												cerr << "Erro no envio de confirmacao de entrega para remetente "
													 << it_user->getLogin() << ". Desconectando\n";
												it_user->close();
											}
											else
											{
												imprimeComandoEnviado(it_user->getLogin(), CMD_MSG_ENTREGUE, msg.getId(), "");
											}
										}
									}
									else
									{
										cerr << "Erro no envio da mensagem para o destinatario '" << achar_dest->getLogin()
											 << "'. Desconectando este usuario...\n";
										achar_dest->close();
									}
								}
							}

							break;
						} // Fim do case CMD_NOVA_MSG

						case CMD_MSG_LIDA1:
						{
							string remetente;

							// LER a ID do cliente
							iResult = it_user->read_int(id, TIMEOUT_WHATSPROG * 1000);

							// LER o REMETENTE do cliente
							if (iResult == mysocket_status::SOCK_OK)
								iResult = it_user->read_string(remetente, TIMEOUT_WHATSPROG * 1000);

							if (iResult != mysocket_status::SOCK_OK)
							{
								// Pode ser mysocket_status::SOCK_TIMEOUT, mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_ERRO
								if (iResult == mysocket_status::SOCK_ERROR || iResult == mysocket_status::SOCK_DISCONNECTED)
									cerr << "Erro na recepcao de parametros de CMD_MSG_LIDA1 do cliente "
										 << it_user->getLogin() << ". Desconectando\n";

								it_user->close();
							}

							if (it_user->connected())
							{
								// Procura no buffer se ha msg com a mesma ID no BUFFER
								testRemetId it_prov(remetente, id);
								it_buffer = find(buffer.begin(), buffer.end(), it_prov); // deu erro na compilacao

								if (it_buffer == buffer.end())
								{
									cerr << "CMD_MSG_LIDA1 de mensagem inexistente (" << id << "," << remetente
										 << ") recebida do cliente '" << it_user->getLogin() << "'. Desconectando...\n";
									it_user->close();
								}
							}

							if (it_user->connected())
							{
								imprimeComandoRecebido(it_user->getLogin(), CMD_MSG_LIDA1, id, remetente);

								it_buffer->setStatus(MsgStatus::MSG_LIDA);

								// Testa se o remetente estah conectado
								// Se sim, envia a confirmacao de visualizacao e remove a msg do buffer
								achar_remet = find(user.begin(), user.end(), it_buffer->getRemetente());
								if (achar_remet != user.end())
								{
									if (achar_remet->connected())
									{
										bool envioOk = (achar_remet->write_int(CMD_MSG_LIDA2) == mysocket_status::SOCK_OK);

										if (envioOk)
											envioOk = (achar_remet->write_int(it_buffer->getId()) == mysocket_status::SOCK_OK);

										if (envioOk)
										{
											imprimeComandoEnviado(it_buffer->getRemetente(), CMD_MSG_LIDA2, it_buffer->getId());
											buffer.erase(it_buffer);
										}
									}
								}
							}

							break;
						} // Fim do case CMD_MSG_LIDA1

						case CMD_LOGOUT_USER:
						{

							imprimeComandoRecebido(it_user->getLogin(), CMD_LOGOUT_USER);
							it_user->close();
							break;
						} // Fim do case CMD_LOGOUT_USER

						default:
						{
							// Comando invalido
							cerr << "Comando invalido (" << nome_cmd((ComandoWhatsProg)cmd) << ") recebido do cliente "
								 << it_user->getLogin() << ". Desconectando\n";
							it_user->close();
							break;
						} // Fim do case CMD_LOGOUT_USER
						} // FIM DO SWITCH
				}		  // Fim do if (had_activity) no socket do cliente
			}			  // FIM DO FOR

			// Depois, testa se houve atividade no socket de conexao
			if (!fim && c.accepting() && f.had_activity(c))
			{
				tcp_mysocket t;
				string login, senha;

				// Aceita provisoriamente a nova conexao
				if (c.accept(t) != mysocket_status::SOCK_OK)
				{
					cerr << "Nao foi possivel estabelecer uma conexao\n";
					t.close();
					// Encerra a thread do servidor
					fim = true;
				}
				// Leh o comando
				if (t.connected())
				{
					iResult = t.read_int(cmd, TIMEOUT_LOGIN_WHATSPROG * 1000);

					if (iResult != mysocket_status::SOCK_OK)
					{
						// Pode ser mysocket_status::SOCK_TIMEOUT, mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_ERRO
						cerr << "Erro na leitura do comando de conexao. Desconectando\n";
						t.close();
					}
				}
				// Testa o comando
				if (t.connected() &&
					(cmd != CMD_LOGIN_USER && cmd != CMD_NEW_USER))
				{
					cerr << "Comando (" << cmd << ") invalido. Desconectando\n";
					t.close();
				}
				// Leh o parametro com o login do usuario que deseja se conectar
				if (t.connected())
				{
					iResult = t.read_string(login, TIMEOUT_LOGIN_WHATSPROG * 1000);
					if (iResult != mysocket_status::SOCK_OK)
					{
						cerr << "Erro na leitura de login. Desconectando\n";
						t.close();
					}
				}
				// Leh o parametro com a senha do usuario que deseja se conectar
				if (t.connected())
				{
					iResult = t.read_string(senha, TIMEOUT_LOGIN_WHATSPROG * 1000);
					if (iResult != mysocket_status::SOCK_OK)
					{
						cerr << "Erro na leitura de senha. Desconectando\n";
						t.close();
					}
				}
				// Testa o login e senha
				if (t.connected() &&
					(!testarNomeUsuario(login) || !testarSenha(senha)))
				{
					cerr << "Login/senha (" << login << "/" << senha << ") invalido. Desconectando\n";
					t.write_int(CMD_LOGIN_INVALIDO);
					t.close();
				}
				// Testa se o usuario eh adequado
				if (t.connected())
				{
					it_user = find(user.begin(), user.end(), login);

					if (cmd == CMD_NEW_USER)
					{
						imprimeComandoRecebido(login, CMD_NEW_USER);

						// Verifica se jah existe um usuario cadastrado com esse login
						if (it_user != user.end())
						{
							// Jah existe um usuario com esse login. Nao pode criar
							cerr << "Login (" << login << ") jah utilizado. Desconectando\n";
							t.write_int(CMD_LOGIN_INVALIDO);
							t.close();
						}

						if (t.connected())
						{
							Usuario novo;
							if (novo.setUsuario(login, senha))
							{
								novo.swapSocket(t);
								user.push_back(novo);
								novo.write_int(CMD_LOGIN_OK);

								imprimeComandoEnviado(novo.getLogin(), CMD_LOGIN_OK);

								it_user = find(user.begin(), user.end(), login);
							}
							else
							{
								cerr << "Usuario (" << login << "/" << senha << ") e/ou socket invalidos. Desconectando...\n";
								t.write_int(CMD_LOGIN_INVALIDO);
								t.close();
							}
						}
					}
					else // else cmd == CMD_NEW_USER; implica cmd eh CMD_LOGIN_USER
					{
						imprimeComandoRecebido(login, CMD_LOGIN_USER);

						if (it_user == user.end())
						{
							cerr << "Login (" << login << ") nao existe. Desconectando\n";
							login = "invalido";
							t.write_int(CMD_LOGIN_INVALIDO);
							imprimeComandoEnviado(login, CMD_LOGIN_INVALIDO);
							t.close();
						}

						// Testa se a senha confere
						if (t.connected() && !(it_user->validarSenha(senha)))
						{
							// Senha invalida
							cerr << "Senha para usuario (" << login << ") nao confere. Desconectando\n";
							senha = "invalida";
							t.write_int(CMD_LOGIN_INVALIDO);
							imprimeComandoEnviado(login, CMD_LOGIN_INVALIDO);
							t.close();
						}

						// Testa se o cliente jah estah conectado
						if (t.connected() && it_user->connected())
						{
							// O usuario jah estah conectado
							cerr << "Usuario (" << login << ") jah estah conectado. Desconectando\n";
							login = "invalido";
							t.write_int(CMD_LOGIN_INVALIDO);
							imprimeComandoEnviado(login, CMD_LOGIN_INVALIDO);
							t.close();
						}

						// Associa o socket que se conectou a um usuario cadastrado
						if (t.connected())
						{
							it_user->swapSocket(t);
							it_user->write_int(CMD_LOGIN_OK);
							imprimeComandoEnviado(it_user->getLogin(), CMD_LOGIN_OK);
						}
					} // fim cmd eh CMD_LOGIN_USER

					// Envia a confirmacao de conexao
					for (list<Usuario>::iterator it = user.begin(); it != user.end(); it++)
					{
						if (login == it->getLogin() && it->connected())
						{
							imprimeComandoRecebido(login, (ComandoWhatsProg)cmd);
							bool envioOK = (it->write_int(CMD_LOGIN_OK) == mysocket_status::SOCK_OK);
							if (!envioOK)
							{
								cerr << "Erro no envio de confirmacao de login para cliente "
									 << it->getLogin() << ". Desconectando\n";
								it->close();
							}
							else
							{
								imprimeComandoEnviado(it->getLogin(), CMD_LOGIN_OK);
							}
						}
					}
					// Se for um cliente antigo, envia para o cliente que se reconectou:
					// a) as mensagens enviadas para ele que estao no buffer
					// b) as confirmacoes de visualizacao para ele que estao no buffer
					if (login == it_user->getLogin() && it_user->connected() && cmd == CMD_LOGIN_USER)
					{
						// No servidor real, essas funcoes auxiliares teriam que receber como
						// parametro um iterador para o usuario que se conectou.

						// mensagens enviadas para ele que estao no buffer
						enviarMsgsParaUsuario(it_user);
						// as confirmacoes de visualizacao para ele que estao no buffer
						enviarConfirmacoesParaUsuario(it_user);
					}

				} // fim do teste se usuario eh adequado
			}
		}
	} // FIM DO WHILE

	cout << "\nServidor encerrado.\n";

	return 0;
}

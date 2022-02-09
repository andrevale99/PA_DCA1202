#include "whatsprog_dados_cliente.h"
#include "mysocket.h"
#include <cstdio>
#include <thread>

/// Variaveis globais
/// Precisam ser acessadas em todas as threads e janelas

/// A variavel global que contem todas as msgs de todas as conversas
WhatsProgDadosCliente DC;

/// O socket do cliente, a ser utilizado por todas as threads
tcp_mysocket sock;

/// A thread de leitura do socket
thread thr; // Thread nao executando (not joinable)

/// Funcoes auxiliares

/// Solicita ao usuario uma opcao de conexao
/// Retorna true se ok, false se o usuario escolheu sair
bool executa_menu_conexao();
/// Solicita ao usuario uma opcao de conversa
bool executa_menu_conversa();
/// Solicita ao usuario uma opcao de mensagem
bool executa_menu_mensagem();
/// Conecta com o servidor
bool aceitaUsuario(const string &IP, const string &login,
                   const string &senha, bool novoUsuario);
/// Exibe aviso de nova msg recebida
void avisaNovaMsg(IterConversa it);
/// Converte um inteiro para uma string C++
string toString(int i);
/// Emite mensagem de erro e espera confirmacao de leitura
void emiteErro(const string& msg);

/// ==============================
/// Funcao principal (dialogo com o usuario)
/// ==============================

int main(int argc, char *argv[])
{
  // Inicializa a biblioteca de sockets (exigida no Windows)
  if (mysocket::init() != mysocket_status::SOCK_OK) {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  // Flag de encerramento do programa a pedido do usuario
  bool fim(false);

  cout << "WhatsProg - cliente sem formatacao\n";
  do
  {
    if (!sock.connected())
    {
      // Antes do socket fechar, estava executando a thread?
      if (thr.joinable())
      {
        // Espera pelo fim da thread de recepcao
        thr.join();

        // Salva as informacoes do cliente
        DC.salvar();

        // Limpa os nomes do servidor e usuario
        DC.unsetServidorUsuario();
      }

      // Pede informacoes de conexao (servidor, login, senha) ao usuario
      fim = !executa_menu_conexao();
    }
    else
    {
      // Testa se existe uma conversa ativa
      if (DC.getConversaAtual() == DC.end())
      {
        // Nao hah conversa ativa: pede ao usuario para criar/selecionar uma
        fim = !executa_menu_conversa();
      }
      else
      {
        // Exibe as opcoes sobre as mensagens da conversa ativa (ler, escrever, etc.)
        fim = !executa_menu_mensagem();
      }
    }
  } while (!fim);

  // Envia o comando de logout, caso jah nao tenha sido enviado
  if (sock.connected())
  {
    sock.write_int(CMD_LOGOUT_USER);
  }

  // Fecha o socket, caso jah nao tenha sido fechado
  sock.close();

  // Salva as informacoes do cliente
  DC.salvar();

  // Espera pelo fim da thread de recepcao, caso estivesse executando
  if (thr.joinable()) thr.join();

  /// Encerramento da biblioteca de sockets
  mysocket::end();

  return 0;
}

/// ==============================
/// Esta eh a thread que recebe as informacoes do socket e
/// armazena na variavel DC, de onde elas poderao ser
/// exibidas pela interface visual
/// ==============================

void main_thread(void)
{
  // O status de retorno das funcoes do socket
  mysocket_status iResult;

  while (sock.connected())
  {
    int32_t cmd;
    iResult = sock.read_int(cmd,1000*TIMEOUT_WHATSPROG);
    if (iResult == mysocket_status::SOCK_OK)
    {
      IterConversa it;
      int32_t id;

      switch(cmd)
      {
      case CMD_NEW_USER:
      case CMD_LOGIN_USER:
      case CMD_LOGIN_OK:
      case CMD_LOGIN_INVALIDO:
      case CMD_MSG_LIDA1:
      case CMD_LOGOUT_USER:
      default:
        // Ignorar: erro do servidor
        break;
      case CMD_NOVA_MSG:
      {
        string remetente;
        string texto;

        // Receber a msg
        iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
        if (iResult == mysocket_status::SOCK_OK) iResult = sock.read_string(remetente, TIMEOUT_WHATSPROG*1000);
        if (iResult == mysocket_status::SOCK_OK) iResult = sock.read_string(texto, TIMEOUT_WHATSPROG*1000);
        if (iResult == mysocket_status::SOCK_OK)
        {
          it = DC.findConversa(remetente);
          if (it == DC.end())
          {
            if (DC.insertConversa(remetente))
            {
              // A conversa recem-inserida eh a ultima
              it = DC.last();
            }
          }
          if (it != DC.end())
          {
            // Conversa existe
            Mensagem M;
            // Testa se os parametros da msg estao corretos
            if (M.setId(id) && M.setRemetente(remetente) && M.setDestinatario(DC.getMeuUsuario()) &&
                M.setTexto(texto) && M.setStatus(MsgStatus::MSG_ENTREGUE))
            {
              // Insere a mensagem na conversa
              DC.pushMessage(it,M);
              // Move a conversa com a nova msg para o inicio da lista (begin)
              DC.moveConversaToBegin(it);
              // Modificou o numero de mensagens da conversa
              // Atualizar a celula correspondente na janela de conversas
              avisaNovaMsg(it);
            }
          }
        }
        else
        {
          // Nao conseguiu ler a msg recebida
          sock.close();
        }
        break;
      } // fim do case CMD_NOVA_MSG
      case CMD_MSG_RECEBIDA:
      case CMD_MSG_ENTREGUE:
      case CMD_MSG_LIDA2:
      {
        int ind_msg;

        // Receber a id
        iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
        if (iResult == mysocket_status::SOCK_OK)
        {
          // Procura se existe uma mensagem com essa id;
          DC.findMensagem(id, it, ind_msg);
          if (it!=DC.end() && ind_msg>=0)
          {
            // Encontrou a msg com essa id
            // Testa o status atual
            bool status_ok(false);
            MsgStatus novoStatus(MsgStatus::MSG_INVALIDA);
            if (cmd==CMD_MSG_RECEBIDA)
            {
              status_ok = (it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_ENVIADA);
              novoStatus = MsgStatus::MSG_RECEBIDA;
            }
            if (cmd==CMD_MSG_ENTREGUE)
            {
              status_ok = (it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_ENVIADA ||
                           it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_RECEBIDA);
              novoStatus = MsgStatus::MSG_ENTREGUE;
            }
            if (cmd==CMD_MSG_LIDA2)
            {
              status_ok = (it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_ENVIADA ||
                           it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_RECEBIDA ||
                           it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_ENTREGUE);
              novoStatus = MsgStatus::MSG_LIDA;
            }
            // Testa o remetente e o status
            if (it->getMensagem(ind_msg).getRemetente()==DC.getMeuUsuario() && status_ok)
            {
              // A msg encontrada eh de minha autoria e estah com um dos status correto
              DC.setStatus(it,ind_msg,novoStatus);
            }
            else
            {
              // A msg com essa id nao estah na situacao esperada
              // Ignorar: erro do servidor ou do cliente
            }
          }
          else
          {
            // Nao existe msg com essa id
            // Ignorar: erro do servidor ou do cliente
          }
        }
        else
        {
          // Nao conseguiu ler a id
          sock.close();
        }
        break;
      } // fim do case CMD_MSG_<status>
      case CMD_ID_INVALIDA:
      case CMD_USER_INVALIDO:
      case CMD_MSG_INVALIDA:
      {
        int ind_msg;

        // Receber a id
        iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
        if (iResult == mysocket_status::SOCK_OK)
        {
          // Procura se existe uma mensagem com essa id;
          DC.findMensagem(id, it, ind_msg);
          if (it!=DC.end() && ind_msg>=0)
          {
            // Encontrou a msg com essa id
            // Testa o remetente e o status
            if (it->getMensagem(ind_msg).getRemetente()==DC.getMeuUsuario() &&
                it->getMensagem(ind_msg).getStatus()==MsgStatus::MSG_ENVIADA)
            {
              // A msg encontrada eh de minha autoria e estah com um dos status apropriado
              // Remover msg
              DC.eraseMessage(it,ind_msg);
              emiteErro("Erro na msg com id="+toString(id)+". Removendo");
            }
            else
            {
              // A msg com essa id nao estah na situacao esperada
              // Ignorar: erro do servidor ou do cliente
            }
          }
          else
          {
            // Nao existe msg com essa id
            // Ignorar: erro do servidor ou do cliente
          }
        }
        else
        {
          // Nao conseguiu ler a id
          sock.close();
        }
        break;
      } // Fim do case CMD_<campo>_INVALIDA
      } // Fim do switch
    }
    else // erro na sock.read_int(cmd)
    {
      // A leitura do comando (int) retornou
      // mysocket_status::SOCK_ERROR, mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_TIMEOUT
      // Se for mysocket_status::SOCK_TIMEOUT, aproveita para salvar o arquivo com os dados
      // Nos outros dois casos, a conexao encerrou, de forma correta ou com erro.
      if (iResult == mysocket_status::SOCK_TIMEOUT)
      {
        if (!DC.salvar())
        {
          emiteErro("Erro no salvamento do arquivo "+DC.getNomeArq());
        }
      }
      else
      {
        sock.close();
      }
    }
  } // fim do while (sock.connected())

  // Envia o comando de logout
  if (sock.connected())
  {
    sock.write_int(CMD_LOGOUT_USER);
  }

  // Encerra o socket
  sock.close();

  // Salva as informacoes do cliente
  DC.salvar();

  // Lembrar que essa eh a funcao da thread. Ela nao pode dar um "join" nela mesma
  // A thread vai se encerrar aqui e estarah pronta para que o programa principal deh join
}

/// ==============================
/// Funcoes auxiliares
/// ==============================

/// Solicita ao usuario uma opcao de conexao
/// Retorna true se ok, false se o usuario escolheu sair (opcao 0)
bool executa_menu_conexao()
{
  int opcao;

  if (sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah desconectado");
    return false;
  }
  if (thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread nao estah executando (joinable)");
    return false;
  }

  cout << endl;
  cout << "1 - Conectar criando um novo login\n";
  cout << "2 - Conectar com um login jah cadastrado\n";
  cout << "0 - Sair\n";
  do
  {
    cout << "Opcao: ";
    cin >> opcao;
  } while (opcao<0 || opcao>2);
  if (opcao != 0)
  {
    string IP;
    string login;
    string senha;
    bool novoUsuario = (opcao==1);

    // Leh os paramentros da conexao
    do
    {
      cout << "IP: ";
      cin >> IP;
    } while (IP.size() == 0);
    do
    {
      cout << "Login: ";
      cin >> login;
    } while (!testarNomeUsuario(login));
    do
    {
      cout << "Senha: ";
      cin >> senha;
    } while (!testarSenha(senha));

    // A funcao aceita usuario faz a conexao do socket e cria a thread
    if (!aceitaUsuario(IP, login, senha, novoUsuario))
    {
      emiteErro("Erro na conexao com IP "+IP+" e login "+login);
      return false;
    }

    // Executou corretamente a opcao escolhida (1 ou 2)
    return true;
  }

  // Usuario escolheu opcao 0 - Sair
  return false;
}

/// Solicita ao usuario uma opcao de conversa
/// Retorna true se ok, false se o usuario escolheu sair (opcao 0)
bool executa_menu_conversa()
{
  int opcao;

  if (!sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah conectado");
    return false;
  }
  if (!thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread estah executando (joinable)");
    return false;
  }
  if (DC.getConversaAtual() != DC.end())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a conversa ativa nao estiver definida");
    return false;
  }

  cout << "1 - Listar conversas\n";
  cout << "2 - Criar nova conversa e torna-la ativa\n";
  cout << "3 - Alterar conversa ativa\n";
  cout << "4 - Apagar conversa\n";
  cout << "5 - Desconectar\n";
  cout << "0 - Sair\n";
  do
  {
    cout << "Opcao: ";
    cin >> opcao;
  } while (opcao<0 || opcao>5);
  switch (opcao)
  {
  case 1:
  {
    cout << endl;
    for (IterConversa it=DC.begin(); it!=DC.end(); it++)
    {
      cout << (it==DC.getConversaAtual() ? '*' : ' ')
           << it->getCorrespondente() << " ("
           << it->getNumMsgsEntregues() << '/'
           << it->size() << ") msgs\n";
    }
    break;
  }  // fim case 1
  case 2:
  case 3:
  {
    string conversa;

    do
    {
      cout << "Conversa: ";
      cin >> conversa;
    } while (!testarNomeUsuario(conversa));

    // Insere nova conversa na base de dados, se for opcao 2
    if (opcao == 2) if (!DC.insertConversa(conversa))
    {
      emiteErro("Nova conversa "+conversa+" invalida");
      break;
    }

    // Faz a conversa ser a conversa ativa
    if (DC.setConversaAtual(conversa))
    {
      cout << "Conversa " << conversa << " eh a nova conversa ativa\n";
    }
    else
    {
      emiteErro("Erro ao fazer a conversa "+conversa+" ser a nova conversa ativa");
    }
    break;
  }  // fim case 2, case 3
  case 4:
  {
    string conversa;
    IterConversa it;

    do
    {
      cout << "Conversa: ";
      cin >> conversa;
    } while (!testarNomeUsuario(conversa));

    // procura a conversa
    it = DC.findConversa(conversa);
    if (it == DC.end())
    {
      emiteErro("Conversa "+conversa+" inexistente");
      break;
    }
    if (!DC.eraseConversa(it))
    {
      emiteErro("Erro ao excluir conversa "+conversa);
      break;
    }
    cout << "Conversa " << conversa << " apagada\n";
    break;
  }  // fim case 2, case 3
  case 5:
  {
    if (sock.connected())
    {
      // Envia o comando de logout para o servidor
      sock.write_int(CMD_LOGOUT_USER);
    }

    // Fecha o socket e, consequentemente,
    // encerra a thread de leitura de dados do socket
    sock.close();

    break;
  }  // fim case 4
  case 0:
    return false;
  default:
    break;
  }  // fim switch
  return true;
}

/// Solicita ao usuario uma opcao de mensagem
/// Retorna true se ok, false se o usuario escolheu sair (opcao 0)
bool executa_menu_mensagem()
{
  int opcao;

  if (!sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah conectado");
    return false;
  }
  if (!thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread estah executando (joinable)");
    return false;
  }
  IterConversa it = DC.getConversaAtual();
  if (it == DC.end())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a conversa ativa estiver definida");
    return false;
  }

  cout << "1 - Listar mensagens\n";
  cout << "2 - Escrever mensagem\n";
  cout << "3 - Apagar todas as mensagens\n";
  cout << "4 - Voltar ao menu de conversas\n";
  cout << "0 - Sair\n";
  do
  {
    cout << "Opcao: ";
    cin >> opcao;
  } while (opcao<0 || opcao>4);
  switch (opcao)
  {
  case 1:
  {
    bool envioOK = true;

    cout << "\nConversa " << it->getCorrespondente() << ":\n";
    for (unsigned i=0; i<it->size(); i++)
    {
      cout << it->getMensagem(i).getId() << ") "
           << it->getMensagem(i).getRemetente() << "->"
           << it->getMensagem(i).getDestinatario() << ": "
           << it->getMensagem(i).getTexto();
      if (it->getMensagem(i).getRemetente() == DC.getMeuUsuario())
      {
        // A mensagem eh de minha autoria
        cout << " (";
        if (it->getMensagem(i).getStatus() == MsgStatus::MSG_ENVIADA) cout << "?";
        if (it->getMensagem(i).getStatus() == MsgStatus::MSG_RECEBIDA) cout << "V";
        if (it->getMensagem(i).getStatus() == MsgStatus::MSG_ENTREGUE) cout << "VV";
        if (it->getMensagem(i).getStatus() == MsgStatus::MSG_LIDA) cout << "VVV";
        cout << ")";
      }
      else
      {
        // A msg eh de autoria do outro
        // Muda o status de todas as msgs que foram enviadas para mim de ENTREGUE -> LIDA
        // Enviar confirmacao de visualizaco
        if (it->getMensagem(i).getStatus() == MsgStatus::MSG_ENTREGUE)
        {
          // Envia msg ao servidor informando que a msg foi lida
          if (envioOK) envioOK = (sock.write_int(CMD_MSG_LIDA1) != mysocket_status::SOCK_ERROR);
          if (envioOK) envioOK = (sock.write_int(it->getMensagem(i).getId()) != mysocket_status::SOCK_ERROR);
          if (envioOK) envioOK = (sock.write_string(it->getMensagem(i).getRemetente()) != mysocket_status::SOCK_ERROR);
          if (envioOK)
          {
            // Muda status na base de dados local
            DC.setStatus(it,i,MsgStatus::MSG_LIDA);
          }
        }
      }
      cout << endl;
    } // fim for
    if (!envioOK)
    {
      // Desconecta
      sock.close();
      // Mensagem de erro
      emiteErro("Falha no envio das confimacoes de visualizacao");
    }
    break;
  }  // fim case 1
  case 2:
  {
    Mensagem M;
    string texto;

    cout << "\nMensagem para " << it->getCorrespondente() << ":\n";
    do
    {
      cout << "Texto (1 a 255 chars): ";
      cin >> ws;
      getline(cin,texto);
    } while (!testarTextoMensagem(texto));

    // Define os campos da msg
    if (!M.setId(DC.getNovaIdMensagem()) ||
        !M.setRemetente(DC.getMeuUsuario()) ||
        !M.setDestinatario(it->getCorrespondente()) ||
        !M.setTexto(texto) ||
        !M.setStatus(MsgStatus::MSG_ENVIADA))
    {
      // Desconecta
      sock.close();
      // Mensagem de erro
      emiteErro("Mensagem invalida");
    }

    // Inclui a msg na base de dados local, acrescentando no final da conversa
    // Isso tem que ser feito antes de enviar via socket porque se a resposta do
    // servidor confirmando o recebimento for enviada imediatamente, a mensagem jah
    // tem que estar na base de dados para que o cliente modifique corretamente seu status
    DC.pushMessage(it,M);

    // Envia a msg via socket para o servidor
    bool envioOK = (sock.write_int(CMD_NOVA_MSG) != mysocket_status::SOCK_ERROR);
    if (envioOK) envioOK = (sock.write_int(M.getId()) != mysocket_status::SOCK_ERROR);
    if (envioOK) envioOK = (sock.write_string(M.getDestinatario()) != mysocket_status::SOCK_ERROR);
    if (envioOK) envioOK = (sock.write_string(M.getTexto()) != mysocket_status::SOCK_ERROR);
    if (envioOK)
    {
      // Move a conversa com a nova msg para o inicio da lista (begin)
      DC.moveConversaToBegin(it);
    }
    else
    {
      // Remove ultima msg da conversa
      DC.popMessage(it);
      // Desconecta
      sock.close();
      // Mensagem de erro
      emiteErro("Erro no envio da mensagem");
    }
    break;
  }  // fim case 2
  case 3:
  {
    DC.clearMessages(it);
    break;
  }  // fim case 3
  case 4:
  {
    DC.unsetConversaAtual();
    break;
  }  // fim case 4
  case 0:
    return false;
  default:
    break;
  }  // fim switch
  return true;
}

/// Conecta com o servidor
bool aceitaUsuario(const string &IP, const string &login,
                   const string &senha, bool novoUsuario )
{
  if (sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah desconectado");
    return false;
  }
  if (thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread nao estah executando (joinable)");
    return false;
  }

  bool conexaoOK(true);
  int32_t cmd;

  // Conecta o socket
  if (sock.connect(IP.c_str(), PORTA_WHATSPROG) != mysocket_status::SOCK_OK)
  {
    sock.close();
    return false;
  }

  // Envia a msg de conexao para o servidor, atraves do socket
  if (novoUsuario)
  {
    // Testa se eh possivel cadastrar um novo usuario com esses ip, login e senha
    // Envia o comando CMD_NEW_USER
    if (conexaoOK) conexaoOK = (sock.write_int(CMD_NEW_USER) != mysocket_status::SOCK_ERROR);
  }
  else
  {
    // Testa se eh possivel se conectar com esses ip, login e senha
    // Envia o comando CMD_LOGIN_USER
    if (conexaoOK) conexaoOK = (sock.write_int(CMD_LOGIN_USER) != mysocket_status::SOCK_ERROR);
  }
  if (!conexaoOK) return false;

  // Envia os parametros do comando (login, senha)
  if (conexaoOK) conexaoOK = (sock.write_string(login) != mysocket_status::SOCK_ERROR);
  if (conexaoOK) conexaoOK = (sock.write_string(senha) != mysocket_status::SOCK_ERROR);
  if (!conexaoOK)
  {
    sock.close();
    return false;
  }
  conexaoOK = (sock.read_int(cmd,1000*TIMEOUT_WHATSPROG) == mysocket_status::SOCK_OK);
  if (conexaoOK) conexaoOK = (cmd == CMD_LOGIN_OK);
  if (!conexaoOK)
  {
    sock.close();
    return false;
  }

  // Armazena o novo nome do servidor e do usuario
  DC.setServidorUsuario(IP,login);

  // Leh o arquivo com os dados de conexao anterior, caso exista
  DC.ler();

  // Lanca a thread de leitura dos dados do socket
  // Tem que ser depois de ler o arquivo com os dados da conexao anterior
  // Senao, pode ficar com duas conversas referentes ao mesmo usuario:
  // 1) Chegou uma msg que estava retida no servidor
  // 2) Leu uma conversa previa com mesmo usuario do arquivo
  thr = thread(main_thread);

  // Se soh tiver uma conversa, seleciona-a como ativa
  if (DC.size() == 1)
  {
    DC.setConversaAtual(DC.begin());
  }

  // Tudo OK!
  return true;
}

/// Exibe aviso de nova msg recebida
void avisaNovaMsg(IterConversa it)
{
  cout << "\nNova msg recebida (" << it->getNumMsgsEntregues() << '/' << it->size()
       << ") na conversa " << it->getCorrespondente() << endl;
}

/// Converte um inteiro para uma string C++
string toString(int i)
{
  static char buf[80];
  snprintf(buf,80,"%d",i);
  return buf;
}

/// Emite mensagem de erro
void emiteErro(const string& msg)
{
  cerr << "\nERRO: " << msg << "!\n";
}

#include <QMessageBox>

#include "mysocket.h"
#include "mensageiro2_cliente_thread.h"

using namespace std;

/// O socket do cliente, a ser utilizado por todas as threads
extern tcp_mysocket sock;

/// Construtor
Mensageiro2_Cliente_Thread::Mensageiro2_Cliente_Thread(QObject *parent) :
  QObject(parent),
  thr(),
  em_execucao(false)
{
}

/// Funcao de consulta do estado da thread
bool Mensageiro2_Cliente_Thread::executing() const
{
  return em_execucao;
}

/// Este eh o metodo principal da thread
/// Ele recebe as informacoes do socket e exibe atraves da emissao de sinais
void Mensageiro2_Cliente_Thread::main_thread()
{
  // O status de retorno das funcoes do socket
  mysocket_status iResult;

  string remetente,msg;
  bool erro(false);

  // Permanece em loop sem fim enquanto flag for true e socket conectado
  while (em_execucao && sock.connected())
  {
    iResult = sock.read_string(remetente);
    if (iResult == mysocket_status::SOCK_OK)
    {
      iResult = sock.read_string(msg);
      if (iResult == mysocket_status::SOCK_OK)
      {
        emit signExibirMsgRecebida(QString(remetente.c_str()),
                                   QString(msg.c_str()));
      }
      else
      {
        // A leitura da string retornou
        // mysocket_status::SOCK_ERROR, mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_TIMEOUT
        // Se for mysocket_status::SOCK_TIMEOUT, nao precisa fazer nada. Na realidade,
        // mysocket_status::SOCK_TIMEOUT nunca deve acontecer, pois nao foi definido um timeout.
        // Nos outros dois casos, a conexao encerrou, de forma correta ou com erro.
        if (iResult != mysocket_status::SOCK_TIMEOUT)
        {
          sock.close();
          erro = (iResult == mysocket_status::SOCK_ERROR);
        }
      }
    }
    else
    {
      // A leitura da string retornou
      // mysocket_status::SOCK_ERROR, mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_TIMEOUT
      // Se for mysocket_status::SOCK_TIMEOUT, nao precisa fazer nada. Na realidade,
      // mysocket_status::SOCK_TIMEOUT nunca deve acontecer, pois nao foi definido um timeout.
      // Nos outros dois casos, a conexao encerrou, de forma correta ou com erro.
      if (iResult != mysocket_status::SOCK_TIMEOUT)
      {
        sock.close();
        erro = (iResult == mysocket_status::SOCK_ERROR);
      }
    }
  }

  if (em_execucao && erro)
  {
    // Emitir msg de erro: conexao interrompida abruptamente
    // A thread nao pode fazer nenhuma alteracao na interface, tal como
    // abrir uma janela com msg de erro
    // O que vamos fazer eh emitir um sinal para que a thread principal
    // abra um QMessageBox com msg de erro
    emit signExibirErro("Erro na recepcao", "Conexao interrompida abruptamente");
  }

  // Altera o flag de execucao se jah nao for falso
  em_execucao = false;

  // Fecha o socket se jah nao estiver fechado
  sock.close();

  // Avisa o fim da thread
  emit signExibirThreadEncerrada();
}

/// A funcao main_thread() executa o metodo main_thread()
void main_thread(Mensageiro2_Cliente_Thread *mct)
{
  mct->main_thread();
}

/// Iniciar a thread
bool Mensageiro2_Cliente_Thread::slotIniciar()
{
  if (!sock.connected())
  {
    // Emitir msg de erro: socket nao conectado
    emit signExibirErro("Erro na execucao","Socket nao conectado");
    return false;
  }
  if (em_execucao || thr.joinable())
  {
    sock.close();

    // Emitir msg de erro: thread jah estah em execucao
    emit signExibirErro("Erro na execucao","Thread de leitura jah estah em execucao");
    return false;
  }
  em_execucao = true;
  thr = std::thread(::main_thread, this);
  if (!thr.joinable())
  {
    thr = std::thread();
    em_execucao = false;
    sock.close();

    // Emitir msg de erro: nao foi possivel criar thread
    emit signExibirErro("Erro na execucao","Nao foi possivel criar thread de leitura");
    return false;
  }
  // Thread estah funcionando
  emit signExibirThreadIniciada();

  return true;
}

/// Encerrar e aguardar fim da thread
void Mensageiro2_Cliente_Thread::slotEncerrar(void)
{
  // Altera o flag de execucao se jah nao for falso
  em_execucao = false;

  // Fecha o socket se jah nao estiver fechado
  sock.close();

  // Aguarda o encerramento da thread
  if (thr.joinable()) thr.join();

  // Armazena um valor nulo no identificador da thread
  thr = std::thread();
}

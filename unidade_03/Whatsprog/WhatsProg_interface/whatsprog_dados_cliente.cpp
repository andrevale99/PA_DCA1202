#include "whatsprog_dados_cliente.h"
#include <algorithm>

/***
 *
 * DUPLA: Andre Menezes de Freitas Vale
 *        Luis Filipe Silva da Costa
 *
 * SISTEMA OPERACIONAL USADO: LINUX
 *
 * Caso queira roda no Windows
 * 1. Descomentar no ".pro" o link "-lWs2_32"
 * 2. Descomentar as partes para rodar no Windows nos arquivos "mysocket.h" e "mysocket.cpp"
 *
 */


bool Conversa::setCorrespondente(const string &U)
{
  if (!testarNomeUsuario(U)) return false;
  correspondente = U;
  return true;
}

/// Retorna a i-esima mensagem da conversa
const Mensagem& Conversa::getMensagem(unsigned i) const
{
  static Mensagem vazia;
  return (i<size() ? LM[i] : vazia);
}

/// Remove a i-esima msg
void Conversa::eraseMessage(unsigned i)
{
  if (i<size())
  {
    LM.erase(LM.begin()+i);
  }
}

/// Retorna o numero de msgs entregues mas ainda nao lidas de uma conversa
unsigned Conversa::getNumMsgsEntregues() const
{
  return count(LM.begin(), LM.end(), testRemetStatus(correspondente, MsgStatus::MSG_ENTREGUE));
}

/// Procura na conversa uma mensagem que tenha a id igual ao parametro
/// e que tenha o correspondente da conversa como destinatario
/// (ou seja, tenha o usuario como remetente)
/// Retorna o indice da mensagem dentro da conversa.
/// Caso nao exista, retorna um valor negativo.
int Conversa::findMensagem(int32_t id) const
{
  vector<Mensagem>::const_iterator iM;
  iM = find(LM.begin(), LM.end(), testDestId(correspondente, id));
  if (iM != LM.end())
  {
    // Achou a msg na conversa
    return (iM-LM.begin());
  }
  return -1;
}

/// Ler de arquivo
/// Recebe como entrada uma ifstream jah aberta
/// A ifstream deve ser aberta pela funcao de ler de um nivel superior
/// (da classe DadosCliente)
bool Conversa::ler(ifstream& arq)
{
  string prov_cabecalho;
  string prov_correspondente;
  int prov_size;

  if (!arq.is_open()) return false;
  arq >> prov_cabecalho >> prov_correspondente >> prov_size;
  if (prov_cabecalho != "CONVERSA" ||
      !testarNomeUsuario(prov_correspondente) ||
      prov_size<0)
  {
    correspondente = "";
    LM.clear();
    return false;
  }
  correspondente = prov_correspondente;
  LM.resize(prov_size);
  for (int i=0; i<prov_size; i++)
  {
    if (!LM[i].ler(arq)) return false;
  }
  return true;
}

/// Salvar em arquivo
/// Recebe como entrada uma ofstream jah aberta
/// A ofstream deve ser aberta pela funcao de salvar de um nivel superior
/// (da classe DadosCliente)
bool Conversa::salvar(ofstream& arq) const
{
  if (!arq.is_open()) return false;
  arq << "CONVERSA " << correspondente << ' ' << LM.size() << endl;
  for (unsigned i=0; i<LM.size(); i++)
  {
    if (!LM[i].salvar(arq)) return false;
  }
  return true;
}

/// Construtor default
WhatsProgDadosCliente::WhatsProgDadosCliente():
  servidor(""),meuUsuario(""),convAtual(),idMensagem(0),LC(),mtx()
{
  // Nenhuam conversa selecionada inicialmente
  convAtual=LC.end();
}

/// Retorna iterador para a ultima conversa da lista de conversas
IterConversa WhatsProgDadosCliente::last()
{
  IterConversa it = LC.end();
  if (LC.size() > 0) --it;
  return it;
}

bool WhatsProgDadosCliente::setServidorUsuario(const string &IP, const string &MU)
{
  mtx.lock();

  // Sempre que altera a conexao, apaga todas as conversas
  clearConversas();
  convAtual = LC.end();

  if (IP=="" || !testarNomeUsuario(MU))
  {
    servidor = "";
    meuUsuario = "";
    mtx.unlock();
    return false;
  }
  servidor = IP;
  meuUsuario = MU;
  mtx.unlock();
  return true;
}

void WhatsProgDadosCliente::unsetServidorUsuario()
{
  mtx.lock();
  servidor = "";
  meuUsuario = "";
  clearConversas();
  convAtual = LC.end();
  mtx.unlock();
}

/// Funcao de alteracao da conversa atual
/// Recebe como parametro o nome do correspondente da conversa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::setConversaAtual(const string &U)
{
  if (!testarNomeUsuario(U)) return false;
  IterConversa it = find(LC.begin(), LC.end(), U);
  return setConversaAtual(it);
}

/// Funcao de alteracao da conversa atual
/// Recebe como parametro o iterador para a conversa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::setConversaAtual(IterConversa it)
{
  if (it == LC.end()) return false;
  mtx.lock();
  convAtual = it;
  mtx.unlock();
  return true;
}

/// Inclui (cria) uma nova conversa com o correspondente passado como parametro
/// Se for a unica conversa, torna-a a conversa ativa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::insertConversa(const string &U)
{
  // Soh pode inserir se nao existir uma conversa com mesmo correspondente
  if (findConversa(U) != LC.end()) return false;
  Conversa C;
  // Testa erro na atribuicao do nome da conversa
  if (!C.setCorrespondente(U)) return false;

  mtx.lock();
  LC.push_back(C);
  if (LC.size() == 1) convAtual = LC.begin();
  mtx.unlock();
  return true;
}

/// Retorna o indice de uma conversa cujo correspondente seja igual ao parametro
/// Retorna end() caso nao exista a conversa
IterConversa WhatsProgDadosCliente::findConversa(const string &U)
{
  IterConversa it=find(LC.begin(), LC.end(), U);
  return it;
}

/// Move uma conversa para a primeira posicao da lista (begin)
bool WhatsProgDadosCliente::moveConversaToBegin(IterConversa it)
{
  if (LC.size()==0 || it==LC.end()) return false;
  // Testa se a conversa jah nao estah no inicio da lista
  if (it != LC.begin())
  {
    // Move a conversa para o inicio da lista
    mtx.lock();
    LC.splice(LC.begin(), LC, it);
    mtx.unlock();
  }
  return true;
}

/// Exclui a conversa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::eraseConversa(IterConversa it)
{
  if (LC.size()==0 || it == LC.end()) return false;
  // Apaga a conversa
  mtx.lock();
  LC.erase(it);
  // Se a conversa excluida era a conversa atual,
  // faz com que nenhuma conversa esteja selecionada
  if (it==convAtual)
  {
    convAtual = LC.end();
  }
  mtx.unlock();
  return true;
}

/// Procura em todas as conversa uma mensagem que tenha a id igual ao parametro
/// e que tenha o correspondente da conversa como destinatario
/// (ou seja, tenha o usuario como remetente)
/// Retorna o iterator para a conversa onde estah a mensagem que tem essa id
/// e o indice da mensagem dentro dessa conversa
/// Caso nao exista em nenhuma conversa uma msg com essa id,
/// retorna o iterator igual a end() e o indice negativo
void WhatsProgDadosCliente::findMensagem(int32_t id, IterConversa& it, int& ind_msg)
{
  for (it=LC.begin(); it!=LC.end(); it++)
  {
    ind_msg = it->findMensagem(id);
    if (ind_msg >= 0)
    {
      // Achou a msg na conversa ind_conv
      return;
    }
  }
  it = LC.end();
  ind_msg = -1;
}

/// Altera o status da i-esima mensagem da conversa it
void WhatsProgDadosCliente::setStatus(IterConversa it, unsigned i, MsgStatus S)
{
  if (LC.size()==0 || it==LC.end() || i>=it->size() || !testarStatus(S)) return;
  mtx.lock();
  it->LM[i].setStatus(S);
  mtx.unlock();
}

/// Insere nova msg no final de uma conversa
void WhatsProgDadosCliente::pushMessage(IterConversa it, const Mensagem &M)
{
  if (LC.size()==0 || it==LC.end()) return;
  mtx.lock();
  it->pushMessage(M);
  mtx.unlock();
}

/// Remove ultima msg da conversa it
void WhatsProgDadosCliente::popMessage(IterConversa it)
{
  if (LC.size()==0 || it==LC.end()) return;
  mtx.lock();
  it->popMessage();
  mtx.unlock();
}

/// Apaga todas as msgs da conversa it
void WhatsProgDadosCliente::clearMessages(IterConversa it)
{
  if (LC.size()==0 || it==LC.end()) return;
  mtx.lock();
  it->clearMessages();
  mtx.unlock();
}

/// Apaga a i-esima mensagem da conversa it
void WhatsProgDadosCliente::eraseMessage(IterConversa it, unsigned i)
{
  if (LC.size()==0 || it==LC.end() || i>=it->size()) return;
  mtx.lock();
  it->eraseMessage(i)  ;
  mtx.unlock();
}

/// Retorna o nome do arquivo para ler/salvar os dados
string WhatsProgDadosCliente::getNomeArq() const
{
  if (!testarNomeUsuario(meuUsuario) || servidor.size()==0) return "";
  return "whatsdata_cliente_"+meuUsuario+"_AT_"+servidor+".wdc";
}

/// Ler de arquivo
/// O nome do arquivo eh padronizado
bool WhatsProgDadosCliente::ler()
{
  mtx.lock();

  // Soh leh se servidor e usuario estiverem definidos
  if (servidor.size()<=0 || !testarNomeUsuario(meuUsuario))
  {
    mtx.unlock();
    return false;
  }

  // Se nao conseguir abrir o arquivo, encerra
  ifstream arq(getNomeArq().c_str());
  if (!arq.is_open())
  {
    mtx.unlock();
    return false;
  }

  string prov_cabecalho;
  string prov_servidor;
  string prov_meuUsuario;
  int prov_idMensagem;
  int prov_size;

  arq >> prov_cabecalho >> prov_servidor >> prov_meuUsuario
      >> prov_idMensagem >> prov_size;

  // Se os dados iniciais nao forem validos, encerra
  if (prov_cabecalho != "DADOS_CLIENTE" ||
      prov_servidor.size()==0 ||
      !testarNomeUsuario(prov_meuUsuario) ||
      prov_idMensagem<0 || prov_size<0)
  {
    arq.close();
    mtx.unlock();
    return false;
  }

  // Atualiza os dados gerais do cliente
  servidor = prov_servidor;
  meuUsuario = prov_meuUsuario;
  idMensagem = prov_idMensagem;
  LC.clear();
  convAtual = LC.end();
  // Leh as conversas
  Conversa prov_C;
  for (int i=0; i<prov_size; i++)
  {
    if (!prov_C.ler(arq))
    {
      arq.close();
      mtx.unlock();
      return false;
    }
    LC.push_back(prov_C);
  }
  arq.close();
  mtx.unlock();
  return true;
}

/// Salvar em arquivo
/// O nome do arquivo eh padronizado
bool WhatsProgDadosCliente::salvar()
{
  mtx.lock();

  // Soh salva se servidor e usuario estiverem definidos
  if (servidor.size()<=0 || !testarNomeUsuario(meuUsuario))
  {
    mtx.unlock();
    return false;
  }

  // Se nao conseguir abrir o arquivo, encerra
  ofstream arq(getNomeArq().c_str());
  if (!arq.is_open())
  {
    mtx.unlock();
    return false;
  }

  arq << "DADOS_CLIENTE " << servidor << ' ' << meuUsuario << ' '
      << idMensagem << ' ' << LC.size() << endl;
  for (ListConversa::const_iterator i=LC.begin(); i!=LC.end(); i++)
  {
    if (!i->salvar(arq))
    {
      arq.close();
      mtx.unlock();
      return false;
    }
  }
  arq.close();
  mtx.unlock();
  return true;
}

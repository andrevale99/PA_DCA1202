#include "whatsprog_dados.h"

/// Testa um possivel login: tamanho e soh contem caracteres de 33 a 126
bool testarNomeUsuario(const string& S)
{
  // Testa o tamanho
  if (S.size()<TAM_MIN_NOME_USUARIO || S.size()>TAM_MAX_NOME_USUARIO) return false;
  // Testa se todos os caracteres sao validos: de 33 (!) a 126 (~)
  for (unsigned i=0; i<S.size(); i++) if (S[i]<33 || S[i]>126) return false;
  return true;
}

/// Testa uma possivel senha: tamanho e soh contem caracteres de 33 a 126
bool testarSenha(const string& S)
{
  // Testa o tamanho
  if (S.size()<TAM_MIN_SENHA || S.size()>TAM_MAX_SENHA) return false;
  // Testa se todos os caracteres sao validos: de 33 (!) a 126 (~)
  for (unsigned i=0; i<S.size(); i++) if ((int)S[i]<33 || (int)S[i]>126) return false;
  return true;
}

/// Testa o texto de uma mensagem: tamanho
bool testarTextoMensagem(const string& S)
{
  // Testa o tamanho
  if (S.size()==0 || S.size()>TAM_MAX_MSG) return false;
  return true;
}

/// Testa um possivel status
bool testarStatus(MsgStatus S)
{
  return (S!=MsgStatus::MSG_INVALIDA);
}

/// Retorna uma string com o nome de um status
string nome_status(MsgStatus S)
{
  switch(S)
  {
  case MsgStatus::MSG_ENVIADA: return "ENVI";
  case MsgStatus::MSG_RECEBIDA: return "RECE";
  case MsgStatus::MSG_ENTREGUE: return "ENTR";
  case MsgStatus::MSG_LIDA: return "LIDA";
  case MsgStatus::MSG_INVALIDA: return "INVA";
  }
  return "????";
}

/// Retorna uma string com o nome de um comando
string nome_cmd(ComandoWhatsProg cmd)
{
  switch(cmd)
  {
  case CMD_NEW_USER: return "CMD_NEW_USER";
  case CMD_LOGIN_USER: return "CMD_LOGIN_USER";
  case CMD_LOGIN_OK: return "CMD_LOGIN_OK";
  case CMD_LOGIN_INVALIDO: return "CMD_LOGIN_INVALIDO";
  case CMD_NOVA_MSG: return "CMD_NOVA_MSG";
  case CMD_MSG_RECEBIDA: return "CMD_MSG_RECEBIDA";
  case CMD_MSG_ENTREGUE: return "CMD_MSG_ENTREGUE";
  case CMD_MSG_LIDA1: return "CMD_MSG_LIDA1";
  case CMD_MSG_LIDA2: return "CMD_MSG_LIDA2";
  case CMD_ID_INVALIDA: return "CMD_ID_INVALIDA";
  case CMD_USER_INVALIDO: return "CMD_USER_INVALIDO";
  case CMD_MSG_INVALIDA: return "CMD_MSG_INVALIDA";
  case CMD_LOGOUT_USER: return "CMD_LOGOUT_USER";
  }
  return "CMD_???";
}

/// Alguns tipos auxiliares que sao utilizados pelos algoritmos STL:

/// Testar mensagens com um dado remetente e status
testRemetStatus::testRemetStatus(string R, MsgStatus S): remetente(R), status(S) {}

/// Testar mensagens com um dado remetente e id
testRemetId::testRemetId(string R, int32_t I): remetente(R), id(I) {}

/// Testar mensagens com um dado destinatario e status
testDestStatus::testDestStatus(string D, MsgStatus S): destinatario(D), status(S) {}

/// Testar mensagens com um dado destinatario e id
testDestId::testDestId(string D, int32_t I): destinatario(D), id(I) {}

/// As mensagens armazenadas no cliente e no servidor

/// Construtor default
Mensagem::Mensagem(): id(0), status(MsgStatus::MSG_INVALIDA),
                      remetente(""), destinatario(""), texto("") {}

/// Funcoes de consulta/alteracao de parametros
/// As funcoes de alteracao retornam false em caso de erro, true se OK

int32_t Mensagem::getId() const
{
  return id;
}

bool Mensagem::setId(int32_t I)
{
  if (I<=0)
  {
    return false;
  }
  id=I;
  return true;
}

const string& Mensagem::getRemetente() const
{
  return remetente;
}

bool Mensagem::setRemetente(const string& R)
{
  if (!testarNomeUsuario(R)) return false;
  remetente=R;
  return true;
}

const string& Mensagem::getDestinatario() const
{
  return destinatario;
}

bool Mensagem::setDestinatario(const string& D)
{
  if (!testarNomeUsuario(D)) return false;
  destinatario=D;
  return true;
}

const string& Mensagem::getTexto() const
{
  return texto;
}

bool Mensagem::setTexto(const string& T)
{
  if (!testarTextoMensagem(T)) return false;
  texto=T;
  return true;
}

MsgStatus Mensagem::getStatus() const
{
  return status;
}

bool Mensagem::setStatus(MsgStatus S)
{
  if (!testarStatus(S)) return false;
  status=S;
  return true;
}

/// Os testes de igualdade entre uma msg e os tipos auxiliares
/// que sao utilizados pelos algoritmos STL: find, count, etc
bool Mensagem::operator==(const testRemetStatus& T) const
{
  return remetente==T.remetente && status==T.status;
}

bool Mensagem::operator==(const testRemetId& T) const
{
  return remetente==T.remetente && id==T.id;
}

bool Mensagem::operator==(const testDestStatus& T) const
{
  return destinatario==T.destinatario && status==T.status;
}

bool Mensagem::operator==(const testDestId& T) const
{
  return destinatario==T.destinatario && id==T.id;
}

/// Ler de arquivo
/// Recebe como entrada uma ifstream jah aberta
/// A ifstream deve ser aberta pela funcao de ler de um nivel superior
bool Mensagem::ler(ifstream& arq)
{
  int32_t prov_id;
  int prov_status;
  string prov_remetente;
  string prov_destinatario;
  string prov_texto;

  if (!arq.is_open()) return false;
  arq >> prov_id >> prov_status >> prov_remetente >> prov_destinatario;
  // Descarta newline, espacos em branco, etc
  arq >> ws;
  getline(arq,prov_texto);
  if (prov_id<=0 ||
      !testarStatus((MsgStatus)prov_status) ||
      !testarNomeUsuario(prov_remetente) ||
      !testarNomeUsuario(prov_destinatario) ||
      !testarTextoMensagem(prov_texto))
  {
    id = 0;
    status = MsgStatus::MSG_INVALIDA;
    remetente = destinatario = texto = "";
    return false;
  }
  id = prov_id;
  status = (MsgStatus)prov_status;
  remetente = prov_remetente;
  destinatario = prov_destinatario;
  texto = prov_texto;
  return true;
}

/// Salvar em arquivo
/// Recebe como entrada uma ofstream jah aberta
/// A ofstream deve ser aberta pela funcao de salvar de um nivel superior
bool Mensagem::salvar(ofstream& arq) const
{
  if (!arq.is_open()) return false;
  arq << id << ' ' << (int)status << ' '
      << remetente << ' ' << destinatario << endl;
  arq << texto << endl;
  return true;
}

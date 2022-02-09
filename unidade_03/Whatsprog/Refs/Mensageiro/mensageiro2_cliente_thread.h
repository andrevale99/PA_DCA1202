#ifndef MENSAGEIRO2_CLIENTE_THREAD_H
#define MENSAGEIRO2_CLIENTE_THREAD_H

#include <QObject>
#include <QString>
#include <thread>

class Mensageiro2_Cliente_Thread: public QObject
{
  Q_OBJECT

private:
  /// A thread de leitura do socket no cliente
  std::thread thr;

  /// Este eh o metodo principal da thread (laco sem fim)
  /// Ele recebe as informacoes do socket e exibe atraves da emissao de sinais
  void main_thread();

  /// A funcao main_thread() executa o metodo main_thread()
  friend void main_thread(Mensageiro2_Cliente_Thread *mct);

  /// Thread estah ou nao em execucao
  bool em_execucao;

signals:
  void signExibirThreadIniciada();
  void signExibirMsgRecebida(QString, QString);
  void signExibirErro(QString, QString);
  void signExibirThreadEncerrada();

public slots:
  /// Iniciar a thread
  bool slotIniciar();
  /// Encerrar e aguardar fim da thread
  void slotEncerrar();

public:
  /// Construtor
  explicit Mensageiro2_Cliente_Thread(QObject *parent = nullptr);

  /// Funcao de consulta do estado da thread
  bool executing() const;
};

#endif // MENSAGEIRO2_CLIENTE_THREAD_H

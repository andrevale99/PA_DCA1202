#ifndef MAINCIRCUITO_H
#define MAINCIRCUITO_H

/**
 * @Dupla
 * 		André Menezes de Freitas Vale
 * 		Lucas Emanuel Costa Moisés
*/

#include <QMainWindow>
#include <QLabel>

#include "newcircuito.h"
#include "modificarporta.h"
#include "modificarsaida.h"

#include "circuit.h"

/* ======================================================================== *
 * ESSA EH A CLASSE QUE REPRESENTA A TELA PRINCIPAL DO APLICATIVO           *
 * ======================================================================== */

namespace Ui {
class MainCircuito;
}

class MainCircuito : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainCircuito(QWidget *parent = 0);
  ~MainCircuito();

private slots:
  // Redimensiona o circuito (realoca)
  void slotNewCircuito(int NInputs, int NOutputs, int NPortas);

  // Modifica uma porta
  void slotModificarPorta(int IdPort, QString TipoPort, int NumInputsPort,
                          int IdInput0, int IdInput1, int IdInput2, int IdInput3);

  // Modifica uma saida
  void slotModificarSaida(int idSaida, int idOrigemSaida);

  // Sai do aplicativo
  void on_actionSair_triggered();

  // Exibe a caixa de dialogo para fixar caracteristicas de um novo circuito
  void on_actionNovo_triggered();

  // Abre uma caixa de dialogo para ler um arquivo
  void on_actionLer_triggered();

  // Abre uma caixa de dialogo para salvar um arquivo
  void on_actionSalvar_triggered();

  // Gera e exibe a tabela verdade para o circuito
  // Chama a funcao simular da classe circuito
  void on_actionGerar_tabela_triggered();

  // Exibe a caixa de dialogo para fixar caracteristicas de uma porta
  void on_tablePortas_activated(const QModelIndex &index);

  // Exibe a caixa de dialogo para fixar caracteristicas de uma saida
  void on_tableSaidas_activated(const QModelIndex &index);

signals:
  // Sinaliza uma alteracao nas caracteristicas essenciais do circuito
  // Numero de entradas e de portas
  // Enviado apos a criacao de novo circuito
  void signSetRangeInputs(int minimo, int maximo);
  // Sinaliza a necessidade de exibicao da janela de novo circuito
  void signShowNewCircuito();
  // Sinaliza a necessidade de exibicao da janela de modificar porta
  void signShowModificarPorta(int IdPort, QString TipoPort, int NumInputsPort,
                              int IdInput0, int IdInput1, int IdInput2, int IdInput3);
  // Sinaliza a necessidade de exibicao da janela de modificar saida
  void signShowModificarSaida(int IdSaida, int IdOrigemSaida);

private:
  Ui::MainCircuito *ui;

  Circuit C;

  // Os exibidores dos valores na barra de status
  QLabel *numIn;     // Exibe o numero de entradas do circuito na barra de status
  QLabel *numOut;    // Exibe o numero de saidas do circuito na barra de status
  QLabel *numPortas; // Exibe o numero de portas do circuito na barra de status

  // As caixas pop up para digitacao de valores do circuito
  NewCircuito *newCircuito;        // Caixa de dialogo para criar um novo circuito
  ModificarPorta *modificarPorta;  // Caixa de dialogo para modificar uma porta
  ModificarSaida *modificarSaida;  // Caixa de dialogo para modificar uma saida

  // Redimensiona todas as tabelas e reexibe todos os valores da barra de status
  // Essa funcao deve ser chamada sempre que mudar o circuito (digitar ou ler de arquivo)
  void redimensionaTabelas();

  // Exibe os dados da i-esima porta
  // Essa funcao deve ser chamada sempre que mudar caracteristicas da porta
  // A funcao redimensiona_tabela jah chama essa funcao para todas as portas
  void showPort(unsigned i);

  // Exibe os dados da i-esima saida
  // Essa funcao deve ser chamada sempre que mudar valores da saida
  // A funcao redimensiona_tabela jah chama essa funcao para todas as saidas
  void showOutput(unsigned i);

  // Limpa o resultado da simulacao (tabela verdade)
  void limparTabelaVerdade();
};

#endif // MAINCIRCUITO_H

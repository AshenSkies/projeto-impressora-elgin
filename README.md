
# **Sistema de Controle de Impressora Fiscal/SAT**

## **Descrição Geral**

Este projeto envolve um programa em C99 desenvolvido para demonstrar o uso de DLLs como bibliotecas de código.

O sistema implementa um menu interativo de console que permite:

* Abrir e fechar conexão com a impressora
* Imprimir texto simples
* Imprimir QR Codes e códigos de barras
* Imprimir XML SAT (venda)
* Imprimir XML de Cancelamento SAT
* Controlar gavetas de dinheiro (Elgin e padrão)
* Emitir sinais sonoros
* Configurar parâmetros da impressora

Sua finalidade é servir de *base educacional* para uso das APIs da impressora e manipulação de arquivos XML de cupons fiscais.

---

## **Arquitetura**

O sistema é dividido em 5 componentes:

1. **Carregamento Dinâmico da DLL**

   * Com *LoadLibraryA*, *GetProcAddress* e *FreeLibrary*
   * Mapeamento automático das funções exportadas pela DLL

2. **Configurações da Impressora**

   * Tipo (USB, TCP, RS232, etc.)
   * Modelo (i7, i9, MP-4200 etc.)
   * Conexão (porta, IP, etc.)
   * Parâmetros extras

3. **Menu Principal**

   * Chamadas para cada operação da API da impressora

4. **Funções de Impressão**

   * Texto
   * QR Code
   * Código de Barras
   * XML SAT e XML de Cancelamento

5. **Funções Auxiliares**

   * Leitura de arquivos para memória
   * Limpeza do buffer de entrada
   * Avanço de papel + corte automático

---

## **Funções da DLL Utilizadas**

A seguir, uma lista com explicação resumida de todas as funções importadas da DLL.

 `AbreConexaoImpressora(int tipo, const char* modelo, const char* conexao, int param)`

Abre a conexão física com a impressora.

 `FechaConexaoImpressora()`

Fecha a conexão atual da impressora.

 `InicializaImpressora()`

Reinicializa o buffer interno da impressora.

 `ImpressaoTexto(const char *texto, int alinhamento, int fonte, int tamanho)`

Imprime texto comum.

 `ImpressaoQRCode(const char *dados, int tamanho, int nivelCorrecao)`

Imprime um QR Code básico.

 `ImpressaoCodigoBarras(int tipo, const char *dados, int altura, int largura, int hri)`

Imprime um código de barras (EAN, CODE128, etc.).

 `Corte(int tipo)`

Executa corte parcial ou total dependendo do modelo.

 `AvancaPapel(int linhas)`

Avança o papel para evitar corte sobre texto.

 `AbreGavetaElgin(int acao, int tempoOn, int tempoOff)`

Comando específico da Elgin.

 `AbreGaveta(int pino, int tempoOn, int tempoOff)`

Comando padrão ESC/POS.

 `SinalSonoro(int qtd, int tempo, int pausa)`

Aciona o beep interno da impressora.

`int ImprimeXMLSAT(const char* dados, int param);`

Aceita:

* O **conteúdo completo do XML**, ou
* Um caminho no formato:
    `path=C:/pasta/arquivo.xml`

`int ImprimeXMLCancelamentoSAT(const char* dados, const char* assQRCode, int param);`

A assinatura do QRCode **deve** ser enviada sempre.


## **Funções Auxiliares**

### `flush_entrada()`

Espera e limpa caracteres pendentes no `stdin`.

### `exibirMenu()`

Mostra o menu principal com todas as opções.

### `carregarFuncoes()`

Carrega e valida todas as funções da DLL usando macros.

### `liberarBiblioteca()`

Finaliza a DLL ao sair do programa.

---

## **Fluxo Geral do Programa**

1. Carrega a DLL
2. Exibe o menu
3. Usuário escolhe operação
4. Sistema executa a função da DLL correspondente
5. Repete até o usuário escolher "Fechar e sair"

## Observações Importantes

- Alguns modelos de impressora têm limitações em cortes e QRCodes.
- O programa assume que a DLL está presente e atualizada.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Conven��o de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configura��o ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver s�mbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Fun��es para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Fun��es a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{
    // TODO: implementar exibi��o do menu principal com as op��es de impress�o
    printf("1 - Configurar Conexao\n");
    printf("2 - Abrir Conexao\n");
    printf("3 - Impressao Texto\n");
    printf("4 - Impressao QRCode\n");
    printf("5 - Impressao Cod Barras\n");
    printf("6 - Impressao XML SAT\n");
    printf("7 - Impressao XML Canc SAT\n");
    printf("8 - Abrir Gaveta Elgin\n");
    printf("9 - Abrir Gaveta\n");
    printf("10 - Sinal Sonoro\n");
    printf("0 - Fechar Conexao e Sair\n");
    printf("\n");
}

// buffers  de configuracao
int tipo_impressora = 0;
int modelo_impressora = 0;
int conexao_impressora = 0;
char modelo_impressora_value[100];
char conexao_impressora_value[100];
int conexao_configurada = 1;

// Configura a conex�o com o dispositivo, pedindo parametros do mesmo.
static void configurarConexao(void)
{
	flush_entrada();
    // TODO: pedir ao usu�rio tipo, modelo, conex�o e par�metro
    
    // opcoes de tipo
    while (tipo_impressora < 1 || tipo_impressora > 5) {
	    printf("Digite o tipo da impressora:\n");
	    printf("1 - USB\n");
	    printf("2 - RS232\n");
	    printf("3 - TCP-IP\n");
	    printf("4 - Bluetooth\n");
	    printf("5 - Impressoras Acopladas (Android)\n");
    
    	scanf("%d", &tipo_impressora);
    	printf("\n");
	}
    
    // opcoes de modelo
    while (modelo_impressora < 1 || modelo_impressora > 12) {
	    printf("Digite o modelo da impressora:\n");
	    printf("1 - i7\n");
	    printf("2 - i7 Plus\n");
	    printf("3 - i8\n");
	    printf("4 - i9\n");
	    printf("5 - ix\n");
	    printf("6 - Fitpos\n");
	    printf("7 - BK-T681\n");
	    printf("8 - MP-4200 (TH e ADV)\n");
	    printf("9 - MP-4200 HS\n");
	    printf("10 - MK\n");
	    printf("11 - MP-2800\n");
    
    	scanf("%d", &modelo_impressora);
    	printf("\n");
	}
	switch(modelo_impressora){
    	case 1: strcpy(modelo_impressora_value, "i7"); break;
    	case 2: strcpy(modelo_impressora_value, "i7 Plus"); break;
    	case 3: strcpy(modelo_impressora_value, "i8"); break;
    	case 4: strcpy(modelo_impressora_value, "i9"); break;
    	case 5: strcpy(modelo_impressora_value, "ix"); break;
    	case 6: strcpy(modelo_impressora_value, "Fitpos"); break;
    	case 7: strcpy(modelo_impressora_value, "BK-T681"); break;
    	case 8: strcpy(modelo_impressora_value, "MP-4200 (TH e ADV)"); break;
    	case 9: strcpy(modelo_impressora_value, "MP-4200 HS"); break;
    	case 10: strcpy(modelo_impressora_value, "MK"); break;
    	case 11: strcpy(modelo_impressora_value, "MP-2800"); break;
	}
    
    // opcoes de conexao
    while (conexao_impressora < 1 || conexao_impressora > 5) {
	    printf("Digite o tipo da conexao:\n");
	    printf("1 - USB\n");
	    printf("2 - RS232\n");
	    printf("3 - TCP-IP\n");
	    printf("4 - Bluetooth\n");
	    printf("5 - Impressoras Acopladas (Android)\n");
    
    	scanf("%d", &conexao_impressora);
    	printf("\n");
	}
	switch(modelo_impressora){
    	case 1: strcpy(conexao_impressora_value, "USB"); break;
    	case 2: strcpy(conexao_impressora_value, "COM2"); break;
    	case 3: strcpy(conexao_impressora_value, "192.168.0.20"); break;
    	case 4: strcpy(conexao_impressora_value, "AA:BB:CC:DD:EE:FF"); break;
    	case 5: strcpy(conexao_impressora_value, ""); break;
	}
	conexao_configurada = 0;
}

int conexao_aberta = 1;

// Abre uma conex�o caso a mesma esteja configurada.
// Retorna ao menu caso contrario.
static void abrirConexao(void)
{	
	// TODO: chamar AbreConexaoImpressora e validar retorno
	if (conexao_configurada != 0) {
		printf("A conexao ainda nao foi configurada.\n\n");
		return;
	}
	else if (conexao_aberta == 0) {
		printf("Ja ha uma conexao pre-estabelecida.\n\n");
		return;
	}
	int ret = AbreConexaoImpressora(tipo_impressora, modelo_impressora_value, conexao_impressora_value, 0);
	if (ret != 0) {
		printf("Houve um erro de codigo %d durante a abertura de conexao.", ret);
		return;
	}
	conexao_aberta = 0;
	
}

// Fecha a conex�o seguramente.
static void fecharConexao(void)
{
    // TODO: chamar FechaConexaoImpressora e tratar retorno
	if (conexao_aberta != 0) {
		printf("A conexao ainda nao foi estabelecida com a impressora.\n\n");
		return;
	}
	FechaConexaoImpressora();
	conexao_aberta = 1;
    
}

// Pede um input de texto e formata��o ao usu�rio, e imprime.
static void imprimirTexto(void)
{
	flush_entrada();
    // TODO: solicitar texto do usu�rio e chamar ImpressaoTexto
    // incluir AvancaPapel e Corte no final
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
	
	char conteudo[30];
	printf("Digite o conteudo da impressao: ");
	scanf("%s", &conteudo);
	
	int posicao = 0;
	while (posicao < 1 || posicao > 3) {
	    printf("Digite o alinhamento da impressao:\n");
	    printf("1 - Esquerda\n");
	    printf("2 - Centro\n");
	    printf("3 - Direita\n");
    
    	scanf("%d", &posicao);
    	printf("\n");
	}
	posicao = posicao-1;
	
	int tamanho = -1;
	while (tamanho < 0 || (tamanho > 7 && tamanho < 16) || tamanho > 16) {
	    printf("Digite o tamanho da impressao:\n");
	    printf("0\n");
	    printf("1\n");
	    printf("2\n");
	    printf("3\n");
	    printf("4\n");
	    printf("5\n");
	    printf("6\n");
	    printf("7\n");
	    printf("16\n");
    
    	scanf("%d", &tamanho);
    	printf("\n");
	}
    	
	int ret = ImpressaoTexto(conteudo, posicao, 0, tamanho);
	
	if(ret >= 0){
		printf("Impressao OK...\n");
		AvancaPapel(5);
		Corte(2);
	}else{
		printf("Erro. Retorno de valor %d \n", ret);
	}
    
}

// Pede um input de texto e formata��o ao usu�rio, e imprime em formato de QRCode
static void imprimirQRCode(void)
{
	flush_entrada();
    // TODO: solicitar conte�do do QRCode e chamar ImpressaoQRCode(texto, 6, 4)
    // incluir AvancaPapel e Corte no final
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
	
	char conteudo[30];
	printf("Digite o conteudo do QRCode: ");
	scanf("%s", &conteudo);
	
	int tamanho = -1;
	while (tamanho < 1 || tamanho > 6) {
	    printf("Digite o tamanho da impressao:\n");
	    printf("1\n");
	    printf("2\n");
	    printf("3\n");
	    printf("4\n");
	    printf("5\n");
	    printf("6\n");
    
    	scanf("%d", &tamanho);
    	printf("\n");
	}
    	
	int ret = ImpressaoQRCode(conteudo, tamanho, 2);
	
	if(ret >= 0){
		printf("Impressao OK\n");
		AvancaPapel(5);
		Corte(2);
	}else{
		printf("Erro. Retorno de valor %d \n", ret);
	}
}

// Imprime um c�digo de barras de teste.
static void imprimirCodigoBarras(void)
{
	flush_entrada();
    // TODO: usar ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3)
    // incluir AvancaPapel e Corte no final
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
    	
	int ret = ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
	
	if(ret >= 0){
		printf("Impressao OK\n");
		AvancaPapel(5);
		Corte(2);
	}else{
		printf("Erro. Retorno de valor %d \n", ret);
	}
}

// Imprime um SAT de um arquivo XML de teste.
static void imprimirXMLSAT(void)
{
	flush_entrada();
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
    const char *file_path = "path=./XMLSAT.xml";

    // Chama a funcao de impressao do XML do SAT
    int ret = ImprimeXMLSAT(file_path, 0);

    // Tratamento de retorno
    if (ret == 0)
        printf("Impressao concluida via caminho.\n");
    else
        printf("Erro ao imprimir XMLSAT (ret=%d)\n", ret);
    
    // Avanca o papel por 5 linhas e realiza o corte
    AvancaPapel(5);
    Corte(0);
}

// Imprime um SAT de cancelamento de um arquivo XML de teste.
static void imprimirXMLCancelamentoSAT(void)
{
	flush_entrada();
    // TODO: ler o arquivo ./CANC_SAT.xml e chamar ImprimeXMLCancelamentoSAT
    // incluir AvancaPapel e Corte no final
    
	/*usar assinatura abaixo:
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
        */
	
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
    const char *file_path = "path=./CANC_SAT.xml";
    const char *assinatura = "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
                             "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
                             "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
                             "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
                             "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
                             "YVFCDtYR9Hi5qgdk31v23w==";
    
    // Chama a funcao de impressao do XML de cancelamento do SAT
    int ret = ImprimeXMLCancelamentoSAT(file_path, assinatura, 0);

    // Tratamento de retorno
    if (ret == 0)
        printf("Impressao concluida via caminho.\n");
    else
        printf("Erro ao imprimir XMLSAT (ret=%d)\n", ret);
    
    // Avanca o papel por 5 linhas e realiza o corte
    AvancaPapel(5);
    Corte(0);
}

// Abre a Gaveta com os padr�es Elgin.
static void abrirGavetaElginOpc(void)
{
	flush_entrada();
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
    // TODO: chamar AbreGavetaElgin(1, 50, 50)
    AbreGavetaElgin(1, 50, 50);
}

// Abre a Gaveta com parametros customizados.
static void abrirGavetaOpc(void)
{
	flush_entrada();
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
    // TODO: chamar AbreGaveta(1, 5, 10)
    AbreGaveta(1, 5, 10);
}

// Emite um sinal sonoro com parametro de duracao.
static void emitirSinalSonoro(void)
{
	flush_entrada();
    if (conexao_aberta != 0) {
		printf("A impressora nao esta conectada.\n");
		return;
	}
	int tamanho = -1;
	while (tamanho < 10 || tamanho > 50) {
	    printf("Digite a dura��o dos bipes (entre 10 e 50):\n");
    
    	scanf("%d", &tamanho);
    	printf("\n");
	}
    // TODO: chamar SinalSonoro(4, 50, 5)
    SinalSonoro(4, tamanho, 5);
}

/* ======================= Fun��o principal ======================= */
int main(void)
{
    if (!carregarFuncoes()) {
        return 1;
    }

    int opcao = 0;
    while (1) {
        
        //construir o menu e chamar as fun�oes aqui!!!
        exibirMenu();
        scanf("%d", &opcao);
        switch(opcao) {
        	case 1: configurarConexao(); break;
        	case 2: abrirConexao(); break;
        	case 3: imprimirTexto(); break;
        	case 4: imprimirQRCode(); break;
        	case 5: imprimirCodigoBarras(); break;
        	case 6: imprimirXMLSAT(); break;
        	case 7: imprimirXMLCancelamentoSAT(); break;
        	case 8: abrirGavetaElginOpc(); break;
        	case 9: abrirGavetaOpc(); break;
        	case 10: emitirSinalSonoro(); break;
        	case 0: fecharConexao(); return 0;
		}
        
    }
}


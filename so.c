#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSOS 5
#define QUANTUM 2

typedef struct {
    int id;
    int bilhetes;
    int prioridade;
    int tempoCPU;
    int tempoExecucao;
    int tempoChegada;
} Processo;

Processo processos[] = {
    {1, 10, 2, 10, 20, 0},
    {2, 5, 1, 8, 30, 0},
    {3, 2, 3, 6, 10, 0},
    {4, 8, 5, 15, 40, 0},
    {5, 15, 4, 12, 25, 0}
};

int n = sizeof(processos) / sizeof(processos[0]);

// ID dos itens de menu
#define IDM_FIF0 101
#define IDM_SJF 102
#define IDM_SRT 103
#define IDM_RR 104
#define IDM_PRIORIDADES 105
#define IDM_ESCALONAMENTO_GARANTIDO 106
#define IDM_ESCALONAMENTO_LOTERIA 107
#define IDM_ESCALONAMENTO_FRACAO_JUSTA 108
#define IDM_SAIR 109

// Função de tratamento de mensagens da janela
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDM_FIF0:
                {
                    printf("Ordem de execucao dos processos (FIFO):\n");
                    for (int i = 0; i < n; i++) {
                        printf("%d ", processos[i].id);
                    }
                    printf("\n");
                    MessageBox(hwnd, "Algoritmo FIFO selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_SJF:
                {
                    for (int i = 0; i < n - 1; i++) {
                        for (int j = 0; j < n - i - 1; j++) {
                            if (processos[j].tempoExecucao > processos[j + 1].tempoExecucao) {
                                Processo temp = processos[j];
                                processos[j] = processos[j + 1];
                                processos[j + 1] = temp;
                            }
                        }
                    }

                    printf("Ordem de execucao dos processos (SJF):\n");
                    for (int i = 0; i < n; i++) {
                        printf("%d ", processos[i].id);
                    }
                    printf("\n");
                    MessageBox(hwnd, "Algoritmo SJF selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_SRT:
                {
                    printf("Ordem de execucao dos processos (SRT):\n");

                    int tempo_restante[n];
                    for (int i = 0; i < n; i++) {
                        tempo_restante[i] = processos[i].tempoExecucao;
                    }

                    int concluidos = 0;
                    int tempo_atual = 0;
                    while (concluidos != n) {
                        int processo_atual = -1;
                        int menor_tempo_restante = 9999999;
                        for (int i = 0; i < n; i++) {
                            if (tempo_restante[i] > 0 && tempo_restante[i] < menor_tempo_restante) {
                                processo_atual = i;
                                menor_tempo_restante = tempo_restante[i];
                            }
                        }

                        if (processo_atual == -1) {
                            tempo_atual++;
                            continue;
                        }

                        printf("%d ", processos[processo_atual].id);
                        tempo_restante[processo_atual]--;
                        if (tempo_restante[processo_atual] == 0) {
                            concluidos++;
                        }
                        tempo_atual++;
                    }

                    printf("\n");
                    MessageBox(hwnd, "Algoritmo SRT selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_RR:
                {
                    printf("Ordem de execucao dos processos (Round Robin):\n");

                    int tempo_restante[n];
                    for (int i = 0; i < n; i++) {
                        tempo_restante[i] = processos[i].tempoExecucao;
                    }

                    int concluidos = 0;
                    int tempo_atual = 0;
                    while (concluidos != n) {
                        for (int i = 0; i < n; i++) {
                            if (tempo_restante[i] > 0) {
                                if (tempo_restante[i] <= QUANTUM) {
                                    printf("%d ", processos[i].id);
                                    tempo_atual += tempo_restante[i];
                                    tempo_restante[i] = 0;
                                    concluidos++;
                                } else {
                                    printf("%d ", processos[i].id);
                                    tempo_atual += QUANTUM;
                                    tempo_restante[i] -= QUANTUM;
                                }
                            }
                        }
                    }

                    printf("\n");
                    MessageBox(hwnd, "Algoritmo Round Robin selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_PRIORIDADES:
                {
                    for (int i = 0; i < n - 1; i++) {
                        for (int j = 0; j < n - i - 1; j++) {
                            if (processos[j].prioridade > processos[j + 1].prioridade) {
                                Processo temp = processos[j];
                                processos[j] = processos[j + 1];
                                processos[j + 1] = temp;
                            }
                        }
                    }

                    printf("Ordem de execucao dos processos (por prioridades):\n");
                    for (int i = 0; i < n; i++) {
                        printf("%d ", processos[i].id);
                    }
                    printf("\n");
                    MessageBox(hwnd, "Algoritmo por Prioridades selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_ESCALONAMENTO_GARANTIDO:
                {
                    printf("Escalonamento Garantido\n");
                    int tempoTotal = 0;

                    for (int i = 0; i < n; i++) {
                        tempoTotal += processos[i].tempoExecucao;
                    }

                    printf("ID\tTempo de Execucao\tTempo de CPU\n");

                    for (int i = 0; i < n; i++) {
                        float proporcao = (float)processos[i].tempoExecucao / tempoTotal;
                        float tempoCPU = proporcao * 100;

                        printf("%d\t%d\t\t%.2f%%\n",
                            processos[i].id,
                            processos[i].tempoExecucao,
                            tempoCPU);
                    }

                    MessageBox(hwnd, "Algoritmo de Escalonamento Garantido selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_ESCALONAMENTO_LOTERIA:
                {
                    printf("Escalonamento por Loteria\n");
                    int totalBilhetes = 0;

                    for (int i = 0; i < n; i++) {
                        totalBilhetes += processos[i].bilhetes;
                    }

                    printf("ID\tBilhetes\tProbabilidade\n");

                    for (int i = 0; i < n; i++) {
                        float probabilidade = (float)processos[i].bilhetes / totalBilhetes * 100;

                        printf("%d\t%d\t\t%.2f%%\n",
                            processos[i].id,
                            processos[i].bilhetes,
                            probabilidade);
                    }

                    srand(time(NULL));

                    int bilheteVencedor = rand() % totalBilhetes + 1;
                    int bilheteAtual = 0;
                    int processoVencedor = -1;

                    for (int i = 0; i < n; i++) {
                        bilheteAtual += processos[i].bilhetes;

                        if (bilheteVencedor <= bilheteAtual) {
                            processoVencedor = i;
                            break;
                        }
                    }

                    if (processoVencedor != -1) {
                        printf("\nO processo vencedor e o processo %d\n", processos[processoVencedor].id);
                    }

                    MessageBox(hwnd, "Algoritmo de Escalonamento por Loteria selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_ESCALONAMENTO_FRACAO_JUSTA:
                {
                    printf("Escalonamento por Fracao Justa\n");
                    int tempoTotal = 0;
                    int temposParciais[MAX_PROCESSOS] = {0};

                    for (int i = 0; i < n; i++) {
                        tempoTotal += processos[i].tempoCPU;
                    }

                    printf("ID\tPrioridade\tTempo de CPU\tFracao Justa\n");

                    for (int i = 0; i < n; i++) {
                        float fracaoJusta = (float)processos[i].tempoCPU / tempoTotal;

                        printf("%d\t%d\t\t%d\t\t%.2f\n",
                            processos[i].id,
                            processos[i].prioridade,
                            processos[i].tempoCPU,
                            fracaoJusta);

                        temposParciais[i] = processos[i].tempoCPU;
                    }

                    printf("\nExecutando processos:\n");

                    while (tempoTotal > 0) {
                        for (int i = 0; i < n; i++) {
                            if (temposParciais[i] > 0) {
                                printf("Executando processo %d\n", processos[i].id);
                                temposParciais[i]--;
                                tempoTotal--;
                            }
                        }
                    }

                    MessageBox(hwnd, "Algoritmo de Escalonamento por Fração Justa selecionado", "Escalonamento", MB_OK);
                    break;
                }
                case IDM_SAIR:
                    // Fechar a janela e encerrar o programa
                    DestroyWindow(hwnd);
                    break;
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Registrar a classe da janela
    const char CLASS_NAME[] = "EscalonamentoClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Criar a janela principal
    HWND hwnd = CreateWindowEx(
        0,                              // Estilo adicional
        CLASS_NAME,                     // Classe da janela
        "Escalonamento",                // Título da janela
        WS_OVERLAPPEDWINDOW,            // Estilo da janela
        CW_USEDEFAULT, CW_USEDEFAULT,   // Posição inicial da janela
        500, 300,                       // Largura e altura da janela
        NULL,                           // Janela pai
        NULL,                           // Menu da janela
        hInstance,                      // Instância do aplicativo
        NULL                            // Dados adicionais
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Criar o menu
    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreateMenu();

    // Adicionar os itens de menu
    AppendMenu(hSubMenu, MF_STRING, IDM_FIF0, "FIFO - Primeiro a Entrar Primeiro a Sair");
    AppendMenu(hSubMenu, MF_STRING, IDM_SJF, "SJF - Tarefa Mais Curta Primeiro");
    AppendMenu(hSubMenu, MF_STRING, IDM_SRT, "SRT - Tempo Remanescente mais Curto");
    AppendMenu(hSubMenu, MF_STRING, IDM_RR, "Round Robin");
    AppendMenu(hSubMenu, MF_STRING, IDM_PRIORIDADES, "Prioridades");
    AppendMenu(hSubMenu, MF_STRING, IDM_ESCALONAMENTO_GARANTIDO, "Escalonamento Garantido");
    AppendMenu(hSubMenu, MF_STRING, IDM_ESCALONAMENTO_LOTERIA, "Escalonamento por Loteria");
    AppendMenu(hSubMenu, MF_STRING, IDM_ESCALONAMENTO_FRACAO_JUSTA, "Escalonamento por Fracao Justa");
    AppendMenu(hSubMenu, MF_STRING, IDM_SAIR, "Sair");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Algoritmos");
    SetMenu(hwnd, hMenu);

    // Exibir a janela
    ShowWindow(hwnd, nCmdShow);

    // Loop de mensagens da janela
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_ITENS 100
#define TAM_NOME 50

char nomes[MAX_ITENS][TAM_NOME];
float precos[MAX_ITENS];
int quantidades[MAX_ITENS];
int total_item = 0;
int carrinho_id[MAX_ITENS];
int carrinho_qtd[MAX_ITENS];
int carrinho_total = 0;

void pause() {  //pausa para o usuario ler
    printf("\nAperte ENTER para continuar...");
    while (getchar() != '\n');
}

void adicionar() {  //adiciona itens na lista do estoque
    if (total_item >= MAX_ITENS) {
        printf("\nCapacidade maxima atingida!\n");
        return;
    }

    printf("Nome do item: ");
    scanf(" %49[^\n]", nomes[total_item]);
    getchar();

    printf("Preco: ");
    scanf("%f", &precos[total_item]);
    getchar();

    printf("Quantidade disponivel: ");
    scanf("%d", &quantidades[total_item]);
    getchar();

    total_item++;

    printf("\nItem adicionado!\n");
}

void listar() { //mostra a lista do estoque
    int i;
    if (total_item == 0) {
        printf("\nNenhum item cadastrado.\n");
        return;
    }

    printf("\n=== ITENS CADASTRADOS ===\n");
    for (i = 0; i < total_item; i++) {
        printf("ID[%d] %s - R$ %.2f (Qtd: %d)\n",
               i + 1, nomes[i], precos[i], quantidades[i]);
    }
}



void remover() {    //remove itens da lista do estoque
    int i;
    if (total_item == 0) {
        printf("\nNenhum item para remover.\n");
        pause();
        return;
    }

    listar();   
    printf("\nID do item para remover: ");
    int id;
    scanf("%d", &id);
    getchar();
    id--;

    if (id < 0 || id >= total_item) {
        printf("ID invalido!\n");
        pause();
        return;
    }

    for (i = id; i < total_item - 1; i++) {
        strcpy(nomes[i], nomes[i + 1]);
        precos[i] = precos[i + 1];
        quantidades[i] = quantidades[i + 1];
    }

    total_item--;
    printf("\nItem removido!\n");
    pause();
}


void alterar() {    //altera os itens no estoque
    if (total_item == 0) {
        printf("\nNenhum item para alterar.\n");
        pause();
        return;
    }

    listar();
    printf("\nID do item para alterar: ");
    int id;
    scanf("%d", &id);
    getchar();
    id--;

    if (id < 0 || id >= total_item) {
        printf("ID invalido!\n");
        pause();
        return;
    }

    printf("Novo nome: ");
    scanf(" %49[^\n]", nomes[id]);
    getchar();

    printf("Novo preco: ");
    scanf("%f", &precos[id]);
    getchar();

    printf("Nova quantidade: ");
    scanf("%d", &quantidades[id]);
    getchar();

    printf("\nItem atualizado!\n");
    pause();
}


void admin() {
    int opc;

    do {
        Sleep(300);
        system("cls");

        printf("\n==> MENU ADMIN <==\n");
        printf("1. Listar itens\n");
        printf("2. Adicionar item(s)\n");
        printf("3. Remover item\n");
        printf("4. Alterar item\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        system("cls");

        switch (opc) {
            case 1:
                listar();
                pause();
                break;

            case 2: {
                int cont = 1;
                while (cont != 0) {
                    adicionar();
                    printf("\nDigite 0 para parar de adicionar ou outro numero para continuar: ");
                    scanf("%d", &cont);
                    getchar();
                    system("cls");
                }
                break;
            }

            case 3:
                remover();
                break;

            case 4:
                alterar();
                break;

            case 0:
                printf("Voltando...\n");
                Sleep(500);
                break;

            default:
                printf("Opcao invalida!\n");
                pause();
        }

    } while (opc != 0);
}

void mostrarCarrinho() {    //mostra o carrinho do usuario
    int i;
    if (carrinho_total == 0) {
        printf("\nCarrinho vazio.\n");
        return;
    }

    printf("\n=== CARRINHO ===\n");
    float total = 0;

    for (i = 0; i < carrinho_total; i++) {
        int id = carrinho_id[i];
        float parcial = precos[id] * carrinho_qtd[i];
        total += parcial;

        printf("[%d]%s - Qtd: %d - R$ %.2f\n",
               id+1,nomes[id], carrinho_qtd[i], parcial);
    }

    printf("\nTOTAL: R$ %.2f\n", total);
}

float totalCarrinho() { //mostra o preço dos itens no carrinho
    int i;
    float total = 0;
    for (i = 0; i < carrinho_total; i++) {
        total += precos[carrinho_id[i]] * carrinho_qtd[i];
    }
    return total;
}

void removerDoCarrinho() {  //remove itens do carrinho
    int i;
    if (carrinho_total == 0) {
        printf("\nCarrinho vazio.\n");
        return;
    }

    mostrarCarrinho();

    printf("\nNumero do item no carrinho para remover: ");
    int pos;
    scanf("%d", &pos);
    getchar();
    pos--;

    if (pos < 0 || pos >= carrinho_total) {
        printf("\nID invalido!\n");
        return;
    }

    int id = carrinho_id[pos];

    printf("Quantidade para remover: ");
    int qtd_remover;
    scanf("%d", &qtd_remover);
    getchar();

    // Caso o usuário tente remover mais itens do que existem no carrinho ele retira todos os itens
    if (qtd_remover >= carrinho_qtd[pos]) {
        qtd_remover = carrinho_qtd[pos];  
    }

    quantidades[id] += qtd_remover; // Devolve ao estoque

    carrinho_qtd[pos] -= qtd_remover;   // Diminui do carrinho

    if (carrinho_qtd[pos] == 0) {   // Se zerou, remove o item da lista do carrinho
        for (i = pos; i < carrinho_total - 1; i++) {
            carrinho_id[i] = carrinho_id[i + 1];
            carrinho_qtd[i] = carrinho_qtd[i + 1];
        }
        carrinho_total--;
    }

    printf("\nItem removido!\n");
}


void pagamento() {  //realiza o pagamento
    float total = totalCarrinho();
    printf("\nTotal da compra: R$ %.2f\n", total);

    printf("\n=== Formas de pagamento ===\n");
    printf("1 - A vista (5%% de DESCONTO)\n");
    printf("2 - Parcelado (5%% de JUROS simples ao mes, ate 12x)\n");
    printf("Escolha: ");

    int opc;
    scanf("%d", &opc);
    getchar();

    float valorFinal = total;

    if (opc == 1) { //desconto de vista
        float desconto = total * 0.05;
        valorFinal = total - desconto;
        printf("\nPagamento a vista com 5%% de desconto!\n");
        printf("Total com desconto: R$ %.2f\n", valorFinal);
    }
    else if (opc == 2) { //juros de parcelas
        int parcelas;
        printf("Numero de parcelas (1 a 12): ");
        scanf("%d", &parcelas);
        getchar();

        if (parcelas < 1 || parcelas > 12) parcelas = 12;

        float juros = total * 0.05 * parcelas;
        valorFinal = total + juros;

        printf("\nParcelado em %d vezes.\n", parcelas);
        printf("Valor final com juros: R$ %.2f\n", valorFinal);
        printf("Valor por parcela: R$ %.2f\n", valorFinal / parcelas);
    }

    int confirmar;  //painel de confirmação de compra
    printf("\nConfirmar compra? (1-Sim / 0-Nao): ");
    scanf("%d", &confirmar);
    getchar();

    if (confirmar != 1) {
        printf("\nCompra cancelada!\n");
        return;
    }

    printf("\n=== COMPROVANTE ===\n");
    mostrarCarrinho();
    printf("\nTotal pago: R$ %.2f\n", valorFinal);
    printf("Obrigado pela compra!\n");

    carrinho_total = 0;
}

void user() {
    int opc, i;

    do {
        Sleep(300);
        system("cls");

        printf("\n==> MENU USER <==\n");
        printf("1. Listar produtos\n");
        printf("2. Adicionar ao carrinho\n");
        printf("3. Remover do carrinho\n");
        printf("4. Ver carrinho\n");
        printf("5. Finalizar compra\n");
        printf("0. Voltar\n");
        printf("Escolha: ");

        scanf("%d", &opc);
        getchar();
        system("cls");

        switch (opc) {
            case 1:
                listar();
                pause();
                break;

            case 2: {
                int continuar = 1;

                while (continuar != 0) {
                    listar();
                    printf("\nID do produto para adicionar: ");
                    int id;
                    scanf("%d", &id);
                    getchar();
                    id--;

                    if (id < 0 || id >= total_item) {
                        printf("ID invalido!\n");
                        pause();
                        break;
                    }

                    printf("Quantidade: ");
                    int qtd;
                    scanf("%d", &qtd);
                    getchar();

                    if (qtd <= 0 || qtd > quantidades[id]) {
                        printf("Quantidade invalida!\n");
                        pause();
                        break;
                    }

                    int achou = 0;
                    for (i = 0; i < carrinho_total; i++) {
                        if (carrinho_id[i] == id) {
                            carrinho_qtd[i] += qtd;
                            achou = 1;
                            break;
                        }
                    }

                    if (!achou) {
                        carrinho_id[carrinho_total] = id;
                        carrinho_qtd[carrinho_total] = qtd;
                        carrinho_total++;
                    }

                    quantidades[id] -= qtd;

                    printf("\nItem adicionado ao carrinho!\n");

                    printf("\nDigite 0 para parar de adicionar ou outro numero para continuar: ");
                    scanf("%d", &continuar);
                    getchar();

                    system("cls");
                }

                break;
            }

            case 3:
                removerDoCarrinho();
                pause();
                break;

            case 4:
                mostrarCarrinho();
                pause();
                break;

            case 5:
                if (carrinho_total == 0) {
                    printf("\nCarrinho vazio!\n");
                    pause();
                } else {
                    pagamento();
                    pause();
                }
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
                pause();
        }

    } while (opc != 0);
}

int main() {
    int opc1, senha;

    do {
        Sleep(300);
        system("cls");
        printf("\n==> MENU INICIAL <==\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc1);
        getchar();

        system("cls");

        switch (opc1) {
            case 1:
                printf("Senha: ");
                scanf("%d", &senha);
                getchar();
                if (senha == 1234) admin();//senha default
                else {
                    printf("Senha incorreta!\n");
                    Sleep(1000);
                }
                break;

            case 2:
                user(); //menu usuario
                break;

            case 0:
                printf("Fechando...\n");
                Sleep(800);
                break;

            default:
                printf("Opcao invalida!\n");
                Sleep(500);
        }

    } while (opc1 != 0);

    return 0;
}

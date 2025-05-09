#include <stdio.h>

// Função para calcular mdc 
int mdc(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Teste de primalidade com base no mdc 
int sao_primos(int a, int b) {
    return mdc(a, b) == 1;
}

// Inverso modular de G em Z_n
int inverso_modular(int g, int n) {
    int t = 0, novo_t = 1;
    int r = n, novo_r = g;
    while (novo_r != 0) {
        int q = r / novo_r;
        int temp = t - q * novo_t;
        t = novo_t; novo_t = temp;

        temp = r - q * novo_r;
        r = novo_r; novo_r = temp;
    }
    if (r > 1) return -1;
    if (t < 0) t += n;
    return t;
}

// Totiente de Euler 
int phi(int n) {
    int result = n;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// Exponenciação modular eficiente
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    int H, G, n, x, n1;
    printf("Digite os valores de H, G, n, x e n1:\n");
    scanf("%d %d %d %d %d", &H, &G, &n, &x, &n1);

    printf("\nEtapa 1: Verificando se G e n são primos entre si...\n");
    if (!sao_primos(G, n)) {
        printf("G (%d) e n (%d) não são primos entre si. Divisão modular impossível.\n", G, n);
        return 1;
    }

    printf("\nEtapa 2: Calculando o inverso modular de G em Z%d...\n", n);
    int G_inv = inverso_modular(G, n);
    if (G_inv == -1) {
        printf("Não existe inverso modular de G em Z%d.\n", n);
        return 1;
    }
    printf("Inverso de %d mod %d é: %d\n", G, n, G_inv);

    printf("\nEtapa 3: Calculando a = H ⊘ G em Z%d => a = H * G⁻¹ mod n\n", n);
    int a = (H * G_inv) % n;
    printf("a = %d\n", a);

    printf("\nEtapa 4: Verificando se a e n1 são coprimos...\n");
    if (!sao_primos(a, n1)) {
        printf("a (%d) e n1 (%d) não são coprimos. Operação insegura.\n", a, n1);
        return 1;
    }

    printf("\nEtapa 5: Verificando se n1 é primo...\n");
    int primo_n1 = 1;
    for (int i = 2; i * i <= n1; i++) {
        if (n1 % i == 0) {
            primo_n1 = 0;
            break;
        }
    }

    int x1 = primo_n1 ? (n1 - 1) : phi(n1);
    printf("n1 %s primo. x₁ = %d\n", primo_n1 ? "é" : "não é", x1);

    printf("\nEtapa 8: Decompondo x como x = x1*q + r\n");
    int q = x / x1;
    int r = x % x1;
    printf("x = %d = %d * %d + %d\n", x, x1, q, r);

    printf("\nEtapa 9–11: Calculando valores intermediários e finais...\n");
    int x2 = mod_exp(a, x1, n1);
    int x2q = mod_exp(x2, q, n1);
    int ar = mod_exp(a, r, n1);
    int resultado = (x2q * ar) % n1;

    printf("a^x1 mod n1 = %d\n", x2);
    printf("(a^x1)^q mod n1 = %d\n", x2q);
    printf("a^r mod n1 = %d\n", ar);
    printf("Resultado final: ((%d^%d)^%d * %d^%d) mod %d = %d\n", a, x1, q, a, r, n1, resultado);

    return 0;
}

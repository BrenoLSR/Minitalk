# 🛰️ Minitalk — Comunicação Cliente/Servidor via Sinais (42)

Minitalk é um projeto da 42 que implementa comunicação entre processos utilizando exclusivamente sinais UNIX.  
O objetivo é enviar uma string do cliente para o servidor, bit a bit, usando SIGUSR1 e SIGUSR2.

---

## 📌 Descrição

O projeto consiste em dois programas:

- **server** → recebe sinais, reconstrói os caracteres e imprime a mensagem.
- **client** → envia a mensagem para o servidor, transformando cada caractere em uma sequência de 8 bits.

Cada bit é enviado como um sinal:
- **SIGUSR1** → bit 0  
- **SIGUSR2** → bit 1  

O servidor converte esses bits novamente em caracteres e os imprime.

---

## ⚙️ Funcionamento Técnico

### 🔹 Cliente
1. Lê o PID do servidor.
2. Converte cada caractere da string em binário.
3. Para cada bit:
   - Envia SIGUSR1 (bit 0)  
   - Envia SIGUSR2 (bit 1)
4. Aguarda confirmação (bônus).

### 🔹 Servidor
1. Recebe sinais continuamente.
2. Monta cada caractere, bit por bit.
3. Quando 8 bits são recebidos:
   - Imprime o caractere.
   - Se o caractere for `'\0'`, finaliza a mensagem.
4. Envia ACK ao cliente (bônus).

---

## 🛠️ Compilação

Use o Makefile:

```sh
make          # compila client e server
make bonus    # compila client_bonus e server_bonus
make clean
make fclean
make re



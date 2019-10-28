# SFS - Simple file system

Desenvolvido para a matéria de Arquitetura de Sistemas Operacionais - UFSC

## Pré-requisitos
- g++
- c++14
- Biblioteca pthread incluida no SO (Já incluída no MakeFile)
- Para visualização correta, máquina com pelo menos 2 threads reais

## Instalação
Utilize o arquivo make disponível. Navegue até a pasta raiz e execute

```bash
make
```
Um executavel será gerado dentro da pasta bin.

## Uso
Acesse o executável partindo da raiz 

```bash
./bin/sfs
```

Caso tudo tenha ocorrido bem, a mensagem de inicilização abaixo será mostrada
```bash
[Common] Iniciando criação de socket listener /tmp/DefaultConnection.sock
[Common] Limpando /tmp/DefaultConnection.sock
[Common] Ouvindo socket /tmp/DefaultConnection.sock
```
A pasta **./sfs-home** é o local utilizado como relativo nas conexões. Qualquer usuário que se conecte por um socket, será iniciado na pasta raiz relativa **home**, que é a placa absoluta **~/..../sfs-home**, sendo "...." o local onde foi clonado este projeto.

## Funcionalidades
 - Estabelece uma conexão via socket, sendo possível acessar os sistema de arquivos unix por meio do terminal. 
 - Cada conexão é gerenciada por uma thread e possui um endereço relativo fictício no sistema de arquivos do SO.
 - Operações de remoção e leitura de arquivos são encapsuladas por uma classe genérica de mutex para proteger e evitar condições de corrida.
 - Nas operações de escrita e leitura foram colocados timers para ser possível a visualização do bloqueio do recurso pelo mutex da thread.

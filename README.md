
# SFS - Simple file system

Desenvolvido para a matéria de Arquitetura de Sistemas Operacionais - UFSC

## Pré-requisitos
- g++
- c++14
- Biblioteca pthread incluida no SO (Já incluída nas flags do computador no MakeFile)
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

## Como testar
Não é necessário a criação de um cliente. É possível usar utilitários para escrita e leitura em um socket como por exemplo netcat ou socat

#### Usando netcat
Com o [netcat](http://netcat.sourceforge.net/download.php) instalado, execute em um terminal paralelo usando o argumento -U para unix domain socket
```bash
nc -U /tmp/DefaultConnection.sock
```
#### Usando socat
Com o [socat](http://www.dest-unreach.org/socat/) instalado, execute em um terminal paralelo
```bash
socat - UNIX-CONNECT:/tmp/DefaultConnection.sock
```

## Comandos no cliente
Comandos marcados com **\*** são obrigatórios e podem ser usados com um caminho que antecede seu segundo argumento. Caso não sejam supridos, o comando não será executado. 

| Comando | Ação  |
|---|---|
| vdir path   |  Visualiza diretório path, caso path seja vazio, visualize diretório atual |
| edir \*path  |  Entra em no diretório path |
| cdir \*newdir  |  Cria um diretório chamado newdir |
| rdir \*dir  |  Remove um diretório dir, caso esteja vazio |
| cfile \*newfile  |  Cria um arquivo chamado newfile |
|  rfile \*file |  Remove um arquivo file |
|  vfile \*file |  Visualiza o texto de um arquivo file |
| efile \*file \*text  |  Escreve uma sequencia de caracteres text em um arquivo file |


## Resultados
Após um acesso, o cliente receberá um pre-fixo no input do terminal, sendo **X** o número da sessão, como mostrado abaixo.
```bash
sfs@instance-X:/home/$ 
```
Já no servidor, será exibido uma mensagem do tipo Success com o identificador da thread atrelada a conexão, assim como o descriptor do socket. Note, o descriptor do socket também é o identificador da instância mencionada acima, logo, poderá ser usado para visualizar mensagens futuras.
```bash
[Success] Thread 140068240434944 criada com sucesso!
[Success] Nova conexão criada - Socket Descriptor: 5
```

#### Ao executar qualquer comando
O resultado do comando aparecerá no cliente, caso seja um comando de visualização de dado (vdir, vfile).
Já no lado do servidor, será possível identificar o solicitante, e também, o comando solicitado, numa mensagem na tela do tipo Common.
```bash
[Common] Descriptor 5: vdir
```

#### Comandos que alocam recursos
Uma classe genérica mutex sempre encapsula um recurso, que para esse caso, sempre será um arquivo.
Quando uma conexão, gerida por uma thread, dispara uma solicitação de leitura de arquivo, por exemplo, o recurso será bloqueado até o desuso.

```bash
[Common] Descriptor 5: vfile test
[Resource] Recurso e5b503a0 trancado pela thread 140068240434944
[Common] Abrindo arquivo ./test...
```
Qualquer outra conexão que tente acessar o recurso e5b503a0 mostrado acima, será bloqueada até liberação, como mostrado abaixo por uma mensagem do tipo Warning:
```bash
[Common] Descriptor 6: vfile test
[Warning] Recurso e5b503a0 já está trancado, aguardando liberação...
```
Após certo tempo..
```bash
[Resource] Recurso e5b503a0 destrancado pela thread 140068240434944
[Resource] Recurso e5b503a0 trancado pela thread 140068232042240
[Common] Abrindo arquivo ./test...
```
O recurso foi destrancado pela conexão anterior, e imediatamente trancado pela conexão que estava no aguardo, gerida pela thread 140068232042240.

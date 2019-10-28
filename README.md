# SFS
Simple File System - Arq. SO

 - Estabelece uma conexão via socket, sendo possível acessar os sistema de arquivos unix por meio do terminal. 
 - Cada conexão é gerenciada por uma thread e possui um endereço relativo fictício no sistema de arquivos do SO.
 - Operações de remoção e leitura de arquivos são encapsuladas por uma classe genérica de mutex para proteger e evitar condições de corrida.
 - Nas operações de escrita e leitura foram colocados timers para ser possível a visualização do bloqueio do recurso pelo mutex da thread.

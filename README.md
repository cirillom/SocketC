# Trabalho 1 (SSC0641)

O código consiste em um servidor e um cliente, cada um implementado em arquivos .c separados.

## Executando o Código

Para executar o servidor, utilize o seguinte comando:

```
make runserver
```

Para executar o cliente, utilize o seguinte comando:

```
make runclient
```

Certifique-se de executar o servidor primeiro. Você deverá definir a porta do servidor quando o servidor for iniciado. Em seguida, execute o cliente e defina a porta do servidor ao qual o cliente se conectará. Além disso, insira o nome do cliente, que será exibido aos outros clientes quando você enviar uma mensagem. Lembre-se de que você receberá as mensagens de todos os clientes conectados ao mesmo servidor que você.

## Detalhes dos Códigos

### Servidor

O código do servidor implementa um mecanismo de troca de mensagens entre clientes conectados. Ele utiliza soquetes (sockets) para permitir a comunicação entre clientes. O servidor é implementado em linguagem C e usa a biblioteca de soquetes.

O servidor cria um soquete (socket) TCP para aceitar conexões de clientes. A escolha do TCP é apropriada para aplicativos que requerem uma comunicação confiável, pois o TCP garante a entrega das mensagens na ordem correta e lida com retransmissões em caso de perda de pacotes.

O código do servidor trata várias funcionalidades, incluindo a aceitação de conexões de clientes, o gerenciamento de várias conexões em threads separadas, o recebimento de mensagens de clientes e o reenvio dessas mensagens para todos os outros clientes.

Principais verificações de falha incluem:

- Verificação da criação do soquete do servidor.
- Verificação da vinculação do soquete à porta.
- Verificação da aceitação de novas conexões.
- Tratamento de erros ao receber e enviar mensagens.

### Cliente

O código do cliente é responsável por se conectar ao servidor e permitir que os usuários enviem e recebam mensagens. Assim como o servidor, o cliente é implementado em C.

O cliente cria um soquete (socket) TCP para se conectar ao servidor. Ele aceita a entrada do nome do cliente, que será usado para identificar o cliente quando ele enviar mensagens para o servidor.

Principais verificações de falha no cliente incluem:

- Verificação da criação do soquete do cliente.
- Verificação da conexão bem-sucedida ao servidor.
- Tratamento de erros ao enviar e receber mensagens.

### Justificativa para o Uso de TCP

A escolha de usar TCP é apropriada para esse tipo de aplicação, pois fornece uma comunicação confiável e ordenada. Os aplicativos de bate-papo, nos quais os clientes enviam mensagens que precisam ser entregues na ordem correta e não podem ser perdidas, se beneficiam do TCP. O TCP lida com a garantia de que as mensagens são entregues e retransmite automaticamente as mensagens em caso de perda de pacotes, garantindo uma comunicação estável e confiável.

## Informações de Ambiente de Desenvolvimento

### Versões de Sistema Operacional Usadas para Teste:

- macOS 13.6
- Fedora 38

### IDE Utilizada:

- VSCode 1.83.1

### Compilador Utilizado:

- GCC 13.2.0
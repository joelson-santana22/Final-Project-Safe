______________________________________________________EMBARCATECH_____________________________________________

UNIDADE 7 | CAPÍTULO 1 | PROJETO FINAL: Simulação de um cofre mecânico na placa de desenvolvimento BitDogLab

______________________________________________________________________________________________________________

## Aluno (matrícula) => Joelson Santana Alves (tic370101150)

___________________________________________
## Breve descrição
Para consolidar a compreensão dos principais conceitos relacionados a capacitção do curso em Sistemas Embarcados, com uso das funcionalidades da placa de desenvolvimento BitDogLab, desenvolveu-se com êxito o projeto que simula o funcionamento de um Cofre Mecânico. O projeto utiliza interrupções e estruturas condicionasis para controlar a entrada de uma senha por meio de botões e exiber feedback visual na matriz de LEDs WS2812.

___________________________________________
## Funcionalidades Implementadas ao Projeto

1. Matriz de LEDs WS2812:
   - Exibe números entre 0 a 9 para representar a entrada da senha.
   - Mostra uma seta indicativa para esquerda (onde fica o botão A) antes do primeiro clique no botão A.
   - Após o primeiro clique, exibe o dígito que se inicia em 1.

2. Botão A (Incremento):
   - Quando pressionado, incrementa o dígito atual em 1 (de 0 a 9). 
   - Inicia em 1, após cada clique no botão A soma-se 1 de modo a seguir a sequência: 1234657890 que retornar a 1.

3. Botão B (Confirmação):
   - Quando pressionado, confirma o dígito atual e armazena-o para gerar o vetor da senha de 6 digitos informada.
   - Após a entrada de 6 dígitos (6 cliques no botão B), verifica se a senha está correta.

4. Feedback Visual:
   - Se a senha estiver correta, exibe um padrão "V" (verde) na matriz de LEDs.
   - Se a senha estiver incorreta, exibe um padrão "X" (vermelho) na matriz de LEDs.
   - Após 3 segundos, o sistema reinicia e exibe a seta indicativa novamente.

5. Brilho dos LEDs:
   - O brilho dos LEDs foi ajustado para 70% para melhor visibilidade e economia de energia.

___________________________________________
## Instruções

1 - Faça o download dos arquivos do projeto clonando o repositório com o comando
| git clone <link do repositório>
ou baixe o arquivo .zip seguindo as opções "CODE > Download ZIP";

2 - No VSCode, utilizando a extensão do Raspberry Pi Pico, selecione "Import Project" e clique em "Change" para selecionar a pasta do projeto extraída caso seja o .zip. 

3 - Após aberto no VSCode, compile o código-fonte;

4 - Para execução na prática do funciomento do LED com PWM, conecte a placa BitDogLab à porta USB do seu computador/notebook. No VSCode, selecione a opção Run no menu/barra inferior do editor.

5- Se atente que a placa deverá está em modo BOOTSEL antes de selecionar o Run. A ativação do modo BOOTSEL pode ser feita de suas maneiras, sendo-as 
- No momento da coneção do cabo USB da placa ao notebook, simultaneamente o botão presente no Raspberry na parte de baixo pode ser pressionado para ativar o modo BOOTSEL;
- Pressionar esse botão citado anteriormente e simultaneamente clicar em um outro botão que fica na parte de cima da placa que tem o nome de Reset. Após o clique, ambos podem ser "soltados". 
___________________________________________
## Requisitos

### Hardware
- Raspberry Pi Pico;
- Placa BitDogLab;
- Matriz de LEDs WS2812 (5x5), conectada à GPIO 7;
- Botão A conectado à GPIO 5;
- Botão B conectado à GPIO 6.

### Software
- SDK do Raspberry Pi Pico configurado;
- Compilador C compatível (como GCC);
- Ferramentas para desenvolvimento do código para o Pico (VSCode por meio da biblioteca do Raspberry Pi Pico).

___________________________________________
## Como Funciona?

1. Inicialização:
   - Ao ligar a placa, a matriz de LEDs exibe uma seta indicando que o sistema está aguardando o primeiro clique no botão A.

2. Entrada da Senha:
   - O botão A incrementa o dígito atual (0 a 9), e o botão B confirma o dígito, armazena-os a cada clique até atingir a quantidade de 6 digitos.
   - Após 6 dígitos, o sistema verifica se a senha está correta.

3. Feedback:
   - Se a senha estiver correta, a matriz exibe um "V" verde.
   - Se a senha estiver incorreta, a matriz exibe um "X" vermelho.
   - Após 3 segundos, o sistema reinicia e exibe a seta novamente.

___________________________________________
## Considerações Finais

Este projeto demonstra o uso eficiente de conceitos práticos voltados ao uso da placa de desenvolvimento BitDogLab, como interrupções para controlar entradas de botões e fornecer feedback visual em tempo real. A combinação de hardware (BitDogLab e matriz de LEDs WS2812) e software (RP2040 e SDK) permite a criação de sistemas interativos e intuitivos, como a simulação de um cofre mecânico.

___________________________________________
## Link de Demonstração: 

## Link do Repositório com o Projeto:

___________________________________________

>> Desenvolvido por: Joelson Santana Alves 
>> Contato: 77992131954
>> Data de Conclusão: 25 de fevereiro de 2025.

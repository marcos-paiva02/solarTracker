<p align="center">
<img src="https://github.com/marcos-paiva02/solarTracker/blob/main/src/img/cabecalho.png"/>
</p>

Este Projeto foi criado para o TCC do ano 2022 na disciplina de Engenharia Elétrica da universidade UNIP.

## DESCRITIVO

  O intuito desse projeto é a criação de um sistema foto-voltaico muito mais eficiente e inteligente, pois a mecânica seguirá onde tiver a maior incidência de luz o que por consequência aumenta aeficiência energética, e também contém um monitoramente online para comparativo das medidas vindas do sitema quando está com o rastreador ligado e quando está desligado (placa foto-voltaica estática), dessa maneira é possível provar o aumento da eficiência, além de se obter um sistema inteligente conectado à internet.
  
  http://solartrackertcc.herokuapp.com/

  O sistema responsável por fazer todo o gerenciamento dos dados é o Firebase, que é uma plataforma apoiada pela Google, própria para desenvolvimento de aplicações, ou seja, todo o Backend, tal como autenticação e gerenciamento de rotas e comunicação com o Server, é criado automaticamente pela plataforma, criando um ambiente seguro para os dados e muito versátil. Foi utilizado o módulo Real-time Database, em que é possível a criação de objetos em JSON para base de dados. Abaixo segue uma abstração do sistema:
  
<p align="center">
<img src="https://github.com/marcos-paiva02/solarTracker/blob/main/src/img/abstracao.png"/>
</p>

De princípio para o funcionamento de todo o sistema foi idealizado o microcontrolador ESP32, devido sua grande quantidade de portas analógicas, porém quando a antena Wifi do dispositivo é utilizada o ADC2 e algumas portas do ADC1, o que impossibilitou a utilização, pois para o projeto, foram necessárias a utilização de 6 portas analógicas, para os sensores de luminosidade, corrente e tensão, por tanto foi utilizado também um arduino Mega 2560, então o ESP32 ficou responsável por fazer a conexão com o Firebase lendo e escrevendo dados e através do RX, TX envia ao Arduíno que processa e toma as ações para o movimento do seistema mecânico.

### `ESQUEMÁTICO:`

<p align="center">
<img src="https://github.com/marcos-paiva02/solarTracker/blob/main/src/img/SCHEMATIC.png"/>
</p>

### `Funcionamento do Algorítimo da mecânica:`

Conforme pode ser visto no código do arduíno, para que os mototes de passo "sigam" a luz foi utilizado um método de comparação, criando duas funções para isso, uma faz a comparação entre sensores da direita e da esquerda e a outra a comparação entre os sensores de cima e de baixo, dependendo do resultado da subtração das somas e sistema se move. Um ponto muito importante para um movimento suave e preciso, é a determinação de um coeficiente para esse comparativo, o que delimita quando que pode-se mover ou não, foi criado uma constante para cada função, pois foi observado que são necessários valores diferentes.

Para driver dos motores de passo foi utlizado o módulo A4988, para a leitura da tensão um módulo sensor de tensão, muito fácil de ser encontrado na internet, que nada mais é que um divisor de tensão. E para a leitura da corrente foi utilizado o módulo ACS712.


### `APP para Android:`

Para implementar ainda mais o projeto, foi criado um App para Android, em que é possível ter a mesma análise de eficiência do web site, porém por lá, é possível controlar os motores manualmente quando o rastreador estiver desligado e também é possível fazer a limpeza do banco de dados para criação de novas comparações. Esse App foi criado pela plataforma gratuita Kodular, que utiliza uma programação por blocos.

<p align="center">
<img src="https://github.com/marcos-paiva02/solarTracker/blob/main/src/img/app.png"/>
</p>

O resultado das telas do App:

<p align="center">
<img src="https://github.com/marcos-paiva02/solarTracker/blob/main/src/img/app2.png"/>
</p>



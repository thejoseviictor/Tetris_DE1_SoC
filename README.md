<h2>Universidade Estadual de Feira de Santana<br>  
  
<br>Discentes:  
<uL> 
  <li><a href="https://github.com/thejoseviictor">José Victor de Oliveira Correia</a></li>
  <li><a href="https://github.com/Macedoxx2000">João Victor Macedo dos Santos Lima </a></li>
  <li><a href="https://github.com/MateusAntony/MateusAntony">Mateus Antony Medeiros Carvalho </a></li>
  <li><a href="https://github.com/WesleiSantos13">Weslei Silva Santos</a></li>
</ul>

<br>Turma: 3;  
<br>Grupo: 3;  
<br>Componente: TEC499 - MI-Sistemas digitais;  
<br>Docente: Wild Freitas da Silva;  
<br>Período: 2024.2.  
</h2>   


<h1 align="center">Projeto 1: Jogo Tetris .</h1>  
<h2 align="center">Game inspirado no clássico Tetris, desenvolvido para o kit de desenvolvimento DE1-SoC utilizando linguagem C </h2>  



<h2 align="justify">Introdução</h2>

  <div align="justify">
  
  No mundo da computação, seja ele dividido em software ou hardware, muitas pessoas se utilizam e aproveitam das quase infinitas possibilidades de conhecimentos e          ferramentas para serem utilizadas no dia a dia de forma produtiva e eficiente para as mais diversas funcionalidades e objetivos.  

  Uma das vertentes é a área de jogos, que hoje é considerada uma gigantesca parte do entretenimento e mercado global (que faturou cerca de $187,7 bilhões em 2023) . A     cada ano que passa, o mundo dos games se torna mais avançado, em quesitos de memória, gráfico, comunicação online e armazenamento em nuvem.  

  Entretanto, a nossa inspiração para o projeto remota ao ano de 1984, quando o pesquisador soviético Alexey Pajitnov desenvolveu o seu primeiro modelo do Tetris.  

  </div>

<h2 align="justify">Descrição do projeto</h2>

  <div align="justify">
  
  O projeto PBL1 teve como objetivo a utilização dos conceitos aprendidos em arquitetura de computadores e sistemas digitais, de forma prática, para a recriação do  jogo   tetris, atendendo os seguintes requisitos mínimos:   

  * Utilizar o Kit de desenvolvimento DE1-SoC e os seus respectivos componentes;  
  * O código deve ser escrito em linguagem C;  
  * Não é permitido o uso de bibliotecas para o acelerômetro;  
  * O jogo deve pontuar e eliminar agrupamentos.  

  </div>  

<h2>Descrição de softwares, equipamentos e ferramentas utilizadas: </h2>  
<div align="justify">  

  <h3> - Kit de desenvolvimento DE1-SoC: </h3>  
  <div align="justify">  

  Texto

  <p align="center">
  <img src="Images/DE1-SoC Layout.webp" width = "500" />
  </p>
  <p align="center"><strong>Kit de desenvolvimento DE1-SoC </strong></p>

  </div>

  <h3> - Linguagem C: </h3>  
  <div align="justify">  

  Texto

  </div>  

  <h3> - Monitor CTR DELL E773c: </h3>  
  <div align="justify">  

  O monitor utilizado no desenvolvimento do projeto é o "CTR DELL E773c" que utiliza a tecnologia de tubo de raios catódicos para projeção imagens. Possui uma tela de 16 polegadas, resolução máxima de 1280 x 1024 pixels, entrada VGA e com frequência horizontal de até 70 kHz e taxa de atualização vertical de até 160 Hz​. Proporciona um bom contraste e brilho em comparação com outros monitores CRT da período de fabricação.

  <p align="center">
  <img src="Images/Monitor CTR Dell E773c.jpg" width = "400" />
  </p>
  <p align="center"><strong>Monitor CRT DELL E733c</strong></p>

  </div>  
  
  <h3> - Visual Studio Code: </h3>  
  <div align="justify">  

  O Visual Studio Code (VS Code) é um editor de código-fonte multiplataforma disponível para Windows, macOS e Linux e desenvolvido pela Microsoft. Oferece suporte nativo para diversas linguagens de programação, como JavaScript, TypeScript, Python, C, C++, Java, PHP, Ruby e permite a instalação de extensões para suportar mais linguagens. Além disso permite sugestões de código, documentação e programação colaborativa em tempo real, e grande capacidade de personalização para atender as preferências do programador.

  <p align="center">
  <img src="Images/Visual Code Studio editor.png" width = "500" />
  </p>
  <p align="center"><strong></strong></p>

  <div align="center">
  <a href="https://code.visualstudio.com/">Visual Code Studio</a>
  </div>

  </div>  

  <h3> - Compilador GNU: </h3>  
  <div align="justify"> 

  Texto

  </div> 

  
</div>

<h2>Lógica de jogo</h2>  
  <div align="justify">

  <h3>- Parte visual e gráfica:</h2>  
  <div align="justify">  

  Texto

  </div>  

  <h3>- Movimentação de peças: </h2>
  <div align="justify">

  A lógica de movimentação das peças utiliza os sensores de força G do acelerômetro ADXL345, presente no FPGA DE1-SoC Cyclone V.<br>

  O ADXL345 possui três eixo (X, Y, Z) com alta resolução de medição, porém apenas o eixo X foi usado, permitindo a movimentação das peças no sentido horizontal.<br>

  A comunicação entre o acelerômetro e o Cyclone V é feita através do serial I2C0, usando o HPS.<br>

  Para conectar o I2C0 ao ADXL345 é preciso modificar as entradas do bloco de multiplexadores (Pin Mux). Definindo a entrada "1" nos registradores "GENERALIO7/GENERALIO8" e "0" no "I2C0USEFPGA".<br>

  Antes de instanciar as funções do acelerômetro, é preciso mapear os endereços físicos base do "I2C0" e "SYSMGR" em endereços virtuais. E, em seguida, mapear os registradores restantes através da soma do endereço virtual base com o seu offset.<br>

  Do mesmo modo, a lógica de movimentação conta com as seguintes funções:<br>

  <strong> int open_memory(void);</strong><br>
  Acessa a memória física do sistema linux, através do diretório "/dev/mem", e retorna "-1", caso ocorra um erro na abertura do arquivo de memória<br>

  <strong> void enable_I2C0_HPS(int);</strong><br>
  Recebe como entrada o inteiro usando na função "open_memory".<br>
  Habilita a comunicação do ADXL345 com o I2C0, através do bloco de multiplexadores no módulo "sysmgr", usando os pinos HPS.<br>

  <strong>void setting_I2C0_ADXL345(void *);</strong><br>
  Recebe como entrada o endereço virtual base do "I2C0".<br>
  Inicia a comunicação I2C0 e seleciona o "ADXL345" como slave/target.<br>

  <strong>void setting_ADXL345(volatile unsigned int *);</strong><br>
  Recebe como entrada o endereço do registrador "I2C0_DATA".<br>
  Realiza as calibragens e configurações iniciais dos sensores do ADXL345, com base nas calibragens do "FPGA Academy".<br>

  <strong>int16_t lerEixoX(volatile unsigned int *, volatile unsigned int *);</strong><br>
  Recebe como entradas os endereços dos registradores "I2C0_DATA" e "I2C0_RXFLR".<br>
  Lê e retorna os valores de medição do eixo X em um inteiro de 16 bits com sinal.<br>

  Diante do exposto, a função principal faz a leitura do valor do eixo x, para valores maiores do que "+100g" a peça se moverá para a direita, para valores menores do que "-100g" a peça se moverá para a esquerda.
</div>

</div>

<h2>Conclusões</h2>
<div align="justify">

Texto

</div>

<h2>Referências</h2>
<div align="justify">

https://newzoo.com/resources/blog/explore-the-global-games-market-in-2023<br>
Cyclone V HPS Register Address Map and Definitions: https://www.intel.com/content/www/us/en/programmable/hps/cyclone-v/hps.html<br>
Lab 7 - Using the ADXL345 Accelerometer: https://fpgacademy.org/courses.html

</div>

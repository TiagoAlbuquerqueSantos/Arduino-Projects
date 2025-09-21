/* =============================================================================================================
    
   
   WR Kits Channel & Usina Info número 026


  Controle de Servo com Mostrador de Ângulo  
    
    
   Autor: Eng. Wagner Rambo  Data: Março de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits
   
   
============================================================================================================= */


// =============================================================================================================
// --- Bibliotecas Auxiliares ---
#include     <Servo.h>             //biblioteca para controle de servos


// =============================================================================================================
// --- Mapeamento de Hardware ---
#define       buzzer    3           //Buzzer (ativo em LOW)
#define       servo1    5           //Servo motor
#define       store     7           //Registrador de armazenamento (shift register)
#define       data      8           //Entrada de dados (shift register)
#define       shift     4           //Registrador de deslocamento (shift register)
#define       menos    15           //botão de decremento
#define       enter    16           //botão de acionamento
#define       mais     17           //botão de incremento


// =============================================================================================================
// --- Protótipo das Funções ---  
void my_register(byte value);            //Função para controle dos registradores de deslocamento
byte display_seg(byte num, boolean dp);  //Função para controle do display
void readButtoms();                      //Função para leitura dos botões


// =============================================================================================================
// --- Declaração de Objetos --- 
Servo sv1;


// =============================================================================================================
// --- Variáveis Globais ---  
byte counter, control_U2;                      //bytes de controle dos registradores de deslocamento
 
short control = 1;                             //Variável de controle (para saber qual display está ativo)
      
int mil, cen, dez, uni;                        //Variáveis auxiliares para dados nos displays

boolean  dig_mil = 0x00,                       //flag do display dos milhares
         dig_cen,                              //flag do display das centenas
         dig_dez,                              //flag do display das dezenas
         dig_uni,                              //flag do display das unidades
         f_menos = 0x00,                       //flag aux para botão de decremento
         f_enter = 0x00,                       //flag aux para botão de acionamento
         f_mais  = 0x00;                       //flag aux para botão de incremento

int disp_number = 0;                           //valor a ser exibido no display

int      angle     = 90,                       //angulo do servo motor
         new_angle = 90;                       //novo angulo


//========================================================================================//
// --- Rotina de Interrupção ---
ISR(TIMER2_OVF_vect)      // Trata Overflow do Timer2
{
    TCNT2 = 196;          // Reinicializa o registrador do Timer2
    
    
    if(!dig_mil && control == 1)                   //Dígito dos milhares desligado?
    {                                              //Variável de controle igual a 1?
        control = 0x02;                            //Sim, control recebe o valor 2
        dig_uni = 0x00;                            //Apaga o dígito das unidades
        dig_dez = 0x00;                            //Apaga o dígito das dezenas
        dig_cen = 0x00;                            //Apaga o dígito das centenas
        control_U2 = 0x80;                         //Carrega valor para o registrador U2 (dig_1)
        mil     = disp_number/1000;                //Calcula o dígito dos milhares
        dig_mil = 0x01;                            //Ativa dígito dos milhares
        counter = display_seg(mil, 0);             //Recebe o valor retornado do display

        digitalWrite(shift, LOW);                  //Atualiza display
        my_register(counter);
        my_register(control_U2);
        digitalWrite(shift, HIGH);
       
        
    } //end if dig_mil

    else if(!dig_cen && control == 2)              //Dígito das centenas desligado?
    {                                              //Variável de controle igual a 2?
        control = 0x03;                            //Sim, control recebe o valor 3
        dig_uni = 0x00;                            //Apaga o dígito das unidades
        dig_dez = 0x00;                            //Apaga o dígito das dezenas
        dig_mil = 0x00;                            //Apaga o dígito dos milhares
        control_U2 = 0x40;                         //Carrega valor para o registrador U2 (dig_2)
        cen     = (disp_number%1000)/100;          //Calcula o dígito das centenas
        dig_cen = 0x01;                            //Ativa dígito das centenas
        counter = display_seg(cen, 0);             //Recebe o valor retornado do display

        digitalWrite(shift, LOW);                  //Atualiza display
        my_register(counter);
        my_register(control_U2);
        digitalWrite(shift, HIGH);
        
        
    } //end if dig_cen
        
    else if(!dig_dez && control == 3)              //Dígito das dezenas desligado?
    {                                              //Variável de controle igual a 3?
        control = 0x04;                            //Sim, control recebe o valor 4
        dig_uni = 0x00;                            //Apaga o dígito das unidades
        dig_mil = 0x00;                            //Apaga o dígito dos milhares
        dig_cen = 0x00;                            //Apaga o dígito das centenas
        control_U2 = 0x20;                         //Carrega valor para o registrador U2 (dig_3)
        dez     = (disp_number%100)/10;            //Calcula o dígito das dezenas
        dig_dez = 0x01;                            //Ativa dígito das dezenas
        counter = display_seg(dez, 0);             //Recebe o valor retornado do display

        digitalWrite(shift, LOW);                  //Atualiza display
        my_register(counter);
        my_register(control_U2);
        digitalWrite(shift, HIGH);
       
        
    } //end if dig_dez
        
    else if(!dig_uni && control == 4)              //Dígito das unidades desligado?
    {                                              //Variável de controle igual a 4?
        control = 0x01;                            //Sim, control recebe o valor 1
        dig_mil = 0x00;                            //Apaga o dígito dos milhares
        dig_dez = 0x00;                            //Apaga o dígito das dezenas
        dig_cen = 0x00;                            //Apaga o dígito das centenas
        control_U2 = 0x10;                         //Carrega valor para o registrador U2 (dig_4)
        uni     = disp_number%10;                  //Calcula o dígito das unidades
        dig_uni = 0x01;                            //Ativa dígito das unidades
        counter = display_seg(uni, 0);             //Recebe o valor retornado do display

        digitalWrite(shift, LOW);                  //Atualiza display
        my_register(counter);
        my_register(control_U2);
        digitalWrite(shift, HIGH);
       
        
    } //end if dig_uni
    
     
} //end Timer2 OVF


// =============================================================================================================
// --- Configurações Iniciais ---  
void setup()
{
      //Saídas digitais
      pinMode(buzzer, OUTPUT);
      pinMode(servo1, OUTPUT);
      pinMode(store,  OUTPUT);
      pinMode(data,   OUTPUT);
      pinMode(shift,  OUTPUT);
   
      //Entradas digitais
      pinMode(menos,  INPUT);
      pinMode(enter,  INPUT);
      pinMode(mais,   INPUT);


      sv1.attach(servo1); //conexão do servo1
      digitalWrite(buzzer, HIGH); //buzzer inicia desligado

      sv1.write(angle);  //inicializa servo1


      // -- Configuração do Estouro do Timer 2 --
     TCCR2A = 0x00;   //Timer operando em modo normal
     TCCR2B = 0x07;   //Prescaler 1:1024
     TCNT2  = 196;    //Inicia conteúdo do Timer2 em 196d
     TIMSK2 = 0x01;   //Habilita interrupção do Timer2
      
 
} //end setup


// =============================================================================================================
// --- Variáveis Globais ---  
void loop()
{
  
    
    readButtoms();           //lê botões

    disp_number = new_angle; //mostra ângulo ajustado

    sv1.write(angle);        //posiciona servo de acordo com ângulo ajustado

    delay(30);              //aguarda 30ms
 
  
} //end setup


// =============================================================================================================
// --- Desenvolvimento das Funções ---  


// =============================================================================================================
// --- Controle dos Registradores de Deslocamento ---  
void my_register(byte value)
{
   
   boolean control;
  
   digitalWrite(store, LOW); //deixa o registrador pronto para receber dados
   digitalWrite(data,  LOW);
   
   for(int i = 0; i < 8; i++) //deslocamento dos dados
   {
   
     digitalWrite(store, LOW);
     
     if(value & (1<<i))  // testa se há dados para atualizar
     {
       control = HIGH;
     
     }
     else               // Se não houver nenhum dado...
     {
       control = LOW;
     }
     
     digitalWrite(data, control);  // Habilita a saída de dados conforme condições acima
     digitalWrite(store, HIGH);    // Armazenar os 8 dados enviados
     
  
   } //end for

     digitalWrite(store, LOW);     // Comando para garantir atualização da saída

} //end my_register


// =============================================================================================================
// --- Controle do Display ---  
byte display_seg(byte num, boolean dp)
{
    byte anode;                                //armazena código BCD

    //-- Vetor para o código BCD --
    byte SEGMENTO[10]= {0x03,                  //BCD zero   '0'
                        0x9F,                  //BCD um     '1'
                        0x25,                  //BCD dois   '2'
                        0x0D,                  //BCD três   '3'
                        0x99,                  //BCD quatro '4'
                        0x49,                  //BCD cinco  '5'
                        0x41,                  //BCD seis   '6'
                        0x1F,                  //BCD sete   '7'
                        0x01,                  //BCD oito   '8'
                        0x19                   //BCD nove   '9'
                       };                

    if(!dp) anode = SEGMENTO[num];             //se dp igual a zero, retorna sem ponto decimal
    else    anode = SEGMENTO[num] - 1;         //senão, retorna com ponto decimal

    return(anode);                             //retorna o número BCD

} //end display


// =============================================================================================================
// --- Leitura dos Botões ---
void readButtoms()
{
  
    if(!digitalRead(menos))           //botão menos pressionado?
    {                                 //sim
      digitalWrite(buzzer, LOW);      //aciona buzzer
      f_menos = 0x01;                 //seta flag
      
    } //end menos pressionado

    if(!digitalRead(enter))           //botão enter pressionado?
    {                                 //sim
      digitalWrite(buzzer, LOW);      //aciona buzzer
      f_enter = 0x01;                 //seta flag
      
    } //end enter pressionado

    if(!digitalRead(mais))            //botão mais pressionado?
    {                                 //sim
      digitalWrite(buzzer, LOW);      //aciona buzzer
      f_mais = 0x01;                  //seta flag
      
    } //end mais pressionado


    if(digitalRead(menos) && f_menos)  //botão menos solto e flag setada?
    {                                  //sim
      digitalWrite(buzzer, HIGH);      //desliga buzzer
      f_menos = 0x00;                  //limpa flag
      new_angle = new_angle - 5;       //decrementa o ângulo de 5 em 5

      if(new_angle < 0) new_angle = 180;  //menor que zero? volta a ser 180
     
    } //end menos solto

    if(digitalRead(enter) && f_enter)  //botão enter solto e flag setada?
    {                                  //sim
      digitalWrite(buzzer, HIGH);      //desliga buzzer
      f_enter = 0x00;                  //limpa flag
      angle = new_angle;               //atualiza angulo
     
    } //end enter solto

    if(digitalRead(mais)  &&  f_mais)  //botão mais solto e flag setada?
    {                                  //sim
      digitalWrite(buzzer, HIGH);      //desliga buzzer
      f_mais = 0x00;                   //limpa flag
      new_angle = new_angle + 5;       //incrementa o ângulo de 5 em 5
      
      if(new_angle > 180) new_angle = 0;  //maior que 180? volta a ser zero
     
    } //end mais solto
  
  
  
  
} //end readButtoms





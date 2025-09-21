/*
Vídeo #94
Projeto: shield multifunção com display
By: Mamute Eletrônica
Data do projeto: 23/03/2021
Atualizado: 28/08/2023
Engenheiro: Vander Gonçalves
Site: www.mamuteeletronica.com.br
*/

//==========================================================================
//=================> Declarações
//=================> ctrl + u = verifica o código
//=================> não contém variável

//#include <TimerOne.h>                     // Bibliotec TimerOne 
#include <Wire.h>                         // Biblioteca Wire 
#include <MultiFuncShield.h>              // Biblioteca Multifunction shield

//==========================================================================
//=================> Início do setup
//================= ctrl + u = verifica o código
void setup()
{
  pinMode(3, OUTPUT);
 // Timer1.initialize();                  // inicializa o Timer 1
  MFS.initialize();                // initializa a biblioteca Multi função
  Buzzer();                          // toca a campainha 
}//fim_setup

//==========================================================================
//=================> Início da Função
//================= ctrl + u = verifica o código

void Buzzer()
{
  MFS.beep();                             // Bip curto de 300 milisegundos
  delay(1000);                            // atraso de 1 segundo
                                         // 4 bips curtos , repetidos 3 vezes
  MFS.beep(5, // bip por 50 milisegundos
           5, // silencio por 50 milisegundos
           4, // repete o bip 4 vezes
           3, // repete o loop 3 vezes
           50 // aguarda 500 milisegundos entre os loops
          );
}

//==========================================================================
//=================> Início do loop
//================= ctrl + u = verifica o código

void loop()
{

//Grilo
digitalWrite(3,1);
delay(1200);
digitalWrite(3,0);
delay(250);

}

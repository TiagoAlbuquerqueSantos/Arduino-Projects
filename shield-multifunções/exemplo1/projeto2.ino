#include <TimerOne.h>                     // Bibliotec TimerOne 
#include <Wire.h>                         // Biblioteca Wire 
#include <MultiFuncShield.h>              // Biblioteca Multifunction shield
void setup()
{
  Timer1.initialize();                    // inicializa o Timer 1
  MFS.initialize(&Timer1);                // initializa a biblioteca Multi função
  MFS.write("Hi");                        // escreve no display Hi
  delay(2000);                            // atraso de 2 segundos
  MFS.write(-273);                        // escreve no display - 273
  delay(2000);                            // atraso de 2 segundos
  MFS.write(3.141, 2);                    // escreve no display 3.141
  delay(2000);                            // atraso de 2 segundos
}
int counter = 0;                          // incializa o conatdor
byte ended = false;                       // define variavel ended
void loop()
{
  if (counter < 200)                      // se o contador for menor do que 200
  {
    MFS.write((int)counter);              // escreve no display a contagem
    counter++;                            // incrementa o contador 
  }
  else if (!ended)                        // se aconatgem terminou 
  {
    ended = true;            
    MFS.write("End");                     // escreve no display End
    MFS.blinkDisplay(DIGIT_ALL, ON);      // pisca a ultima mensagem
  }
  delay(50);                              // atraso de 50 milisegundos 
}

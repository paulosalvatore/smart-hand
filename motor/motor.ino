#include <Mindwave.h>
#include <SimpleTimer.h>
#include <Servo.h>

#define SERVO 6 // Porta Digital 6 PWM

Mindwave mindwave;

SimpleTimer simpleTimer;

Servo s;

int concentracaoNecessaria = 30;
int duracaoDelay = 1000;

int atencao = 0;
int posicaoAtual = 0;
int intervaloPosicao = 10;

bool motorLiberado = true;

int botao = 12;
bool botaoDesativado = false;

void setup() {
	pinMode(botao, INPUT);
	
	s.attach(SERVO);
	Serial.begin(9600);
  s.write(0); // Inicia motor posição zero
  Serial.begin(MINDWAVE_BAUDRATE);
}

void onMindwaveData()
{
  Serial.print("\tquality: ");
  Serial.print(mindwave.quality());
  Serial.print("\tattention: ");
  Serial.print(mindwave.attention());
  Serial.print("\tmeditation: ");
  Serial.print(mindwave.meditation());
  Serial.print("\tlast update: ");
  Serial.print(mindwave.lastUpdate());
  Serial.print("ms ago");
  Serial.println();
  
  atualizarMotor();
}

void liberarMovimentacaoMotor()
{
	Serial.println("liberarMovimentacaoMotor");
	motorLiberado = true;
}

void atualizarMotor()
{
  //atencao = mindwave.attention();
  
  if (atencao > concentracaoNecessaria)
  {
	// Avança uma posição

	alterarPosicaoServo(intervaloPosicao);
  }
  else
  {
	// Volta uma posição

	alterarPosicaoServo(intervaloPosicao * -1);
  }
}

void pressionarBotao()
{
	bool leituraBotao = digitalRead(botao);
	
	botaoDesativado = leituraBotao;
}

void alterarPosicaoServo(int intervaloReal)
{
  if (!motorLiberado || botaoDesativado)
	return;
  
  Serial.println("alterarPosicaoServo");
  
  posicaoAtual = constrain(posicaoAtual + intervaloReal, 0, 180);

  s.write(posicaoAtual);

  motorLiberado = false;
  
  simpleTimer.setTimeout(duracaoDelay, liberarMovimentacaoMotor);
}

void loop()
{
  simpleTimer.run();

  mindwave.update(Serial, onMindwaveData);
  
  if (Serial.available() > 0)
  {
	char valorRecebido = Serial.read();

	atencao = (valorRecebido == '1') ? 31 : 0;
  }
  
  pressionarBotao();

  //atualizarMotor();
}

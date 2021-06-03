
#define enA 10
#define in1 7
#define in2 6
#define enB 9
#define in3 5
#define in4 4

void setup()
{
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
}

void forward_5cm()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 60);
    analogWrite(enB, 60);

    delay(180);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

void ninety_degrees_right()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 80);
    analogWrite(enB, 80);

    delay(400);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}
void backward_5cm()
{
}

void ninety_degrees_left()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(enA, 80);
    analogWrite(enB, 80);

    delay(400);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

void loop()
{
    delay(5000);
    ninety_degrees_right();
    delay(100);
    ninety_degrees_left();
}

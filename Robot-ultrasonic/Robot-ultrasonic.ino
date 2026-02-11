#include <Servo.h>                                         // เรียกใช้ Library Servo 
#define PWM_ML 2                                         // กำหนดขา PWM_ML
#define in1_ML 3                                           // กำหนดขา in1_ML 
#define in2_ML 4                                           // กำหนดขา in2_ML 
#define in1_MR 5                                           // กำหนดขา in1_MR 
#define in2_MR 6                                           // กำหนดขา in2_MR 
#define PWM_MR 7                                        // กำหนดขา PWM_MR 

// ตัวแปรสำหรับเก็บองศาการหมุน Servo
int right = 0; 
int front = 90; 
int left = 180;

const int trigPin = 9;                                      // กำหนดขา trigPin 
const int echoPin = 10;                                 // กำหนดขา echoPin 
const int servoPin = 11;                                // กำหนดขา servoPin 

long duration;                                            // ตัวแปรเก็บค่าระยะเวลา
int distanceFront;                                       // ตัวแปรเก็บค่าระยะทางด้านหน้า
int distanceLeft;                                         // ตัวแปรเก็บค่าระยะทางด้านซ้าย
int distanceRight;                                       // ตัวแปรเก็บค่าระยะทางด้านขวา
int SpeedR = 255;                                      // ความเร็วของมอเตอร์ด้านขวา
int SpeedL = 255;                                      // ความเร็วของมอเตอร์ด้านซ้าย

Servo myServo;                                        // ออบเจ็กต์ Servo

int measureDistance();                              // ประกาศฟังก์ชันวัดระยะทาง

void setup() {
  pinMode(PWM_ML, OUTPUT);                 // ตั้งค่า PWM_ML เป็น OUTPUT
  pinMode(in1_ML, OUTPUT);                    // ตั้งค่า in1_ML เป็น OUTPUT
  pinMode(in2_ML, OUTPUT);                    // ตั้งค่า in2_ML เป็น OUTPUT
  pinMode(in1_MR, OUTPUT);                    // ตั้งค่า in1_MR เป็น OUTPUT
  pinMode(in2_MR, OUTPUT);                            // ตั้งค่า in2_MR เป็น OUTPUT
  pinMode(PWM_MR, OUTPUT);                         // ตั้งค่า PWM_MR เป็น OUTPUT
  Serial.begin(9600);                                         // เริ่มต้นการสื่อสารผ่าน Serial Monitor ด้วย Buad rate 9600
  pinMode(trigPin, OUTPUT);                             // ตั้งค่า trigPin เป็น OUTPUT
  pinMode(echoPin, INPUT);                             // ตั้งค่า echoPin เป็น INPUT
  myServo.attach(servoPin);                             // เชื่อมต่อเซอร์โวมอเตอร์กับพิน servoPin
  myServo.write(90);                                       // หมุนเซอร์โวมอเตอร์ไปที่ 90 องศา
  delay(3000);                                               // รอ 3 วินาทีเพื่อให้เซอร์โวมอเตอร์หมุนไปยังตำแหน่ง
}

void loop() {
  // วัดระยะทางด้านหน้า
  distanceFront = measureDistance();               // วัดระยะทางด้านหน้า
  Serial.print("Distance Front: ");                       // แสดงข้อความ "Distance Front:"
  Serial.print(distanceFront);                           // แสดงค่าระยะทางด้านหน้า
  Serial.println(" cm");                                    // แสดงหน่วย cm

  





  if (distanceFront > 20) {                             // ถ้าด้านหน้า มีค่ามากกว่า 20 หรือไม่มีสิ่งกีดขวง
    moveForward();                                      // เรียกใช้ฟังก์ชัน moveForward เพื่อเคลื่อนที่ไปข้างหน้า
  } else {                                                    // ถ้าด้านหน้ามีค่าน้อยกว่า 20 หรือมีสิ่งกีดขวาง
    stopMoving();                                         // เรียกใช้ฟังก์ชัน stopMoving เพื่อหยุดเคลื่อนที่
    checkLeft();                                             // เรียกใช้ฟังก์ชัน checkLeft เพื่อเช็คระยะด้านซ้าย
    checkRight();                                           // เรียกใช้ฟังก์ชัน checkRight เพื่อเช็คระยะด้านขวา
    // ตัดสินใจเลือกทิศทางที่จะเคลื่อนที่
    if (distanceLeft > distanceRight) {             // ถ้าระยะด้านซ้ายมากกว่าด้านขวา
      // เคลื่อนที่ไปทางซ้าย
      myServo.write(90);                              // หมุนเซอร์โวมอเตอร์ไปที่ 90 องศา เพื่อพร้อมที่วัดระยะด้านหน้า
      turnLeft();                                          // เรียกใช้ฟังก์ชัน turnLeft เพื่อเลี้ยวซ้าย
    } else {                                                // ถ้าไม่ใช่ หรือระยะด้านขวามากกว่าด้านซ้าย
      // เคลื่อนที่ไปทางขวา
      myServo.write(90);                             // หมุนเซอร์โวมอเตอร์ไปที่ 90 องศา เพื่อพร้อมที่วัดระยะด้านหน้า
      turnRight();                                       // เรียกใช้ฟังก์ชัน turnRight เพื่อเลี้ยวขวา
    }
  }
  delay(100);                                          // รอ 100 มิลลิวินาที ก่อนการเริ่มทำงานใหม่ครั้งต่อไป
}

// สร้างฟังก์ชันตรวจสอบระยะทางด้านซ้าย
void checkLeft() {
  myServo.write(0);                                  // หมุนเซอร์โวมอเตอร์ไปที่ 0 องศา หรือด้านซ้าย
  delay(1000);                                         // รอให้เซอร์โวมอเตอร์หมุนไปยังตำแหน่ง
  distanceLeft = measureDistance();          // วัดระยะทางด้านซ้ายและเก็บค่าไว้ในตัวแปร
  Serial.print("Distance Left: ");                  // แสดงข้อความ "Distance Left:"
  Serial.print(distanceLeft);                      // แสดงค่าระยะทางด้านซ้าย
  Serial.println(" cm");                             // แสดงหน่วย cm
}

// สร้างฟังก์ชันตรวจสอบระยะทางด้านขวา
void checkRight() {
  myServo.write(180);                           // หมุนเซอร์โวมอเตอร์ไปที่ 180 องศา หรือด้านขวา
  delay(1000);                                     // รอให้เซอร์โวมอเตอร์หมุนไปยังตำแหน่ง
  distanceRight = measureDistance();     // วัดระยะทางด้านขวาและเก็บไว้ในตัวแปร
  Serial.print("Distance Right: ");             // แสดงข้อความ "Distance Right:"
  Serial.print(distanceRight);                  // แสดงค่าระยะทางด้านขวา
  Serial.println(" cm");                          // แสดงหน่วย cm
}

// สร้างฟังก์ชันวัดระยะทาง
int measureDistance() {
  // เคลียร์พิน trigPin โดยตั้งค่าให้เป็น LOW
  digitalWrite(trigPin, LOW);                 // ตั้งค่า trigPin เป็น LOW
  delayMicroseconds(2);                     // รอ 2 ไมโครวินาที
// วัดระยะ
  digitalWrite(trigPin, HIGH);                 // ตั้งค่า trigPin เป็น HIGH
  delayMicroseconds(10);                   // รอ 10 ไมโครวินาที
  digitalWrite(trigPin, LOW);                // ตั้งค่า trigPin เป็น LOW
  // อ่านค่า echoPin และคำนวณระยะเวลา
  duration = pulseIn(echoPin, HIGH);  // อ่านค่าระยะเวลา echoPin เป็น HIGH

  // คำนวณระยะทาง (duration / 2) / 29.1
  int distance = duration * 0.034 / 2;                  // คำนวณระยะทางจากค่าระยะเวลา

  return distance;                                           // ส่งค่าระยะทางกลับไปที่ตัวแปร distance
}

// สร้างฟังก์ชันสำหรับการเคลื่อนที่ไปข้างหน้า
void moveForward() {
  digitalWrite(in1_ML, LOW);                            // ตั้งค่า in1_ML เป็น LOW
  digitalWrite(in2_ML, HIGH);                            // ตั้งค่า in2_ML เป็น HIGH
  analogWrite(PWM_ML, SpeedL);                    // ตั้งค่า PWM_ML ให้มอเตอร์ทำงานด้วยความเร็ว SpeedL

  digitalWrite(in1_MR, LOW);                            // ตั้งค่า in1_MR เป็น LOW
  digitalWrite(in2_MR, HIGH);                            // ตั้งค่า in2_MR เป็น HIGH
  analogWrite(PWM_MR, SpeedR);                    // ตั้งค่า PWM_MR ให้มอเตอร์ทำงานด้วยความเร็ว SpeedR
}


// สร้างฟังก์ชันสำหรับการหยุดเคลื่อนที่
void stopMoving() {
  digitalWrite(in1_ML, LOW);                           // ตั้งค่า in1_ML เป็น LOW
  digitalWrite(in2_ML, LOW);                           // ตั้งค่า in2_ML เป็น LOW
  analogWrite(PWM_ML, 0);                            // ตั้งค่า PWM_ML ให้มอเตอร์หยุดทำงาน

  digitalWrite(in1_MR, LOW);                           // ตั้งค่า in1_MR เป็น LOW
  digitalWrite(in2_MR, LOW);                           // ตั้งค่า in2_MR เป็น LOW
  analogWrite(PWM_MR, 0);                            // ตั้งค่า PWM_MR ให้มอเตอร์หยุดทำงาน
}

// สร้างฟังก์ชันสำหรับการเลี้ยวซ้าย
void turnLeft() {
  digitalWrite(in1_ML, HIGH);                           // ตั้งค่า in1_ML เป็น HIGH
  digitalWrite(in2_ML, HIGH);                           // ตั้งค่า in2_ML เป็น HIGH
  analogWrite(PWM_ML, 0);                            // ตั้งค่า PWM_ML ให้มอเตอร์หยุดทำงาน

  digitalWrite(in1_MR, LOW);                          // ตั้งค่า in1_MR เป็น LOW
  digitalWrite(in2_MR, HIGH);                             // ตั้งค่า in2_MR เป็น HIGH
  analogWrite(PWM_MR, 170);                          // ตั้งค่า PWM_MR ให้มอเตอร์ทำงานด้วยความเร็ว 170
  // รอ 250 มิลลิวินาทีก่อนหยุดหมุน
  delay(250);
  // เช็คระยะทางด้านหน้าใหม่
  myServo.write(90);                                      // หมุนเซอร์โวมอเตอร์ไปที่ 90 องศา
  delay(250);                                                // รอ 250 มิลลิวินาที
  distanceFront = measureDistance();              // วัดระยะทางด้านหน้า
  if (distanceFront > 20) {
    moveForward();                                       // ถ้าไม่มีสิ่งกีดขวางด้านหน้า ให้เคลื่อนที่ไปข้างหน้า
  } else {
    stopMoving();                                         // หยุดเคลื่อนที่
  }
}



// สร้างฟังก์ชันสำหรับการเลี้ยวขวา
void turnRight() {
  digitalWrite(in1_ML, LOW);                         // ตั้งค่า in1_ML เป็น LOW
  digitalWrite(in2_ML, HIGH);                        // ตั้งค่า in2_ML เป็น HIGH
  analogWrite(PWM_ML, 150);                      // ตั้งค่า PWM_ML ให้มอเตอร์ทำงานด้วยความเร็ว 150

  digitalWrite(in1_MR, HIGH);                       // ตั้งค่า in1_MR เป็น HIGH
  digitalWrite(in2_MR, HIGH);                       // ตั้งค่า in2_MR เป็น HIGH
  analogWrite(PWM_MR, 0);                        // ตั้งค่า PWM_MR ให้มอเตอร์หยุดทำงาน
  delay(250);                                           // รอ 250 มิลลิวินาทีก่อนหยุดหมุน
  // เช็คระยะทางด้านหน้าใหม่
  myServo.write(90);                                // หมุนเซอร์โวมอเตอร์ไปที่ 90 องศา
  delay(250);                                          // รอ 250 มิลลิวินาที
  distanceFront = measureDistance();        // วัดระยะทางด้านหน้า
  if (distanceFront > 20) {
    moveForward();                                 // ถ้าไม่มีสิ่งกีดขวางด้านหน้า ให้เคลื่อนที่ไปข้างหน้า
  } else {
    stopMoving();                                   // หยุดเคลื่อนที่
  }
}

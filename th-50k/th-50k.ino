#define MAX_DAT 600
#define MIN_DAT 400
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//Mở cổng Serial ở mức 9600
  pinMode (2, INPUT);
  pinMode(A0, INPUT);
  pinMode (13, OUTPUT);
  pinMode (2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(A0);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
                                      // Giá trị được số hóa thành 1 số nguyên có giá trị
                                      // trong khoảng từ 0 đến 1023
  Serial.println(value);//Xuất ra serial Monitor                   
  delay(10);
  if(value>MAX_DAT){
    digitalWrite (2, HIGH);
    }
  if(value<MIN_DAT){
    digitalWrite (2, LOW);
    }
  // Đọc giá trị D0 rồi điều khiển Led 13...Các bạn cũng có thể điều khiển bơm nước thông qua rơle....
  if (digitalRead (2) == 0)
  {
   digitalWrite (13, HIGH);
  }
  else{
  digitalWrite (13, LOW);
       
}

}

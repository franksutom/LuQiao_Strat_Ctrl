
/*
路桥党群的红外感应器触发串口对时序器发送开关的指令
设备：220转12V5V变压器，arduino nano，12V人体感应器，光耦模块，ttl转485
接口2接收：12V人体感应器
向485发送开启："01160000000111AA"
接收485重置："01160000000010AA"
功能：人体感应没有被阻挡时，对中控发送关闭指令
     人体感应有被阻挡时，对中控时序器发送开启指令
问题：arduino的启动加载会有一个检测后再加载的过程，所以不能开机后就发指令

中控指令
序厅开启01160000000111AA
接受重置01160000000010AA
 */
#include <SoftwareSerial.h> 

#define SequencerPinRx 6  //电源时序器模块接收端
#define SequencerPinTx 5  //电源时序器模块发射端
#define Sensor 2  //定义触发红外感应器模块接收端的针脚
/*#define Ctrl 3  //检测按钮位 */

String cmd = "open";

//连接中控485模块软连接
SoftwareSerial SequencerModel(SequencerPinRx, SequencerPinTx); // RX, TX

//向中控发送开启命令
unsigned char SequencerOn[8] = {0x01,0x16,0x00,0x00,0x00,0x01,0x11,0xAA};

//接收485重置命令
unsigned char SequencerOff[8] = {0x01,0x16,0x00,0x00,0x00,0x00,0x10,0xAA};

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  SequencerModel.begin(9600);
  while (!SequencerModel) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(Sensor,INPUT);
  //pinMode(Ctrl,INPUT);
}

void loop() {

  // 读取触发针脚的状态
  int buttonState ;
  buttonState = digitalRead(Sensor);
  
  //读取检测按钮位
  //int Ctrl_State;
  //Ctrl_State = digitalRead(Ctrl);
  
  // 判断检测按钮位，如果是High，就检测阀门扳手是否下移，下移则执行开启时序器，没下移不执行
  // 如果检测按钮位是LOW，则检测阀门扳手是否上移，上移则执行关闭时序器，没上移不执行
  if((buttonState == LOW)&&(cmd == "open")) //被触发后发播放命令，不会重复发送
  {
    SequencerModel.write(SequencerOn,8);
    delay(500);
    cmd = "close";
    Serial.println(" cmd1:"+cmd);
  }
    
 if((buttonState == HIGH)&&(cmd == "close"))  //没有触发后发停止命令，不会重复发送
 {
    SequencerModel.write(SequencerOff,8);
    delay(500);
    cmd = "open";
    Serial.println(" cmd2:"+cmd);
 }
      
 //Serial.print(" buttonState:"+(String)buttonState);
 //Serial.print(" Ctrl_State:"+(String)Ctrl_State);
 


 delay(50);        // delay in between reads for stability

}


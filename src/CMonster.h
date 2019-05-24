/* 
 * Authors: Jinwook On   (KuruKuru)
 *          Hancheol Cho (Baram) 
 *          KyungWan Ki  (Kei)
 */

#ifndef CMONSTER_H_
#define CMONSTER_H_


#include <Arduino.h>
#include <stdint.h>

namespace Ojw
{


class CMonster
{
public:
  enum EMotor_t
  {
    NONE = 0,
    // Default
    XL_320 = 350, // 300도 1024 : 512 center : 0.111 RefRpm, 1023 Limit Rpm
    XL_430 = 1060, // 360도 4096 : 2048 center : 0.229 RefRpm, 480 Limit Rpm
    AX_12 = 12, // 9~12v, 0~300도, c=512, mx=1023, mn=0, drpm=0.111rpm (0x400, 즉 10 번째 비트로 방향 제어 - 속도 모드시)
    AX_18 = 18, //

    //XM_430 = 29,
    //XM_540 = 30, // LED [65], 360도 4096 : 2048 center : 0.229 RefRpm, 480 Limit Rpm  , 확장위치제어모드시 512 회전 가능(+-256)

    XL_2XL = 1090,

    XM430_W210 = 1030,
    XM430_W350 = 1020,

    XM540_W270 = 1120,
    XM540_W150 = 1130,

    XH430_W210 = 1010,
    XH430_W350 = 1000,
    XH430_V210 = 1050,
    XH430_V350 = 1040,

    DX_113 = 113, //
    DX_116 = 116, //
    DX_117 = 117, //
    RX_10 = 10, // 9~12v, 0~300도, c=512, mx=1023, mn=0, drpm=0.111rpm (0x400, 즉 10 번째 비트로 방향 제어 - 속도 모드시)
    RX_24F = 24, // 9~12v, 0~300도, c=512, mx=1023, mn=0, drpm=0.111rpm (0x400, 즉 10 번째 비트로 방향 제어 - 속도 모드시)
    RX_28 = 28, // 12v~18.5v, 0~300도, c=512, mx=1023, mn=0, drpm=0.111rpm (0x400, 즉 10 번째 비트로 방향 제어 - 속도 모드시)
    RX_64 = 64, // 12v~18.5v, 0~300도, c=512, mx=1023, mn=0, drpm=0.111rpm (0x400, 즉 10 번째 비트로 방향 제어 - 속도 모드시)
    EX_106 = 106, // 12v~18.5v, 0~251도, c=2048, mx=4095, mn=0, 
    EX_106P = 107, // 12v~18.5v, 0~251도, c=2048, mx=4095, mn=0, 


    // protocol 2.0
    MX_12 = 360, // 10~14.8v, 0~360, c=2048, mx=4095, mn=0, drpm = 0.916rpm, 다중회전가능(각 7바퀴:-28,672~28,672)
    MX_28 = 30, // 10~14.8v, 0~360, c=2048, mx=4095, mn=0, drpm = 0.114rpm, 다중회전가능(각 7바퀴:-28,672~28,672)  
    MX_64 = 311, // 10~14.8v, 0~360, c=2048, mx=4095, mn=0, drpm = 0.114rpm, 다중회전가능(각 7바퀴:-28,672~28,672)  
    MX_106 = 321, // 10~14.8v, 0~360, c=2048, mx=4095, mn=0, drpm = 0.114rpm, 다중회전가능(각 7바퀴:-28,672~28,672)  
    //MX_ = 17, //

    SG_90 = 100
  };

public:
  CMonster();
  ~CMonster();

  bool Open(int nPort, int nBaudRate);
  void Close(void);
  void Close(int nPort);
  void SetTorq(bool bOn);
  void SetTorq(int nID, bool bOn);
  void AutoSet();
  void Send_Motor(void);
  void Send_Motor(int nMilliseconds);
  void Wait();
  void Wait_Per(float fPercent_0_1);
  void Wait(int nMilliseconds);
  void Delay(int nMilliseconds);

  void SetParam_MotorType(int nCommIndex, int nMotor, EMotor_t EMot);
};


}
#endif 

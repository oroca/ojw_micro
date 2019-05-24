/* 
 * Authors: Jinwook On   (KuruKuru)
 *          Hancheol Cho (Baram) 
 *          KyungWan Ki  (Kei)
 */

#ifndef CTIMER_H_
#define CTIMER_H_


#include <Arduino.h>
#include <stdint.h>

namespace Ojw
{


class CTimer
{
public:
  CTimer();
  ~CTimer();
  static void Wait(uint32_t t);
  static uint32_t GetTick();
};


}
#endif 

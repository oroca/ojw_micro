/* 
 * Authors: Jinwook On   (KuruKuru)
 *          Hancheol Cho (Baram) 
 *          KyungWan Ki  (Kei)
 */

#include "CTimer.h"

namespace Ojw
{


CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

void CTimer::Wait(uint32_t t)
{
  delay(t);
}

uint32_t CTimer::GetTick()
{
  return millis();
}


}
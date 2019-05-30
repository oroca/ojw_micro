/* 
 * Authors: Jinwook On   (KuruKuru)
 *          Hancheol Cho (Baram) 
 *          KyungWan Ki  (Kei)
 */


#include <Arduino.h> 
#include "CMotion.h"


namespace Ojw
{

#define _STR_EXT        "dmt"
#define _STR_EXT_UP     "DMT"
#define _STR_VER_V_12   "1.2"
#define _STR_VER_V_11   "1.1"
#define _STR_VER_V_10   "1.0"
#define _SIZE_FILE_NAME 21




CMotion::CMotion()
{
  file_list.cnt = 0;
  file_list.index = 0;
  file_list.cur_play = -1;

  motion_start_func = NULL;
  motion_motor_func = NULL;  
}

CMotion::~CMotion()
{
}

void CMotion::setMotionStartCallback(void (*start_func)(SMotionHeader_t *p_header))
{
  motion_start_func = start_func;
}

void CMotion::setMotionMotorCallback(bool (*motor_func)(SMotionHeader_t *p_header, SMotionTableDB_t *p_table))
{
  motion_motor_func = motor_func;
}

bool CMotion::addMotionFile(uint16_t number, char *name, const uint8_t *motion_ptr)
{
  bool ret = false;
  SMotionFileListNode_t *p_node;


  if (file_list.index < OJW_MOTION_LIST_COUNT)
  {
    file_list.p_node[file_list.index] = (SMotionFileListNode_t *)malloc(sizeof(SMotionFileListNode_t));

    if (file_list.p_node[file_list.index] != NULL)
    {
      p_node = file_list.p_node[file_list.index];

      p_node->number = number;
      p_node->p_motion_buf = (uint8_t *)motion_ptr;
      strcpy(p_node->motion_name, name);

      file_list.cnt++;

      ret = true;
    }
  }

  return ret;
}

bool CMotion::playMotion(char *name)
{
  bool ret = true;
  int16_t number = -1;
  uint16_t i;


  for (i=0; i<file_list.cnt; i++)
  {
    if (strcmp(file_list.p_node[i]->motion_name, name) == 0)
    {
      number = i;
      break;
    }
  }

  if (number >= 0)
  {
    playMotion(number);
  }

  return ret;
}


bool CMotion::playMotion(int number)
{
  bool ret = true;
  int16_t index = -1;
  uint16_t i;


  for (i=0; i<file_list.cnt; i++)
  {
    if (file_list.p_node[i]->number == number)
    {
      index = i;
      break;
    }
  }

  if (index >= 0)
  {
    if (MotionLoadHeader(&m_SMotion, file_list.p_node[index]->p_motion_buf) == true)
    {
      if (motion_start_func != NULL)
      {
        (*motion_start_func)(&m_SMotion.SHeader);
      }
          
      for (i=0; i<m_SMotion.SHeader.nFrameSize; i++)
      {
        MotionGetTable(&m_SMotion, i);
    
        if (motion_motor_func != NULL)
        {
          if ((*motion_motor_func)(&m_SMotion.SHeader, &m_SMotion.STable) == true)
          {
            break;
          }
        }
        delay(m_SMotion.STable.nTime);  
      }
    }      
  }

  return ret;
}

#define PROG_MEM(x)    pgm_read_byte_near(&(x))
//#define PROG_MEM(x)      x

bool CMotion::MotionLoadHeader(SMotionDB_t *pMotion, uint8_t *pData)
{
  pMotion->pData = pData;

  for (int i=0; i<6; i++)
  {
    pMotion->SHeader.strVersion[i] = PROG_MEM(pData[i]);
  }

  if (
    ( strncmp(_STR_EXT, pMotion->SHeader.strVersion, strlen(_STR_EXT)) != 0 ) &&
    ( strncmp(_STR_EXT_UP, pMotion->SHeader.strVersion, strlen(_STR_EXT_UP)) != 0 )
    )
  {
    return false;
  }

  // version check
  int nVersion = 10;
  if ( strncmp(_STR_VER_V_10, &pMotion->SHeader.strVersion[3], strlen(_STR_VER_V_10)) != 0 )
  {
    if ( strncmp(_STR_VER_V_11, &pMotion->SHeader.strVersion[3], strlen(_STR_VER_V_11)) == 0 )
    {
      nVersion = 11;
    }
    else
    {
      if ( strncmp(_STR_VER_V_12, &pMotion->SHeader.strVersion[3], strlen(_STR_VER_V_12)) == 0 )
      {
        nVersion = 12;
      }
      else
      {
        return false;
      }
    }
  }


  if (nVersion == 10)
  {
    for (int i=0; i<21; i++)
    {
      pMotion->SHeader.strTableName[i] = PROG_MEM(pData[6+i]);
    }

    // Start Position(1)
    int nPos = 27;

    pMotion->SHeader.nStartPosition = (int)((PROG_MEM(pData[nPos]) >= 0) ? PROG_MEM(pData[nPos]) : 0);
    nPos++;
    // MotionFrame(2), Comment(2), Caption(2), PlayTime(4), RobotModelNumber(2), MotorCnt(1)
    // Size
    pMotion->SHeader.nFrameSize       = (int)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;
    pMotion->SHeader.nCommentSize     = (int)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;
    pMotion->SHeader.nCnt_LineComment = (int)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;
    pMotion->SHeader.nPlayTime        = (int)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256 + PROG_MEM(pData[nPos + 2]) * 256 * 256 + PROG_MEM(pData[nPos + 3]) * 256 * 256 * 256); nPos += 4;
    pMotion->SHeader.nRobotModelNum   = (int)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;
    pMotion->SHeader.nMotorCnt        = (int)(PROG_MEM(pData[nPos++]));
    // Size - MotionFrame, Comment, Caption, PlayTime
  }
  else
  {
    return false;
  }

  return true;
}

bool CMotion::MotionGetTable(SMotionDB_t *pMotion, uint16_t tableIndex)
{
  uint32_t nPos;
  uint8_t *pData;
  uint16_t nMemorySize;
  int32_t  nData;
  int16_t  sData;

  pData = pMotion->pData;

  nMemorySize = 35 + pMotion->SHeader.nMotorCnt * 2;
  nPos = 41 + nMemorySize * tableIndex;


  int nEn = PROG_MEM(pData[nPos++]);
  pMotion->STable.bEn = ((nEn & 0x01) != 0) ? true : false;

  int nMotorCntMax = pMotion->SHeader.nMotorCnt;

  if (pMotion->SHeader.nMotorCnt > OJW_MOTOR_MAX_COUNT )
  {
    nMotorCntMax = OJW_MOTOR_MAX_COUNT;
  }

  // 0-Index, 1-En, 2 ~ 24, 25 - speed, 26 - delay, 27,28,29,30 - Data0-3, 31 - time, 32 - caption
  for (int nAxis = 0; nAxis < nMotorCntMax; nAxis++)
  {
    if (nAxis >= pMotion->SHeader.nMotorCnt) nPos += 2;
    else if (nAxis >= pMotion->SHeader.nMotorCnt) pMotion->STable.pnMot[nAxis] = 0;
    else
    {
      //memcpy(&nData, &pData[nPos], sizeof(byte) * 2); nPos += 2;

      nData  = PROG_MEM(pData[nPos+0]);
      nData |= PROG_MEM(pData[nPos+1]) << 8;
      nPos += 2;

      sData = (short)nData;

      pMotion->STable.pnMot[nAxis] = sData;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // #region Speed(2), Delay(2), Group(1), Command(1), Data0(2), Data1(2)
  // Speed
  pMotion->STable.nTime = (int16_t)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;
  
  
  // Delay
  pMotion->STable.nDelay = (int16_t)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;

  // Group
  pMotion->STable.nGroup = (uint8_t)(PROG_MEM(pData[nPos++]));

  // Command
  pMotion->STable.nCmd = (uint8_t)(PROG_MEM(pData[nPos++]));

  // Data0
  pMotion->STable.nData0 = (int16_t)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;

  // Data1
  pMotion->STable.nData1 = (int16_t)(PROG_MEM(pData[nPos]) + PROG_MEM(pData[nPos + 1]) * 256); nPos += 2;
  //
  pMotion->STable.nData2 = 0; //nPos++;
  pMotion->STable.nData3 = 0; //nPos++;
  pMotion->STable.nData4 = 0; //nPos++;
  pMotion->STable.nData5 = 0; //nPos++;


  return true;
}


}
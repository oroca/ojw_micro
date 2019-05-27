/* 
 * Authors: Jinwook On   (KuruKuru)
 *          Hancheol Cho (Baram) 
 *          KyungWan Ki  (Kei)
 */

#ifndef _OJW_MOTION_H_
#define _OJW_MOTION_H_

#include <inttypes.h>
#include <Arduino.h> 



#define OJW_MOTOR_MAX_COUNT     32
#define OJW_MOTION_NAME_LEN     8
#define OJW_MOTION_LIST_COUNT   16


namespace Ojw
{

typedef struct {
  bool     bEn;
  int16_t  pnMot[OJW_MOTOR_MAX_COUNT];
  uint8_t  pnLed[OJW_MOTOR_MAX_COUNT];
  bool     pbEn[OJW_MOTOR_MAX_COUNT];
  bool     pbType[OJW_MOTOR_MAX_COUNT];
  int16_t  nTime;
  int16_t  nDelay;
  uint8_t  nGroup;
  uint8_t  nCmd;
  int16_t  nData0;
  int16_t  nData1;
  int16_t  nData2;
  int16_t  nData3;
  int16_t  nData4;
  int16_t  nData5;
} SMotionTableDB_t;

typedef struct {
  char    strVersion[6];
  char    strTableName[21];

  uint8_t  nStartPosition;
  uint16_t nFrameSize;
  uint16_t nCommentSize;
  uint16_t nCnt_LineComment;
  uint32_t nPlayTime;
  uint16_t nRobotModelNum;
  uint8_t  nMotorCnt;
} SMotionHeader_t;


typedef struct {
  uint8_t         *pData;
  SMotionHeader_t  SHeader;
  SMotionTableDB_t STable;
} SMotionDB_t;


typedef struct
{
  uint16_t number;
  uint8_t  *p_motion_buf;
  char     motion_name[OJW_MOTION_NAME_LEN];
} SMotionFileListNode_t;

typedef struct
{
  uint16_t cnt;
  uint16_t index;
  int16_t  cur_play;

  SMotionFileListNode_t *p_node[OJW_MOTION_LIST_COUNT];
} SMotionFileList_t;



class CMotion
{
public:
  CMotion();
  ~CMotion();

  void setMotionStartCallback(void (*start_func)(SMotionHeader_t *p_header));
  void setMotionMotorCallback(bool (*motor_func)(SMotionHeader_t *p_header, SMotionTableDB_t *p_table));

  bool addMotionFile(uint16_t number, char *name,  const uint8_t *motion_ptr);
  bool playMotion(int number);
  bool playMotion(char *name);

public:
  SMotionDB_t m_SMotion;
  

private:

  void (*motion_start_func)(SMotionHeader_t *p_header);
  bool (*motion_motor_func)(SMotionHeader_t *p_header, SMotionTableDB_t *p_table);


  SMotionFileList_t file_list;


  bool MotionLoadHeader(SMotionDB_t *pMotion, uint8_t *pData);
  bool MotionGetTable(SMotionDB_t *pMotion, uint16_t tableIndex);
};

}

#endif

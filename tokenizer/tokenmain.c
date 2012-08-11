#include "Stringlib.h"
#include "../train/ironBird.h"
#include "../syscall/sysCall.h"
#include "../include/util/type.h"
#include "../include/i486/cs452.h"
#define NUMTRAIN      6

int IsTrain(char * str) {
  /* Does the leading letters in str == train (or Train).
   * Return 1 if yes, 0 if no.
   */
  int retVal;                   /* Value to return */

  retVal = TruncStrCmp(len("Train"), str, "train");
  if (!retVal) {
    retVal = TruncStrCmp(len("Train"), str, "Train");
  }
  if (retVal < 1) return 0;     /* User does not need to worry about OVERRUN */
  return retVal;
}

int IsRun(char * str) {
  /* Does the leading letters in str == run.
   * Return 1 if yes, 0 if no.
   */
  int retVal;                   /* Value to return */

  retVal = TruncStrCmp(len("run"), str, "run");
  if (retVal < 1) return 0;     /* User does not need to worry about OVERRUN */
  return retVal;
}

int IsHalt(char * str) {
  /* Does the leading letters in str == halt.
   * Return 1 if yes, 0 if no.
   */
  int retVal;                   /* Value to return */

  retVal = TruncStrCmp(len("halt"), str, "halt");
  if (retVal < 1) return 0;
  return retVal;
}

int RunWhat(char * str, int n) {
  /* Based on the chars from n to len(str) deterimine the run type,
   * i.e. mission, random or init.
   */

  if (DubTruncStrCmp((n + 1), len(str), str, "mission") > 0) {
    if (len(str) == (len("mission") + n)) return MISSION;
  }
  else if (DubTruncStrCmp((n + 1), len(str), str, "random") > 0) {
    if (len(str) == (len("random") + n)) return RANDOM;
  }
  else if (DubTruncStrCmp((n + 1), len(str), str, "init") > 0) {
    if (len(str) == (len("init") + n)) return INIT;
  }
  return NOOFABOVE;
}

int getInt(char * str, int n) {
  /* Read n and n+1 byte from str, convert the two values, if possible,
   * to an integer and return the integer. i.e. '4' '5' would be returned
   * as 45. If a char is out of range return OVERRUN.
   */
  int ReturnVal;                /* Value to return */

  if ((str[n] < '0') || (str[n] > '9')) return OVERRUN;
  if ((str[(n + 1)] < '0') || (str[(n + 1)] > '9')) return OVERRUN;

  ReturnVal = (str[n] - '0');
  ReturnVal *= 10;
  ReturnVal += (str[(n + 1)] - '0');

  return ReturnVal;
}

char getSens(char * str, int n) {
  /* Read n, n+1 and n+2 bytes from str, convert the values, if possible,
   * to a sensor letter desgination (high nibble represents the module)
   * low nibble represents the sensor number. If char is out of range,
   * return OVERRUN.
   */

  char ReturnVal;               /* Value to return */
  int SensNum;                  /* Sensor number */

  if (str[n] > 'Z') {           /* want to support small letters and big letters */
  ReturnVal = (str[n] - 'a');
  } else {
    ReturnVal = (str[n] - 'A');
  }

  if ((ReturnVal < 0) || (ReturnVal > 4)) return OVERRUN;

  ReturnVal *= 16;              /* Push the module letter into the high nibble */

  SensNum = getInt(str, (n + 1));
  if ((SensNum < 1) || (SensNum > 16)) return OVERRUN;

  ReturnVal += (SensNum - 1);

  return ReturnVal;
}

void displayHelp(void) {

  char WriteData[STRLEN];       /* Data to Write */
#define VERBOSE
#ifdef VERBOSE
  /* Write a helpful error message to the WYSE USART */
  sprintf( WriteData, "train nn -s mnn -e mnn" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "\nexample:\n" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "train 12 -s A01 -e B15" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "\n" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "train 12 -s xxx -e B15" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "\nrun mission\n" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "run random\n" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "run init\n" );
  Write(WriteData, strLen(WriteData), WYSE);
  sprintf( WriteData, "halt\n" );
  Write(WriteData, strLen(WriteData), WYSE);
#else
  sprintf( WriteData, "Bad command\n" );
  Write(WriteData, strLen(WriteData), WYSE);
#endif
}

int validTrainNum(int TrainNum, char * runningTrain) {
  /* If TrainNum is a valid train number, return TrainNum,
   * else return (-1)
   */

  int i;                        /* loop index */
  for (i = 0; i < NUMTRAIN; i++) {
    if (TrainNum == runningTrain[i]) {
      return TrainNum;
    }
  }
  return (-1);
}

main() {
  char InputData[STRLEN];       /* Data to process */
  char WriteData[STRLEN];       /* Data to Write */
  char Instruction[CMD2CC_SIZE];/* Data to send to Central Control */
  char runningTrain[NUMTRAIN];  /* Boolean which trains are live */
  int TrainNum;                 /* Train user wishes to control */
  char SrcNode;                 /* Source Sensor */
  char DestNode;                /* Destination Sensor */
  int DoWhat;                   /* What has to be done, mission, random or init? */
  int CentralControlTID;        /* TID of the Central Controler */

  RegisterAs("Tokenizer");
  Delay(10);

  /* waiting signal from CC */
  CentralControlTID = Receive(&runningTrain, (NUMTRAIN * sizeof(char)));
  Reply(CentralControlTID, NULL, NULL);

  while (1) {
    sprintf(WriteData, "\n$ ");
    Write(WriteData, strLen(WriteData), WYSE);
    Read(InputData,WYSE);
    if (IsHalt(InputData)) {
      Instruction[0] = HALT_CMD;
      Send(CentralControlTID, &Instruction, (sizeof(char) * CMD2CC_SIZE), NULL, NULL); 
    } else if (IsTrain(InputData)) {
      TrainNum = getInt(InputData, len("train "));
      TrainNum = validTrainNum(TrainNum, runningTrain);
      SrcNode =  getSens(InputData, len("train nn -s "));
      DestNode = getSens(InputData, len("train nn -s nnn -e "));
      if ((TrainNum < 0) || (SrcNode  < 0) || (DestNode < 0)) {
        TrainNum = OVERRUN;     /* Handle bad input by ignoring it */
        /*displayHelp();*/
      } else {
        Instruction[0] = TRN_CMD; Instruction[1] = TrainNum;
        Instruction[2] = SrcNode; Instruction[3] = DestNode;
        Send(CentralControlTID, &Instruction, (sizeof(char) * CMD2CC_SIZE), NULL, NULL);
      }
    } else if (IsRun(InputData)) {
        DoWhat = RunWhat(InputData, len("run "));
        if (DoWhat == MISSION) {
          Instruction[0] = MISS_CMD;
          Send(CentralControlTID, &Instruction, (sizeof(char) * CMD2CC_SIZE), NULL, NULL);
        } else if (DoWhat == RANDOM) {
          Instruction[0] = RAND_CMD;
          Send(CentralControlTID, &Instruction, (sizeof(char) * CMD2CC_SIZE), NULL, NULL);
        } else if (DoWhat == INIT) {
          Instruction[0] = INIT_CMD;
          Send(CentralControlTID, &Instruction, (sizeof(char) * CMD2CC_SIZE), NULL, NULL);
        }
    } else {
      /*displayHelp();*/
    }
  }
}

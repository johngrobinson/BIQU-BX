#include "Speed.h"
#include "../includes.h"

MENUITEMS percentageItems = {
// title
LABEL_PERCENTAGE_SPEED,
// icon                       label
{{ICON_DEC,                  LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_INC,                  LABEL_INC},
  {ICON_MOVE,                 LABEL_PERCENTAGE_SPEED},
  {ICON_E_5_PERCENT,          LABEL_5_PERCENT},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_BACK,                 LABEL_BACK},}
};

const ITEM itemPercentage[SPEED_NUM] = {
// icon                       label
  {ICON_MOVE,                 LABEL_PERCENTAGE_SPEED},
  {ICON_EXTRUDE,              LABEL_PERCENTAGE_FLOW},
};

static int16_t itemPercentageTitle[SPEED_NUM] = {
  LABEL_PERCENTAGE_SPEED,     LABEL_PERCENTAGE_FLOW
};

static uint8_t item_percentage_i = 0;

#define ITEM_PERCENT_UNIT_NUM 3

const ITEM itemPercentUnit[ITEM_PERCENT_UNIT_NUM] = {
// icon                       label
  {ICON_E_1_PERCENT,          LABEL_1_PERCENT},
  {ICON_E_5_PERCENT,          LABEL_5_PERCENT},
  {ICON_E_10_PERCENT,         LABEL_10_PERCENT},
};

const  uint8_t item_percent_unit[ITEM_PERCENT_UNIT_NUM] = {1, 5, 10};
static uint8_t item_percent_unit_i = 0;

void setSpeedItemIndex(uint8_t index)
{
  item_percentage_i = index;
}

void percentageReDraw(char * title)
{
  char tempstr[20];

  GUI_DispString(exhibitRect.x0, exhibitRect.y0, (uint8_t *)title);

  sprintf(tempstr, "  %d%%  ", speedGetPercent(item_percentage_i));
  setLargeFont(true);
  GUI_DispStringInPrect(&exhibitRect,(uint8_t*)tempstr);
  setLargeFont(false);
}

void menuSpeedCallBack(void)
{ 
  KEY_VALUES key_num   = menuKeyGetValue();
  uint16_t now = speedGetPercent(item_percentage_i);
  
  #ifdef SUTOADD
  KEY_VALUES key_num_P = menuKeyPressValue();
  switch (key_num_P)
  {
    case KEY_ICON_0:
        if(now > SPEED_MIN)
        speedSetPercent(item_percentage_i, now - item_percent_unit[item_percent_unit_i]);
        percentageReDraw((char*)textSelect(percentageItems.title.index));
      break;
    case KEY_ICON_3:
        if(now < SPEED_MAX)
        speedSetPercent(item_percentage_i, now + item_percent_unit[item_percent_unit_i]);
        percentageReDraw((char*)textSelect(percentageItems.title.index));
      break;
    default:
      break;
  }
  #endif
  switch(key_num)
  {
    #ifndef SUTOADD
    case KEY_ICON_0:
        if(now > SPEED_MIN)
        speedSetPercent(item_percentage_i, now - item_percent_unit[item_percent_unit_i]);
        percentageReDraw((char*)textSelect(percentageItems.title.index));
      break;
    case KEY_ICON_3:
        if(now < SPEED_MAX)
        speedSetPercent(item_percentage_i, now + item_percent_unit[item_percent_unit_i]);
        percentageReDraw((char*)textSelect(percentageItems.title.index));
      break;
    #endif

    case KEY_ICON_4:
      item_percentage_i = (item_percentage_i+1) % SPEED_NUM;
      percentageItems.items[key_num] = itemPercentage[item_percentage_i];
      menuDrawItem(&percentageItems.items[key_num], key_num);
      percentageItems.title.index = itemPercentageTitle[item_percentage_i];
      menuDrawTitle(textSelect(percentageItems.title.index));
      GUI_ClearPrect(&exhibitRect);
      percentageReDraw((char*)textSelect(percentageItems.title.index));
      break;

    case KEY_ICON_5:
      item_percent_unit_i = (item_percent_unit_i+1) % ITEM_PERCENT_UNIT_NUM;
      percentageItems.items[key_num] = itemPercentUnit[item_percent_unit_i];
      menuDrawItem(&percentageItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      speedSetPercent(item_percentage_i, 100);
      percentageReDraw((char*)textSelect(percentageItems.title.index));
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default:
      #if LCD_ENCODER_SUPPORT
        if(encoderPosition)
        {
          if (now < SPEED_MAX && encoderPosition > 0)
          {
            speedSetPercent(item_percentage_i, now + item_percent_unit[item_percent_unit_i]);
          }
          if (now > SPEED_MIN && encoderPosition < 0)
          {
            speedSetPercent(item_percentage_i, now - item_percent_unit[item_percent_unit_i]);
          }
          encoderPosition = 0;
        }
      #endif
    break;
  }

  if (SpeedChanged(item_percentage_i))
    percentageReDraw((char *)textSelect(percentageItems.title.index));
  loopFrontEnd();
}
void menuSpeed(void)
{
  percentageItems.items[KEY_ICON_5] = itemPercentUnit[item_percent_unit_i];

  storeCmd("M220\nM221\n");
  for(uint8_t i=0; i<SPEED_NUM; i++)
    percentageItems.title.index = itemPercentageTitle[item_percentage_i];
  percentageItems.items[KEY_ICON_4] = itemPercentage[item_percentage_i];
  menuDrawPage(&percentageItems);
  percentageReDraw((char*)textSelect(percentageItems.title.index));
  
  #if LCD_ENCODER_SUPPORT
    encoderPosition = 0;
  #endif

  menuSetFrontCallBack(menuSpeedCallBack);
}
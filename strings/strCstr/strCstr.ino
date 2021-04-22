#include "mString.h"
void setup() {
  Serial.begin(9600);
  int val1 = 1234;
  int val2 = 5678;
  uint32_t us = micros();

  //mString<60> mstr;
  //mstr = mstr + "http://website.ru/request?param1=" + val1 + "&param2=" + val2;
  //mstr = "12345abc";
  //int valm = mstr.toInt();
  //mstr = "param,12345";
  //char* strs[2];
  //mstr.split(strs, ',');

  /*
    String str;
    str = str + "http://website.ru/request?param1=" + val1 + "&param2=" + val2;
    Serial.println(str);
  */
  /*
    char cstr[60] = "";
    strcat(cstr, "http://website.ru/request?param1=");
    //char cint[10];
    //itoa(val1, cint, DEC);
    //strcat(cstr, cint);
    itoa(val1, cstr + strlen(cstr), DEC);
    strcat(cstr, "&param2=");
    itoa(val2, strchr(cstr, NULL), DEC);
    Serial.println(cstr);
  */
  /*
    String str2 = "12345abc";
    int vals = str2.toInt();
    Serial.println(vals);
  */
  /*
    char cstr2[] = "12345abc";
    int valc = atoi(cstr2);
    Serial.println(valc);
  */
  /*
    String str3 = "param,12345";
    int comma = str3.indexOf(',');
    String name = str3.substring(0, comma);
    Serial.println(name);
    int value = str3.substring(comma + 1, str3.length()).toInt();
    Serial.println(value);
  */
  /*
    char cstr3[] = "param,12345";
    char* comma = strchr(cstr3, ',');
    char name[10] = "";
    strncpy(name, cstr3, comma-cstr3);
    int value = atoi(++comma);
    Serial.println(name);
    Serial.println(value);
  */
  /*
    char cstr3[] = "param,12345";
    char* comma = strchr(cstr3, ',');
    comma[0] = NULL;
    int value = atoi(++comma);
    Serial.println(cstr3);
    Serial.println(value);
  */

  Serial.println(micros() - us);
}


void loop() {
}
/*
  3626-240 - 324
  2008-230 - 172
  2256-230 - 436

  3130-206 - 36
  1972-196 - 16
  2072-196 - 40

  3654-210 - 148
  2074-200 - 28 и 20

  сплит 1928-200 - 36
*/

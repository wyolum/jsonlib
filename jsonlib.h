#ifndef JSONLIB_H
#define JSONLIB_H

#include <Arduino.h>

String jsonIndexList(String json, int idx);
String jsonExtract(String json, String name);

#endif

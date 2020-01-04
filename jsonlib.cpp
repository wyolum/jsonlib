#include "jsonlib.h"

String jsonIndexList(String json, int idx){
  int count = 1; // number of braces seen { = +1 } = -1
  int i = 1;
  int item_idx = 0;
  int start = i;
  int stop = json.length() - 1;
  
  while(i < json.length() && count > 0){
    if(json.charAt(i) == ']' or json.charAt(i) == '}'){
      count--;
    }
    if(json.charAt(i) == '{' or json.charAt(i) == '['){
      count++;
    }
    if(count == 1 && json.charAt(i) == ',' && item_idx == idx){
      //item separator!
      stop = i;
      return json.substring(start, stop);
    }
    if(count == 1 && json.charAt(i) == ']' && item_idx == idx){ 
	stop = i + 1;
	return json.substring(start, stop);
    }
    if(count == 1 && json.charAt(i) == ','){
      item_idx++;
      start = i + 1;
    }
    i++;
  }
  return json.substring(start, stop);
}

// return a sub-json struct
String jsonExtract(String json, String name){
  char next;
  int start, stop;
  
  name = String("\"") + name + String("\"");
  start = json.indexOf(name) + name.length() + 1;
  next = json.charAt(start);
  if(next == '\"'){
    //Serial.println(".. a string");
    start = start + 1;
    stop = json.indexOf('"', start + 1);
  }
  else if(next == '['){
    //Serial.println(".. a list");
    int count = 1;
    int i = start;
    while(count > 0 && i++ < json.length()){
      if(json.charAt(i) == ']'){
	count--;
      }
      else if(json.charAt(i) == '['){
	count++;
      }
    }
    stop = i + 1;
  }
  else if(next == '{'){
    //Serial.println(".. a struct");
    int count = 1;
    int i = start;
    while(count > 0 && i++ < json.length()){
      if(json.charAt(i) == '}'){
	count--;
      }
      else if(json.charAt(i) == '{'){
	count++;
      }
    }
    stop = i + 1;
  }
  else if(next == '.' || next == '-' || ('0' <= next  && next <= '9')){
    //Serial.println(".. a number");
    int i = start;
    while(i++ < json.length() && json.charAt(i) == '.' || ('0' <= json.charAt(i)  && json.charAt(i) <= '9')){
    }
    stop = i;
  }
  return json.substring(start, stop);
}


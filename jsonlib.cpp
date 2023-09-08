#include "jsonlib.h"

// remove all white space from the json string... preserving strings
String jsonRemoveWhiteSpace(const String& json){
  int i = 0;
  int cursor = 0;
  int quote_count = 0;
  String out = String();
  char out_chars[json.length()+1];
  
  for(i=0; i<json.length(); i++){
    if(json[i] == ' ' || json[i] == '\n' || json[i] == '\t' || json[i] == '\r'){
      if(quote_count % 2){ // inside a string
	out_chars[cursor++] = json[i];
      }
      else{ // outside a string!
      }
    }
    else{
      if(json[i] == 34){ // ascii dounble quote
	//check for escaped quote
	if(i > 0 && json[i - 1] == '\\'){
	  //escaped!
	}
	else{ // not escaped
	  quote_count++;
	}
      }
      out_chars[cursor++] = json[i];
    }
  }
  out_chars[cursor] = 0;
  out = String(out_chars);
  return out;
}

String jsonIndexList(const String& json, int idx){
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
String jsonExtract(const String& json, const String& nameArg){
  char next;
  int start = 0, stop = 0;
  static const size_t npos = -1;
  const String QUOTE = "\"";
  
  String name = QUOTE + nameArg + QUOTE;
  if (json.indexOf(name) == npos) return json.substring(0,0);
  start = json.indexOf(name) + name.length() + 1;
  next = json.charAt(start);
  
  while(start < json.length() && next == ' ') { // filters blanks before value if there
    start++;
    next = json.charAt(start);
  }
  
  if(next == '\"'){
    //Serial.println(".. a string");
    start = start + 1;
    stop = json.indexOf('"', start);
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
  else if(next == 't'){
	//Serial.println("... a boolean true");
	int i = start;
	
	while(i++ < json.length()){
		if(json.charAt(i) == 't' && json.charAt(i + 1) == 'r' && json.charAt(i + 2) == 'u' && json.charAt(i + 3) == 'e')
		{
			stop = i + 3;
			break;
		}
		else if (json.charAt(i) == ',' || json.charAt(i) == '}' || json.charAt(i) == ']' || json.charAt(i) == ']') {
			stop = i;
			break;
		}
	}
  } 
  else if(next == 'f' ){
	//Serial.println("... a boolean false");
    int i = start;
	
	if(json.charAt(i) == 'f' && json.charAt(i + 1) == 'a' && json.charAt(i + 2) == 'l' && json.charAt(i + 3) == 's' && json.charAt(i + 4) == 'e')
	{
		stop = i + 4;
	}
	else {
		stop = i;
	}	
  } 
  else if(next == 'n' ){
	//Serial.println("... a null");
    int i = start;
	if(json.charAt(i) == 'n' && json.charAt(i + 1) == 'u' && json.charAt(i + 2) == 'l' && json.charAt(i + 3) == 'l')
	{
		stop = i + 3;
	}
	else {
		stop = i;
	}
  }  
  else if(next == '.' || next == '-' || ('0' <= next  && next <= '9')){
    //Serial.println(".. a number");
    int i = start;
    while(i++ < json.length() && (json.charAt(i) == '.' || ('0' <= json.charAt(i)  && json.charAt(i) <= '9'))){
    }
    stop = i;
  }
  return json.substring(start, stop);
}


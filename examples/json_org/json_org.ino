#include <jsonlib.h>

String json = String(
"{"
"    \"glossary\": {"
"        \"title\": \"example glossary\","
"		\"GlossDiv\": {"
"            \"title\": \"S\","
"			\"GlossList\": {"
"                \"GlossEntry\": {"
"                    \"ID\": \"SGML\","
"					\"SortAs\": \"SGML\","
"					\"GlossTerm\": \"Standard Generalized Markup Language\","
"					\"Acronym\": \"SGML\","
"					\"Abbrev\": \"ISO 8879:1986\","
"					\"GlossDef\": {"
"                        \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\","
"						\"GlossSeeAlso\": [\"GML\", \"XML\"]"
"                    },"
"					\"GlossSee\": \"markup\""
"                }"
"            }"
"        }"
"    }"
"}");

void setup(){
  Serial.begin(115200);
  Serial.println("Simple json library offered by wyolum.com\n\n");
  
  json = jsonRemoveWhiteSpace(json);
  String glossary = jsonExtract(json, "glossary");
  String glossdiv = jsonExtract(glossary, "GlossDiv");
  String glossentry = jsonExtract(glossdiv, "GlossEntry");
  String glossterm = jsonExtract(glossentry, "GlossTerm");
  Serial.print("GlossTerm:");
  Serial.println(glossterm);
}
void loop(){
}

  /* 
Simple library for parsing simple JSON with two functions:

// pull named component out of the json String.
String jsonExtract(String json, String name);

//pull indexed component from the json list (in the String).
String jsonIndexList(String json, int idx);

Example JSON received from openweathermap.org
{"coord":{"lon":-77.35,"lat":38.93},"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10d"}],"base":"stations","main":{"temp":286.92,"feels_like":287.06,"temp_min":283.15,"temp_max":292.04,"pressure":1003,"humidity":100},"visibility":16093,"wind":{"speed":1.5,"deg":20},"rain":{"1h":0.46},"clouds":{"all":75},"dt":1577728963,"sys":{"type":1,"id":4624,"country":"US","sunrise":1577708860,"sunset":1577742942},"timezone":-18000,"id":4781530,"name":"Reston","cod":200}
   */

#include <jsonlib.h>

String json = String("{\"coord\":{\"lon\":-77.35,\"lat\":38.93},"
		     "\"weather\":[{\"id\":500,\"main\":\"Rain\",\"description\":\"light rain\",\"icon\":\"10d\"}],"
		     "\"base\":\"stations\",\"main\":{\"temp\":286.92,\"feels_like\":287.06,\"temp_min\":283.15,\"temp_max\":292.04,"
		     "\"pressure\":1003,\"humidity\":100},\"visibility\":16093,\"wind\":{\"speed\":1.5,\"deg\":20},\"rain\":{\"1h\":0.46},"
		     "\"clouds\":{\"all\":75},\"dt\":1577728963,\"sys\":{\"type\":1,\"id\":4624,\"country\":\"US\","
		     "\"sunrise\":1577708860,\"sunset\":1577742942},\"timezone\":-18000,\"id\":4781530,\"name\":\"Reston\",\"cod\":200}");

/*
OUTPUT:
weather_list:[{"id":500,"main":"Rain","description":"light rain","icon":"10d"}]
weather_0:{"id":500,"main":"Rain","description":"light rain","icon":"10d"}
Location:38.930000,-77.349998.  Current conditions: light rain
*/
void setup(){
  Serial.begin(115200);
  Serial.println("Simple json library offered by wyolum.com\n\n");
  
  String posStr = jsonExtract(json, "coord");          // {"lon":-77.35,"lat":38.93}
  float lat = jsonExtract(posStr, "lat").toFloat();    // 38.93
  float lon = jsonExtract(posStr, "lon").toFloat();    // -77.35
  String weather_list = jsonExtract(json, "weather");  // [{"id":500,"main":"Rain","description":"light rain","icon":"10d"}]
  Serial.print("weather_list:");
  Serial.println(weather_list);
  String weather_0 = jsonIndexList(weather_list, 0);   // {"id":500,"main":"Rain","description":"light rain","icon":"10d"}
  Serial.print("weather_0:");
  Serial.println(weather_0);
  String desc = jsonExtract(weather_0, "description"); // light rain
  Serial.printf("Location:%f,%f.  Current conditions: %s", lat, lon, desc.c_str());
}

void loop(){
}

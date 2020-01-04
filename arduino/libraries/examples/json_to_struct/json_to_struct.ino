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

struct WeatherStruct{
  float temp;
  float feels_like;
  float temp_min;
  float temp_max;
  float pressure;
  float humidity;
  float wind_speed;
  float wind_deg;
  float clouds_all;
  uint32_t sys_sunrise;
  uint32_t sys_sunset;
  uint32_t weather_id;
  String name;
  String weather_main;
  String weather_description;
  String weather_icon;
  uint32_t last_update;
};

struct WeatherStruct my_weather;

void print_weather(struct WeatherStruct weather){
  Serial.println("#WeatherStruct");
  Serial.println("{");
  Serial.print("    \"temp\":");Serial.print(weather.temp);Serial.println(",");
  Serial.print("    \"feels_like\":");Serial.print(weather.feels_like);Serial.println(",");
  Serial.print("    \"temp_min\":");Serial.print(weather.temp_min);Serial.println(",");
  Serial.print("    \"temp_max\":");Serial.print(weather.temp_max);Serial.println(",");
  Serial.print("    \"pressure\":");Serial.print(weather.pressure);Serial.println(",");
  Serial.print("    \"humidity\":");Serial.print(weather.humidity);Serial.println(",");
  Serial.print("    \"wind_speed\":");Serial.print(weather.wind_speed);Serial.println(",");
  Serial.print("    \"wind_deg\":");Serial.print(weather.wind_deg);Serial.println(",");
  Serial.print("    \"clouds_all\":");Serial.print(weather.clouds_all);Serial.println(",");
  Serial.print("    \"sys_sunrise\":");Serial.print(weather.sys_sunrise);Serial.println(",");
  Serial.print("    \"sys_sunset\":");Serial.print(weather.sys_sunset);Serial.println(",");
  Serial.print("    \"weather_id\":");Serial.println(weather.weather_id);
  Serial.print("    \"name\":\"");Serial.print(weather.name);Serial.println("\",");
  Serial.print("    \"weather_main\":\"");Serial.print(weather.weather_main);Serial.println("\",");
  Serial.print("    \"weather_description\":\"");Serial.print(weather.weather_description);Serial.println("\",");
  Serial.print("    \"weather_icon\":\"");Serial.print(weather.weather_icon);Serial.println("\",");
  Serial.print("    \"last_update\":");Serial.print(weather.last_update);Serial.println(",");
  Serial.println("}");
  
}

void get_weather(){
  String payload = json;
  
  String weather_0 = jsonIndexList(jsonExtract(payload, String("weather")), 0);
  String wind = jsonExtract(payload, "wind");
  String sys = jsonExtract(payload, "sys");
  
  my_weather.temp = jsonExtract(payload, "temp").toFloat();
  my_weather.feels_like = jsonExtract(payload, "feels_like").toFloat();
  my_weather.temp_min = jsonExtract(payload, "temp_min").toFloat();
  my_weather.temp_max = jsonExtract(payload, "temp_max").toFloat();
  my_weather.pressure = jsonExtract(payload, "pressure").toFloat();
  my_weather.humidity = jsonExtract(payload, "humidity").toFloat();
  my_weather.wind_speed = jsonExtract(wind, "speed").toFloat();
  my_weather.wind_deg = jsonExtract(wind, "deg").toFloat();
  my_weather.clouds_all = jsonExtract(jsonExtract(payload, "clouds"), "all").toFloat();
  if(abs(my_weather.clouds_all - 1.) < .1){
    my_weather.clouds_all = 100;
  }
  my_weather.sys_sunrise = jsonExtract(sys, "sunrise").toFloat();
  my_weather.sys_sunset = jsonExtract(sys, "sunset").toFloat();
  my_weather.name = jsonExtract(payload, "name");
  my_weather.weather_main = jsonExtract(weather_0, "main");
  my_weather.weather_description = jsonExtract(weather_0, "description");
  my_weather.weather_icon = jsonExtract(weather_0, "icon");
  my_weather.weather_id = jsonExtract(weather_0, "id").toInt();
  my_weather.last_update = millis();// place holder for example
}

void setup(){
  Serial.begin(115200);
  Serial.println("Simple json library offered by wyolum.com\n\n");
  get_weather();
  print_weather(my_weather);
}
void loop(){
}

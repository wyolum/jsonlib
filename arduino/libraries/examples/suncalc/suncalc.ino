/* 
Simple library for parsing simple JSON with two functions:

// pull named component out of the json String.
String jsonExtract(String json, String name);

//pull indexed component from the json list (in the String).
String jsonIndexList(String json, int idx);
*/

#include <jsonlib.h>
#include <ezTime.h>

String json = String(
"{\"solarNoon\":\"2020-01-03T17:14:48.837Z\",\"nadir\":\"2020-01-03T05:14:48.837Z\",\"sunrise\":\"2020-01-03T12:29:31.185Z\",\"sunset\":\"2020-01-03T22:00:06.489Z\",\"sunriseEnd\":\"2020-01-03T12:32:39.992Z\",\"sunsetStart\":\"2020-01-03T21:56:57.683Z\",\"dawn\":\"2020-01-03T11:59:43.140Z\",\"dusk\":\"2020-01-03T22:29:54.534Z\",\"nauticalDawn\":\"2020-01-03T11:26:23.018Z\",\"nauticalDusk\":\"2020-01-03T23:03:14.656Z\",\"nightEnd\":\"2020-01-03T10:54:01.500Z\",\"night\":\"2020-01-03T23:35:36.174Z\",\"goldenHourEnd\":\"2020-01-03T13:11:12.683Z\",\"goldenHour\":\"2020-01-03T21:18:24.991Z\",\"moonTimes\":{\"rise\":\"2020-01-03T17:26:44.710Z\"},\"moonIllum\":{\"fraction\":0.5428922767814602,\"phase\":0.26366983686256984,\"angle\":-1.968403051849827},\"tazel_points\":[[1578028488837,-3.1292999852871906,-1.291064982546701,1.5700401419047152,0.016782214155241504],[1578050088837,-1.2517860129854914,-0.24446859896637815,3.0634137397186194,-0.8554216508142396],[1578071688837,0.0027937417640065327,0.4924048128871014,-1.6659073112253968,-0.03511623611768218],[1578093288837,1.2555031839177042,-0.2479249137597645,-0.23490866588835393,0.946025491856983],[1578114888837,-3.1360633978288015,-1.2894239211569793,1.5256112908191726,0.19658423039451808]]}"
		     );

time_t parseTime(String timeString){
  tmElements_t tm;
  
  // check if we have a time string:
  if (timeString[10] == 'T' && timeString[23] == 'Z') {
    tm.Year = timeString.substring(0, 4).toInt() - 1970;
    tm.Month = timeString.substring(5, 7).toInt();
    tm.Day = timeString.substring(8, 10).toInt();
    tm.Hour = timeString.substring(11, 13).toInt();
    tm.Minute = timeString.substring(14, 16).toInt();
    tm.Second = timeString.substring(17, 19).toInt();
  }
  else{
    tm.Year = 0;
    tm.Month = 0; 
    tm.Day = 0;
    tm.Hour = 0;
    tm.Minute = 0;
    tm.Second = 0;
  }
  return makeTime(tm);
}

struct SunStruct{
  time_t solarNoon;
  time_t nadir;
  time_t sunrise;
  time_t sunset;
  time_t sunriseEnd;
  time_t sunsetStart;
  time_t dawn;
  time_t dusk;
  time_t nauticalDawn;
  time_t nauticalDusk;
  time_t nightEnd;
  time_t night;
  time_t goldenHourEnd;
  time_t goldenHour;
  time_t moonTimes_rise;
  time_t moonTimes_set;
  time_t times[5];
  double azels[5][4]; // sunaz, sunel, moonaz, moonel
  uint32_t last_update;
};

struct SunStruct my_sun;

void print_sun(struct SunStruct sun){
  Serial.println("#SunStruct");
  Serial.println("{");
  Serial.print("    \"solarNoon\":");Serial.print(sun.solarNoon);Serial.println(",");
  Serial.print("    \"sunrise\":");Serial.print(sun.sunrise);Serial.println(",");
  Serial.print("    \"sunset\":");Serial.print(sun.sunset);Serial.println(",");
  Serial.print("    \"sunriseEnd\":");Serial.print(sun.sunriseEnd);Serial.println(",");
  Serial.print("    \"sunsetStart\":");Serial.print(sun.sunsetStart);Serial.println(",");
  Serial.print("    \"dawn\":");Serial.print(sun.dawn);Serial.println(",");
  Serial.print("    \"dusk\":");Serial.print(sun.dusk);Serial.println(",");
  Serial.print("    \"nauticalDawn\":");Serial.print(sun.nauticalDawn);Serial.println(",");
  Serial.print("    \"nauticalDusk\":");Serial.print(sun.nauticalDusk);Serial.println(",");
  Serial.print("    \"night\":");Serial.print(sun.night);Serial.println(",");
  Serial.print("    \"goldenHourEnd\":");Serial.print(sun.goldenHourEnd);Serial.println(",");
  Serial.print("    \"goldenHour\":");Serial.print(sun.goldenHour);Serial.println(",");
  Serial.print("    \"moonTimes_rise\":");Serial.print(sun.moonTimes_rise);Serial.println(",");
  Serial.print("    \"moonTimes_set\":");Serial.print(sun.moonTimes_set);Serial.println(",");
  Serial.print("    \"last_update\":");Serial.print(sun.last_update);Serial.println(",");
  Serial.println("    \"tazel_points\":[");
  for(int ii=0; ii<5; ii++){
    Serial.printf("        [%d, %f, %f, %f, %f],\n",
		  sun.times[ii], sun.azels[ii][0],sun.azels[ii][1],sun.azels[ii][2],sun.azels[ii][3]);
  }
  Serial.println("    ]");
  Serial.println("}");
}

void get_sun(){
  String payload = json;
  String moonTimes = jsonExtract(payload, "moonTimes");

  my_sun.solarNoon = parseTime(jsonExtract(payload, "solarNoon"));
  my_sun.sunset = parseTime(jsonExtract(payload, "sunset"));
  my_sun.sunrise = parseTime(jsonExtract(payload, "sunrise"));
  my_sun.sunriseEnd = parseTime(jsonExtract(payload, "sunriseEnd"));
  my_sun.sunsetStart = parseTime(jsonExtract(payload, "sunsetStart"));
  my_sun.dawn = parseTime(jsonExtract(payload, "dawn"));
  my_sun.dusk = parseTime(jsonExtract(payload, "dusk"));
  my_sun.nauticalDawn = parseTime(jsonExtract(payload, "nauticalDawn"));
  my_sun.nauticalDusk = parseTime(jsonExtract(payload, "nauticalDusk"));
  my_sun.nightEnd = parseTime(jsonExtract(payload, "nightEnd"));
  my_sun.night = parseTime(jsonExtract(payload, "night"));
  my_sun.goldenHour = parseTime(jsonExtract(payload, "goldenHour"));
  my_sun.goldenHourEnd = parseTime(jsonExtract(payload, "goldenHourEnd"));
  my_sun.moonTimes_rise = parseTime(jsonExtract(moonTimes, "rise"));
  my_sun.moonTimes_set = parseTime(jsonExtract(moonTimes, "set"));

  String tazel_points = jsonExtract(payload, "tazel_points");
  for(int ii=0; ii<5; ii++){
    String tazel = jsonIndexList(tazel_points, ii);
    String tm = jsonIndexList(tazel, 0);
    my_sun.times[ii] = tm.substring(0, tm.length() - 3).toInt();
    my_sun.azels[ii][0] = jsonIndexList(tazel, 1).toFloat();
    my_sun.azels[ii][1] = jsonIndexList(tazel, 2).toFloat();
    my_sun.azels[ii][2] = jsonIndexList(tazel, 3).toFloat();
    my_sun.azels[ii][3] = jsonIndexList(tazel, 4).toFloat();
  }
  my_sun.last_update = millis(); // placeholder
}
void setup(){
  Serial.begin(115200);
  Serial.println("Simple json library offered by wyolum.com\n\n");
  get_sun();
  print_sun(my_sun);
}

void loop(){
}

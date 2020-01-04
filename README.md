# jsonlib
Simple JSON parsing library for arduino

jsonlib
=======
Simple JSON parsing library for arduino

## Usage example

```c++
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
```

## Reference

### jsonExtract

```c++
String jsonIndexList(String json, int idx);
```

Returns a String containing indexed item from provided String containing a json object.

```c++
String jsonExtract(String json, String name);
```

Returns a String containing the named item from provided String containing a json object.
Adds a custom time when the sun reaches the given angle to results returned by `SunCalc.getTimes`.

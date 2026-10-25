jsonlib
=======
Simple JSON parsing library for arduino.

Fork of [wyolum/jsonlib](https://github.com/wyolum/jsonlib) with string-aware parsing:
escaped quotes, commas and brackets inside string values no longer break key lookup
or list splitting, keys are matched only at the level of the given object and
whitespace is allowed anywhere between JSON tokens.

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
String jsonExtract(const String &json, const String &name);
```

Returns the value of key `name` from the JSON object in `json`. Only keys of this object
are searched, not keys of nested objects; to reach a nested value, extract the nested
object first:

```c++
jsonExtract(jsonExtract(json, "main"), "temp");
```

* string values are returned without quotes, escape sequences `\" \\ \/ \n \r \t \b \f`
  are converted, `\uXXXX` is kept as is (UTF-8 text is passed through unchanged),
* numbers, `true`, `false` and `null` are returned as written,
* objects and arrays are returned as JSON text, ready for another `jsonExtract` / `jsonIndexList`.

Returns an empty String if the key does not exist or `json` is not a JSON object.

### jsonIndexList

```c++
String jsonIndexList(const String &json, int idx);
```

Returns item `idx` (0-based) of the JSON array in `json`, in the same form as `jsonExtract`.
Returns an empty String if the index is out of range or `json` is not a JSON array.

## Changes from wyolum/jsonlib 0.1.3

* `jsonRemoveWhiteSpace` removed, whitespace is handled by the parser.
* `jsonExtract` matches keys only at the level of the given object (previously the first
  occurrence of `"name"` anywhere in the text was used, including nested objects and string values).
* `jsonIndexList` returns string items without quotes and converts escapes, like `jsonExtract`.
* `false` and `null` are returned complete (previously `fals` and `nul`).

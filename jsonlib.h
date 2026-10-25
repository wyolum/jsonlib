/*
 * jsonlib - Simple JSON parsing library for Arduino
 *
 * Original work: https://github.com/wyolum/jsonlib (Apache License 2.0)
 * Modified by jjirasek: string-aware parsing (escapes, commas and brackets
 * inside strings), whitespace tolerant, keys matched only at object level,
 * const String& parameters, jsonRemoveWhiteSpace removed.
 */

#ifndef JSONLIB_H
#define JSONLIB_H

#include <Arduino.h>

// Returns the value of key `name` in JSON object `json`.
// String values are returned without quotes, with escapes \" \\ \/ \n \r \t \b \f
// converted (\uXXXX is kept literally). Numbers, true/false/null are returned
// as written, objects and arrays as raw JSON text.
// Returns "" if the key does not exist or the input is not a valid object.
String jsonExtract(const String &json, const String &name);

// Returns item `idx` (0-based) of JSON array `json`, in the same form as jsonExtract.
// Returns "" if the index is out of range or the input is not a valid array.
String jsonIndexList(const String &json, int idx);

#endif

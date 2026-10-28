/*
 * jsonlib - Simple JSON parsing library for Arduino
 *
 * Original work: https://github.com/wyolum/jsonlib (Apache License 2.0)
 * Modified by jjirasek, see jsonlib.h.
 */

#include "jsonlib.h"

static bool isWs(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static size_t skipWs(const char *s, size_t i, size_t n) {
  while (i < n && isWs(s[i])) {
    i++;
  }
  return i;
}

// s[i] == '"'; returns index just after the closing quote (or n if unterminated)
static size_t skipString(const char *s, size_t i, size_t n) {
  i++;
  while (i < n) {
    if (s[i] == '\\') {
      i += 2;
    } else if (s[i] == '"') {
      return i + 1;
    } else {
      i++;
    }
  }
  return n;
}

// returns index just after the value starting at s[i]
static size_t skipValue(const char *s, size_t i, size_t n) {
  if (i >= n) {
    return n;
  }
  if (s[i] == '"') {
    return skipString(s, i, n);
  }
  if (s[i] == '{' || s[i] == '[') {
    int depth = 0;
    while (i < n) {
      char c = s[i];
      if (c == '"') {
        i = skipString(s, i, n);
        continue;
      }
      if (c == '{' || c == '[') {
        depth++;
      } else if (c == '}' || c == ']') {
        depth--;
        if (depth == 0) {
          return i + 1;
        }
      }
      i++;
    }
    return n;
  }
  // number, true, false, null
  while (i < n && s[i] != ',' && s[i] != '}' && s[i] != ']' && !isWs(s[i])) {
    i++;
  }
  return i;
}

static String valueToString(const String &json, size_t start, size_t end) {
  const char *s = json.c_str();
  if (start >= end) {
    return String();
  }
  if (s[start] != '"') {
    return json.substring(start, end);
  }
  // string: strip quotes, convert escapes
  size_t stop = end;
  if (stop > start + 1 && s[stop - 1] == '"') {
    stop--;
  }
  String out;
  out.reserve(stop - start);
  for (size_t i = start + 1; i < stop; i++) {
    char c = s[i];
    if (c == '\\' && i + 1 < stop) {
      char e = s[++i];
      switch (e) {
        case 'n': out += '\n'; break;
        case 'r': out += '\r'; break;
        case 't': out += '\t'; break;
        case 'b': out += '\b'; break;
        case 'f': out += '\f'; break;
        case 'u': out += '\\'; out += 'u'; break;   // \uXXXX kept literally
        default:  out += e;    break;               // \" \\ \/
      }
    } else {
      out += c;
    }
  }
  return out;
}

String jsonExtract(const String &json, const String &name) {
  const char *s = json.c_str();
  size_t n = json.length();
  size_t i = skipWs(s, 0, n);
  if (i >= n || s[i] != '{') {
    return String();
  }
  i++;
  while (true) {
    i = skipWs(s, i, n);
    if (i >= n || s[i] != '"') {
      return String();                    // '}' (end of object) or malformed
    }
    size_t keyStart = i + 1;
    i = skipString(s, i, n);
    size_t keyLen = (i > keyStart) ? i - 1 - keyStart : 0;
    i = skipWs(s, i, n);
    if (i >= n || s[i] != ':') {
      return String();
    }
    i = skipWs(s, i + 1, n);
    size_t valStart = i;
    size_t valEnd = skipValue(s, i, n);
    if (keyLen == name.length() && memcmp(s + keyStart, name.c_str(), keyLen) == 0) {
      return valueToString(json, valStart, valEnd);
    }
    i = skipWs(s, valEnd, n);
    if (i >= n || s[i] != ',') {
      return String();
    }
    i++;
  }
}

String jsonIndexList(const String &json, int idx) {
  const char *s = json.c_str();
  size_t n = json.length();
  if (idx < 0) {
    return String();
  }
  size_t i = skipWs(s, 0, n);
  if (i >= n || s[i] != '[') {
    return String();
  }
  i++;
  for (int k = 0; ; k++) {
    i = skipWs(s, i, n);
    if (i >= n || s[i] == ']') {
      return String();
    }
    size_t valStart = i;
    size_t valEnd = skipValue(s, i, n);
    if (k == idx) {
      return valueToString(json, valStart, valEnd);
    }
    i = skipWs(s, valEnd, n);
    if (i >= n || s[i] != ',') {
      return String();
    }
    i++;
  }
}

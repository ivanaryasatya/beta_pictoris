#pragma once
#include <Arduino.h>
bool parseCmd(String s, String &t, String &c, String &v) {
  if (!s.endsWith(";")) return false;
  s.remove(s.length() - 1);

  int p1 = s.indexOf('.');
  int p2 = s.indexOf('.', p1 + 1);

  if (p1 < 0 || p2 < 0) return false;

  t = s.substring(0, p1);
  c = s.substring(p1 + 1, p2);
  v = s.substring(p2 + 1);

  return true;
}
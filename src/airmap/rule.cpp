#include <airmap/rule.h>

bool airmap::operator==(const Rule& lhs, const Rule& rhs) {
  return lhs.type == rhs.type && lhs.id == rhs.id && lhs.name == rhs.name &&
         lhs.description == rhs.description && lhs.jurisdiction == rhs.jurisdiction;
}
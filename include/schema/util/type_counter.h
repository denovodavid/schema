#pragma once

/*
 Based on implementation by Tobias Stein
 source:
 https://github.com/tobias-stein/EntityComponentSystem/blob/master/EntityComponentSystem/include/ECS/util/FamilyTypeID.h
*/

namespace schema {
template <class T> class TypeCounter {
private:
  static size_t Count;

public:
  template <class U> static const size_t get() {
    static const size_t TypeId{Count++};
    return TypeId;
  }

  static const size_t get() { return Count; }
};

} // namespace schema

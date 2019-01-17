#pragma once

/*
 Based on implementation by Tobias Stein
 source: https://github.com/tobias-stein/EntityComponentSystem/blob/master/EntityComponentSystem/include/ECS/util/FamilyTypeID.h
*/

namespace schema {
    template <class T>
    class TypeCounter
    {
        static size_t count;

    public:
        template <class U>
        static const size_t Get()
        {
            static const size_t TYPE_ID { count++ };
            return TYPE_ID;
        }

        static const size_t Get()
        {
            return count;
        }
    };
}

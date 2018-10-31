#ifndef PART__H_
#define PART__H_

namespace PART {
enum TYPES_PARTS {
    front = 0,
    left = 1,
    back = 2,
    right = 3,
    front_left = 4,
    front_right = 5,
    back_right = 6,
    back_left = 7,
    none = 8,
    all = 9
};
}

typedef PART::TYPES_PARTS TYPE_PART;

#endif

#ifndef QUAT_H_
#define QUAT_H_

#include "vector.h"

typedef Vec4f Quat;

void quat(const Vec3f axis, float rad, Quat dest);

void quat_mul(const Quat q0, const Quat q1, Quat dest);

void quat_inverse(const Quat q, Quat dest);

void quat_rotate(const Quat q, const Vec3f v, Vec3f dest);

void quat_div(const Quat q0, const Quat q1, Quat dest);

void quat_slerp(const Quat q0, const Quat q1, float alpha, Quat dest);

#endif // QUAT_H_

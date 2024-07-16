#ifndef QUAT_H_
#define QUAT_H_

#include "vector.h"
#include <math.h>

typedef Vec4f Quat;

void quat(const Vec3f axis, float rad, Quat dest) {
    float i = axis[0] * sinf(rad / 2);
    float j = axis[1] * sinf(rad / 2);
    float k = axis[2] * sinf(rad / 2);
    float r = cosf(rad / 2);
    dest[0] = i; dest[1] = j; dest[2] = k; dest[3] = r;
}

void quat_mul(const Quat q0, const Quat q1, Quat dest) {
    // Vec3f v0 = {q0->x, q0->y, q0->z};
    // float w0 = q0->z;
    // Vec3f v1 = {q1->x, q1->y, q1->z};
    // float w1 = q1->z;
    //
    // dest->w = 0.0f;
    //
    // // ((v0 cross v1) + w0v1 + w1v0, w0*w1 - (v0 dot v1))
    //
    // Vec3f out_v = {0};
    //
    // vec3f_cross(&v0, &v1, &out_v);
    //
    // Vec3f w0_v1, w1_v0;
    // vec3f_scale(&v0, w1, &w1_v0);
    // vec3f_scale(&v1, w0, &w0_v1);
    //
    // vec3f_add(&out_v, &w0_v1, &out_v);
    // vec3f_add(&out_v, &w1_v0, &out_v);
    //
    // float v0_dot_v1 = vec3f_dot(&v0, &v1);
    //
    // dest->x = out_v.x;
    // dest->y = out_v.y;
    // dest->z = out_v.z;
    // dest->w = w0 * w1 - v0_dot_v1;

    dest[0] = q0[3] * q1[0] + q0[0] * q1[3] + q0[1] * q1[2] - q0[2] * q1[1];
    dest[1] = q0[3] * q1[1] - q0[0] * q1[2] + q0[1] * q1[3] + q0[2] * q1[0];
    dest[2] = q0[3] * q1[2] + q0[0] * q1[1] - q0[1] * q1[0] + q0[2] * q1[3];
    dest[3] = q0[3] * q1[3] - q0[0] * q1[0] - q0[1] * q1[1] - q0[2] * q1[2];
}

void quat_inverse(const Quat q, Quat dest) {
    dest[0] = -q[0];
    dest[1] = -q[1];
    dest[2] = -q[2];
    dest[3] = q[3];
}

void quat_rotate(const Quat q, const Vec3f v, Vec3f dest) {
    Quat q_inv; quat_inverse(q, q_inv);
    Quat q_v = { v[0], v[1], v[2], 0.0f };
    Quat out = {0};
    quat_mul(q, q_v, out);
    quat_mul(out, q_inv, out);
    
    dest[0] = out[0];
    dest[1] = out[1];
    dest[2] = out[2];
}

void quat_div(const Quat q0, const Quat q1, Quat dest) {
    Quat q1_inv; quat_inverse(q1, q1_inv);
    quat_mul(q0, q1_inv, dest);
}

// void quat_slerp(const Quat* q0, const Quat* q1, float alpha, Quat* dest) {
//     Quat q_r; quat_div(q1, q0, &q_r);
//     if (q_r.w < 0.0f) {
//         q_r.x *= -1.0f;
//         q_r.y *= -1.0f;
//         q_r.z *= -1.0f;
//         q_r.w *= -1.0f;
//     }
//     Vec3f v_r = vec3f(q_r.x, q_r.y, q_r.z);
//     float v_rnorm = vec3f_norm(&v_r);
//     float theta_r = 2.0f * atanf(v_rnorm / q_r.w);
//
//     float v_rnorm_r = 1 / v_rnorm;
//     v_r.x *= v_rnorm_r;
//     v_r.y *= v_rnorm_r;
//     v_r.z *= v_rnorm_r;
//
//     float theta_alpha = alpha * theta_r;
//     float sin_alpha = sinf(theta_alpha / 2.0f);
//     float cos_alpha = cosf(theta_alpha / 2.0f);
//
//     Quat q_alpha = {sin_alpha * v_r.x, sin_alpha * v_r.y, sin_alpha * v_r.z, cosf(theta_alpha)};
//
//     quat_mul(&q_alpha, q0, dest);
// }


#endif // QUAT_H_

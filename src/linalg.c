#include "linalg.h"

#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "math.h"

float degrees_to_rad(float degrees) { return degrees * (PI / 180.0f); }

// Vector

void vec3f_print(const Vec3f v) {
    printf("%.2f\t%.2f\t%.2f\n", v[0], v[1], v[2]);
}

void vec3f_clone(const Vec3f src, Vec3f dest) {
    memcpy(dest, src, 3 * sizeof(dest[0]));
}

void vec3f_add(const Vec3f a, const Vec3f b, Vec3f d) {
    d[0] = a[0] + b[0];
    d[1] = a[1] + b[1];
    d[2] = a[2] + b[2];
}

void vec3f_sub(const Vec3f a, const Vec3f b, Vec3f d) {
    d[0] = a[0] - b[0];
    d[1] = a[1] - b[1];
    d[2] = a[2] - b[2];
}

void vec3f_scale(Vec3f a, float s, Vec3f d) {
    d[0] = a[0] * s;
    d[1] = a[1] * s;
    d[2] = a[2] * s;
}

float vec3f_norm(Vec3f v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vec3f_normalize(Vec3f v) {
    float norm_r = 1.0f / vec3f_norm(v);
    v[0] *= norm_r;
    v[1] *= norm_r;
    v[2] *= norm_r;
}

void vec3f_cross(const Vec3f v1, const Vec3f v2, Vec3f dest) {
    dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
    dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
    dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float vec3f_dot(const Vec3f v1, const Vec3f v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void vec4f_print(const Vec4f v) {
    printf("%.2f\t%.2f\t%.2f\t%.2f\n", v[0], v[1], v[2], v[3]);
}

void vec4f_clone(const Vec4f src, Vec4f dest) {
    memcpy(dest, src, 4 * sizeof(dest[0]));
}

float vec4f_norm(Vec4f v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

void vec4f_normalize(Vec4f v) {
    float norm_r = 1.0f / vec4f_norm(v);
    v[0] *= norm_r;
    v[1] *= norm_r;
    v[2] *= norm_r;
    v[3] *= norm_r;
}

// Matrix

void mat4x4f_print(const Mat4x4f m) {
    for (size_t y = 0; y < 4; ++y) {
        for (size_t x = 0; x < 4; ++x) {
            printf("%.2f\t", m[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}

void mat4x4f_set_zeroes(Mat4x4f dest) {
    for (size_t y = 0; y < 4U; ++y) {
        for (size_t x = 0; x < 4U; ++x) {
            dest[y][x] = 0.0f;
        }
    }
}

void mat4x4f_set_identity(Mat4x4f dest) {
    mat4x4f_set_zeroes(dest);
    for (size_t i = 0; i < 4U; ++i) {
        dest[i][i] = 1.0f;
    }
}

void mat4x4f_scale(Mat4x4f m1, const Vec3f scale) {
    m1[0][0] *= scale[0];
    m1[1][1] *= scale[1];
    m1[2][2] *= scale[2];
}

void mat4x4f_mul(const Mat4x4f m1, const Mat4x4f m2, Mat4x4f d) {
    // todo: don't load unaligned here once allocator alignt things
    __m128 row1 = _mm_loadu_ps(m2[0]);
    __m128 row2 = _mm_loadu_ps(m2[1]);
    __m128 row3 = _mm_loadu_ps(m2[2]);
    __m128 row4 = _mm_loadu_ps(m2[3]);
    for (int i = 0; i < 4; i++) {
        __m128 brod1 = _mm_set1_ps(m1[i][0]);
        __m128 brod2 = _mm_set1_ps(m1[i][1]);
        __m128 brod3 = _mm_set1_ps(m1[i][2]);
        __m128 brod4 = _mm_set1_ps(m1[i][3]);
        __m128 row = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(brod1, row1), _mm_mul_ps(brod2, row2)),
            _mm_add_ps(_mm_mul_ps(brod3, row3), _mm_mul_ps(brod4, row4)));
        // todo: don't store unaligned here once allocator alignt things
        _mm_storeu_ps(d[i], row);
    }
}

void mat4x4f_translate(Mat4x4f m, const Vec3f t) {
    m[0][3] += t[0];
    m[1][3] += t[1];
    m[2][3] += t[2];
}

void mat4x4f_rotate_q(Mat4x4f m, const Quat rot) {
    float i = rot[0];
    float j = rot[1];
    float k = rot[2];
    float r = rot[3];

    Mat4x4f out = MAT4X4F_IDENTITY_INITIALIZER;

    float ii = i * i;
    float jj = j * j;
    float kk = k * k;
    float ij = i * j;
    float jr = j * r;
    float kr = k * r;
    float ik = i * k;
    float jk = j * k;
    float ir = i * r;

    out[0][0] = 1.0f - 2.0f * (jj + kk);
    out[0][1] = 2.0f * (ij - kr);
    out[0][2] = 2.0f * (ik + jr);
    out[1][0] = 2.0f * (ij + kr);
    out[1][1] = 1.0f - 2.0f * (ii + kk);
    out[1][2] = 2.0f * (jk - ir);
    out[2][0] = 2.0f * (ik - jr);
    out[2][1] = 2.0f * (jk + ir);
    out[2][2] = 1.0f - 2.0f * (ii + jj);

    mat4x4f_mul(out, m, m);
}

void mat4x4f_rotate(Mat4x4f m, const Vec3f axis, float rad) {
    Vec3f axis_clone = {axis[0], axis[1], axis[2]};
    vec3f_normalize(axis_clone);
    Quat r = {0};
    quat(axis, rad, r);
    mat4x4f_rotate_q(m, r);
}

void mat4x4f_projection_scaled_ortho(float left, float right, float bottom,
                                     float top, float z_near, float z_far,
                                     Mat4x4f dest) {
    mat4x4f_set_zeroes(dest);
    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = 1.0f / (z_far - z_near);

    dest[0][0] = 2.0f * rl;
    dest[1][1] = 2.0f * tb;
    dest[2][2] = 2.0f * fn;

    dest[0][3] = -(right + left) * rl;
    dest[1][3] = -(top + bottom) * tb;
    dest[2][3] = (z_far + z_near) * fn;
    dest[3][3] = 1.0f;
}

void mat4x4f_projection_perspective(float fovy, float aspect, float z_near,
                                    float z_far, Mat4x4f dest) {
    float f = 1.0f / tanf(fovy / 2.0f);
    float z_range = z_near - z_far;

    mat4x4f_set_zeroes(dest);

    dest[0][0] = f / aspect;
    dest[1][1] = f;
    dest[2][2] = (z_near + z_far) / z_range;
    dest[2][3] = (2.0f * z_near * z_far) / z_range;
    dest[3][2] = -1.0f;
    dest[3][3] = 0.0f;
}

void mat4x4f_lookat(const Vec3f eye, const Vec3f center, const Vec3f up,
                    Mat4x4f dest) {
    mat4x4f_set_zeroes(dest);
    Vec3f f, s, u;
    vec3f_sub(center, eye, f);
    vec3f_normalize(f);

    vec3f_cross(f, up, s);
    vec3f_normalize(s);

    vec3f_cross(s, f, u);

    dest[0][0] = s[0];
    dest[0][1] = s[1];
    dest[0][2] = s[2];
    dest[0][3] = -vec3f_dot(s, eye);

    dest[1][0] = u[0];
    dest[1][1] = u[1];
    dest[1][2] = u[2];
    dest[1][3] = -vec3f_dot(u, eye);

    dest[2][0] = -f[0];
    dest[2][1] = -f[1];
    dest[2][2] = -f[2];
    dest[2][3] = vec3f_dot(f, eye);

    dest[3][3] = 1.0f;
}

void quat(const Vec3f axis, float rad, Quat dest) {
    float i = axis[0] * sinf(rad / 2);
    float j = axis[1] * sinf(rad / 2);
    float k = axis[2] * sinf(rad / 2);
    float r = cosf(rad / 2);
    dest[0] = i;
    dest[1] = j;
    dest[2] = k;
    dest[3] = r;
}

void quat_mul(const Quat q0, const Quat q1, Quat dest) {
    dest[0] = q0[3] * q1[0] + q0[0] * q1[3] + q0[1] * q1[2] - q0[2] * q1[1];
    dest[1] = q0[3] * q1[1] - q0[0] * q1[2] + q0[1] * q1[3] + q0[2] * q1[0];
    dest[2] = q0[3] * q1[2] + q0[0] * q1[1] - q0[1] * q1[0] + q0[2] * q1[3];
    dest[3] = q0[3] * q1[3] - q0[0] * q1[0] - q0[1] * q1[1] - q0[2] * q1[2];
}

void quat_inverse(const Quat q, Quat dest) {
    dest[0] = q[0];
    dest[1] = q[1];
    dest[2] = q[2];
    dest[3] = -q[3];
}

void quat_rotate(const Quat q, const Vec3f v, Vec3f dest) {
    Quat q_inv;
    quat_inverse(q, q_inv);
    Quat q_v = {v[0], v[1], v[2], 0.0f};
    Quat out = {0};
    quat_mul(q, q_v, out);
    quat_mul(out, q_inv, out);

    dest[0] = out[0];
    dest[1] = out[1];
    dest[2] = out[2];
}

void quat_div(const Quat q0, const Quat q1, Quat dest) {
    Quat q1_inv;
    quat_inverse(q1, q1_inv);
    quat_mul(q0, q1_inv, dest);
}

void quat_slerp(const Quat q0, const Quat q1, float alpha, Quat dest) {
    Quat q_r;
    quat_div(q1, q0, q_r);
    if (q_r[3] < 0.0f) {
        q_r[0] *= -1.0f;
        q_r[1] *= -1.0f;
        q_r[2] *= -1.0f;
        q_r[3] *= -1.0f;
    }
    Vec3f v_r = {q_r[0], q_r[1], q_r[2]};
    float v_rnorm = vec3f_norm(v_r);
    float theta_r = 2.0f * atanf(v_rnorm / q_r[3]);

    if (v_rnorm > 0.0f) {
        float v_rnorm_r = 1.0f / v_rnorm;
        v_r[0] *= v_rnorm_r;
        v_r[1] *= v_rnorm_r;
        v_r[2] *= v_rnorm_r;
    }

    float theta_alpha = alpha * theta_r;
    float sin_alpha = sinf(theta_alpha / 2.0f);
    float cos_alpha = cosf(theta_alpha / 2.0f);

    Quat q_alpha = {sin_alpha * v_r[0], sin_alpha * v_r[1], sin_alpha * v_r[2],
                    cos_alpha};

    quat_mul(q_alpha, q0, dest);
}

float lerp(float a, float b, float t) { return a * (1.0f - t) + b * t; }

float quadratic_bezier(float a, float b, float c, float t) {
    float t_ab = lerp(a, b, t);
    float t_bc = lerp(b, c, t);
    float t_abc = lerp(t_ab, t_bc, t);
    return t_abc;
}

float cubic_bezier(float a, float b, float c, float d, float t) {
    float t_ab = lerp(a, b, t);
    float t_bc = lerp(b, c, t);
    float t_cd = lerp(c, d, t);
    float t_abc = lerp(t_ab, t_bc, t);
    float t_bcd = lerp(t_bc, t_cd, t);
    float t_abcd = lerp(t_abc, t_bcd, t);
    return t_abcd;
}

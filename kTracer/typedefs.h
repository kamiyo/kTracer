#pragma once

#include <memory>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/Geometry>
#include <OpenEXR\ImfRgba.h>
#include <cmath>
#include <ctime>
#include <limits>

#define _USE_MATH_DEFINES

#define INF std::numeric_limits<double>::infinity()
#define nINF -1 * std::numeric_limits<double>::infinity()
#define EULER 2.718281828459045235360287471352
#define EPSILON 0.0001
#define M_1_PI 0.31830988618379067154 // 1/pi

typedef Eigen::Vector2d Vector2d;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Matrix3d Matrix3d;
typedef Eigen::Matrix4d Matrix4d;
typedef Eigen::Vector3d Vector3d;
typedef Eigen::Array3d Rgb;
typedef Eigen::Array4d Rgba;
typedef Eigen::Vector3i Vector3i;
typedef Eigen::Vector4d Vector4d;
typedef Eigen::AngleAxisd AngleAxisd;
typedef Eigen::Array2d Array2d;
typedef Eigen::Array4d Array4d;
typedef Eigen::Array33d Array33d;
typedef Eigen::Matrix<Vector2d, Eigen::Dynamic, Eigen::Dynamic> Sampler2d;
typedef Eigen::Matrix<Vector2i, Eigen::Dynamic, Eigen::Dynamic> Sampler2i;
typedef Eigen::Matrix<Imf::Rgba, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixRgba;
typedef Eigen::Affine3d Transform3d;
typedef Eigen::AffineCompact2d Transform2d;

template <typename Object>
using u_ptr = std::unique_ptr<Object>;

template <typename Object>
using s_ptr = std::shared_ptr<Object>;

template <typename Object>
using w_ptr = std::weak_ptr<Object>;

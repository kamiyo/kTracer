#pragma once

#include <memory>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/Geometry>
#include <OpenEXR\ImfRgba.h>
#include <ctime>
#include <limits>

#define INF std::numeric_limits<double>::infinity()
#define nINF -1 * std::numeric_limits<double>::infinity()
#define EULER 2.718281828459045235360287471352
#define EPSILON 0.0001
#define M_PI 3.14159265358979323846
#define M_1_PI 0.31830988618379067154 // 1/pi
#define INF4DPOINT Vector4d(INF, INF, INF, 1)
#define nINF4DPOINT Vector4d(nINF, nINF, nINF, 1)
#define INF4DDIR Vector4d(INF, INF, INF, 0)
#define nINF4DDIR Vector4d(nINF, nINF, nINF, 0)

typedef Eigen::Vector2d Vector2d;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Matrix<unsigned int, 2, 1> Vector2ui;
typedef Eigen::Matrix3d Matrix3d;
typedef Eigen::Matrix4d Matrix4d;
typedef Eigen::Vector3d Vector3d;
typedef Eigen::Array3d Rgb;
typedef Eigen::Array4d Rgba;
typedef Eigen::Vector3i Vector3i;
typedef Eigen::Vector4d Vector4d;
typedef Eigen::Vector4i Vector4i;
typedef Eigen::AngleAxisd AngleAxisd;
typedef Eigen::Array2d Array2d;
typedef Eigen::Array4d Array4d;
typedef Eigen::Array4i Array4i;
typedef Eigen::Array33d Array33d;
typedef Eigen::MatrixXd MatrixXd;
typedef Eigen::ArrayXi ArrayXi;
typedef Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic> Sampler1d;
typedef Eigen::Array<Vector2d, Eigen::Dynamic, Eigen::Dynamic> Sampler2d;
typedef Eigen::Array<Vector2i, Eigen::Dynamic, Eigen::Dynamic> Sampler2i;
typedef Eigen::Array<Imf::Rgba, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixRgba;
typedef Eigen::Affine3d Transform3d;
typedef Eigen::AffineCompact2d Transform2d;

template <typename Object>
using u_ptr = std::unique_ptr<Object>;

template <typename Object>
using s_ptr = std::shared_ptr<Object>;

template <typename Object>
using w_ptr = std::weak_ptr<Object>;

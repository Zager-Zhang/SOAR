#include <plan_env/edt_environment.h>
#include <plan_env/sdf_map.h>

namespace hetero_planner {
/* ============================== edt_environment ==============================
 */
void EDTEnvironment::init()
{
}

void EDTEnvironment::setMap(shared_ptr<SDFMap>& map)
{
  this->sdf_map_ = map;
  resolution_inv_ = 1 / sdf_map_->getResolution();
}

void EDTEnvironment::getSurroundDistance(Eigen::Vector3d pts[2][2][2], double dists[2][2][2])
{
  for (int x = 0; x < 2; x++)
    for (int y = 0; y < 2; y++)
      for (int z = 0; z < 2; z++) dists[x][y][z] = sdf_map_->getDistance(pts[x][y][z]);
}

void EDTEnvironment::interpolateTrilinear(
    double values[2][2][2], const Eigen::Vector3d& diff, double& value, Eigen::Vector3d& grad)
{
  // trilinear interpolation
  double v00 = (1 - diff(0)) * values[0][0][0] + diff(0) * values[1][0][0];
  double v01 = (1 - diff(0)) * values[0][0][1] + diff(0) * values[1][0][1];
  double v10 = (1 - diff(0)) * values[0][1][0] + diff(0) * values[1][1][0];
  double v11 = (1 - diff(0)) * values[0][1][1] + diff(0) * values[1][1][1];
  double v0 = (1 - diff(1)) * v00 + diff(1) * v10;
  double v1 = (1 - diff(1)) * v01 + diff(1) * v11;

  value = (1 - diff(2)) * v0 + diff(2) * v1;

  grad[2] = (v1 - v0) * resolution_inv_;
  grad[1] = ((1 - diff[2]) * (v10 - v00) + diff[2] * (v11 - v01)) * resolution_inv_;
  grad[0] = (1 - diff[2]) * (1 - diff[1]) * (values[1][0][0] - values[0][0][0]);
  grad[0] += (1 - diff[2]) * diff[1] * (values[1][1][0] - values[0][1][0]);
  grad[0] += diff[2] * (1 - diff[1]) * (values[1][0][1] - values[0][0][1]);
  grad[0] += diff[2] * diff[1] * (values[1][1][1] - values[0][1][1]);
  grad[0] *= resolution_inv_;
}

void EDTEnvironment::evaluateEDTWithGrad(
    const Eigen::Vector3d& pos, double time, double& dist, Eigen::Vector3d& grad)
{
  // Eigen::Vector3d diff;
  // Eigen::Vector3d sur_pts[2][2][2];
  // sdf_map_->getSurroundPts(pos, sur_pts, diff);
  // double dists[2][2][2];
  // getSurroundDistance(sur_pts, dists);
  // interpolateTrilinear(dists, diff, dist, grad);
  dist = sdf_map_->getDistWithGrad(pos, grad);
}

// EDTEnvironment::
}  // namespace hetero_planner
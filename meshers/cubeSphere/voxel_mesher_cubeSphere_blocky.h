#ifndef VOXEL_MESHER_CUBE_SPHERE_BLOCKY_H
#define VOXEL_MESHER_CUBE_SPHERE_BLOCKY_H


#include "../blocky/voxel_mesher_blocky.h"

#include <vector>

namespace zylann::voxel {

// Interprets voxel values as indexes to models in a VoxelBlockyLibrary, and batches them together.
// Overlapping faces are removed from the final mesh.
class VoxelMesherCubeSphereBlocky : public VoxelMesherBlocky {
	GDCLASS(VoxelMesherCubeSphereBlocky, VoxelMesherBlocky)

public:
	static const int PADDING = 1;

	VoxelMesherCubeSphereBlocky();
	~VoxelMesherCubeSphereBlocky();

	void build(VoxelMesher::Output &output, const VoxelMesher::Input &input) override;
	Ref<Resource> duplicate(bool p_subresources = false) const ZN_OVERRIDE_UNLESS_GODOT_EXTENSION;

protected:

	void set_curvature(int newRes);
	int get_curvature();

	static void _bind_methods();

private:
	int curveRes;


};

} // namespace zylann::voxel

#endif // VOXEL_MESHER_CUBE_SPHERE_BLOCKY_H

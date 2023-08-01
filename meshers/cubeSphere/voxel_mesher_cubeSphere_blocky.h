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
	// TODO GDX: Resource::duplicate() cannot be overriden (while it can in modules).
	// This will lead to performance degradation and maybe unexpected behavior
#if defined(ZN_GODOT)
	Ref<Resource> duplicate(bool p_subresources = false) const override;
#elif defined(ZN_GODOT_EXTENSION)
	Ref<Resource> duplicate(bool p_subresources = false) const;
#endif
protected:

	void set_curvature(int newRes);
	int get_curvature();

	static void _bind_methods();

private:
	int curveRes;


};

} // namespace zylann::voxel

#endif // VOXEL_MESHER_CUBE_SPHERE_BLOCKY_H

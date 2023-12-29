#include "voxel_generator_image.h"
#include "../../util/containers/fixed_array.h"
#include "../../util/containers/span.h"
#include "../../util/math/conv.h"

namespace zylann::voxel {

namespace {

inline float get_height_repeat(const Image &im, int x, int y) {
	return im.get_pixel(math::wrap(x, im.get_width()), math::wrap(y, im.get_height())).r;
}

inline float get_height_blurred(const Image &im, int x, int y) {
	float h = get_height_repeat(im, x, y);
	h += get_height_repeat(im, x + 1, y);
	h += get_height_repeat(im, x - 1, y);
	h += get_height_repeat(im, x, y + 1);
	h += get_height_repeat(im, x, y - 1);
	return h * 0.2f;
}

} // namespace

VoxelGeneratorImage::VoxelGeneratorImage() {}

VoxelGeneratorImage::~VoxelGeneratorImage() {}

void VoxelGeneratorImage::set_image(Ref<Image> im) {
	if (im == _image) {
		return;
	}
	if (im.is_valid()) {
		ERR_FAIL_COND(im->is_compressed());
	}
	_image = im;
	Ref<Image> copy;
	if (im.is_valid()) {
		copy = im->duplicate();
	}
	RWLockWrite wlock(_parameters_lock);
	_parameters.image = copy;
}

Ref<Image> VoxelGeneratorImage::get_image() const {
	return _image;
}

void VoxelGeneratorImage::set_blur_enabled(bool enable) {
	RWLockWrite wlock(_parameters_lock);
	_parameters.blur_enabled = enable;
}

bool VoxelGeneratorImage::is_blur_enabled() const {
	RWLockRead rlock(_parameters_lock);
	return _parameters.blur_enabled;
}

void VoxelGeneratorImage::set_mapping_enabled(bool enable) {
	RWLockWrite wlock(_parameters_lock);
	_parameters.mapping_enabled = enable;
}

bool VoxelGeneratorImage::is_mapping_enabled() const {
	RWLockRead rlock(_parameters_lock);
	return _parameters.mapping_enabled;
}

void VoxelGeneratorImage::set_mapping_min(Vector2 min_mapping) {
	RWLockWrite wlock(_parameters_lock);
	_parameters.mapping_min = min_mapping;
}

Vector2 VoxelGeneratorImage::get_mapping_min() const {
	RWLockRead rlock(_parameters_lock);
	return _parameters.mapping_min;
}

void VoxelGeneratorImage::set_mapping_max(Vector2 max_mapping) {
	RWLockWrite wlock(_parameters_lock);
	_parameters.mapping_max = max_mapping;
}

Vector2 VoxelGeneratorImage::get_mapping_max() const {
	RWLockRead rlock(_parameters_lock);
	return _parameters.mapping_max;
}

VoxelGenerator::Result VoxelGeneratorImage::generate_block(VoxelGenerator::VoxelQueryData &input) {
	VoxelBufferInternal &out_buffer = input.voxel_buffer;

	Parameters params;
	{
		RWLockRead rlock(_parameters_lock);
		params = _parameters;
	}

	Result result;

	ERR_FAIL_COND_V(params.image.is_null(), result);
	const Image &image = **params.image;
	Vector2 max_mapping = params.mapping_max;

	if (params.blur_enabled) {
		result = VoxelGeneratorHeightmap::generate(
				out_buffer, [&image](int x, int z) { return get_height_blurred(image, x, z); }, input.origin_in_voxels,
				input.lod);
	} else if (params.mapping_enabled) {
		result = VoxelGeneratorHeightmap::generate(
				out_buffer,
				[&image, &max_mapping](int x, int z) {
					int pixWidth = image.get_width();
					int pixHeight = image.get_height();

					double relativePosX = (double)x / max_mapping.x;
					int xPix = (int)round(relativePosX * (pixWidth - 1));

					double relativePosZ = (double)z / max_mapping.y;
					int zPix = (int)round(relativePosZ * (pixHeight - 1));

					return get_height_repeat(image, xPix, zPix);
				},
				input.origin_in_voxels, input.lod);
	} else {
		result = VoxelGeneratorHeightmap::generate(
				out_buffer, [&image](int x, int z) { return get_height_repeat(image, x, z); }, input.origin_in_voxels,
				input.lod);
	}

	out_buffer.compress_uniform_channels();
	return result;
}

void VoxelGeneratorImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_image", "image"), &VoxelGeneratorImage::set_image);
	ClassDB::bind_method(D_METHOD("get_image"), &VoxelGeneratorImage::get_image);

	ClassDB::bind_method(D_METHOD("set_blur_enabled", "enable"), &VoxelGeneratorImage::set_blur_enabled);
	ClassDB::bind_method(D_METHOD("is_blur_enabled"), &VoxelGeneratorImage::is_blur_enabled);

	ClassDB::bind_method(D_METHOD("set_mapping_enabled", "enable"), &VoxelGeneratorImage::set_mapping_enabled);
	ClassDB::bind_method(D_METHOD("is_mapping_enabled"), &VoxelGeneratorImage::is_mapping_enabled);

	ClassDB::bind_method(D_METHOD("set_mapping_min", "min_mapping"), &VoxelGeneratorImage::set_mapping_min);
	ClassDB::bind_method(D_METHOD("get_mapping_min"), &VoxelGeneratorImage::get_mapping_min);

	ClassDB::bind_method(D_METHOD("set_mapping_max", "max_mapping"), &VoxelGeneratorImage::set_mapping_max);
	ClassDB::bind_method(D_METHOD("get_mapping_max"), &VoxelGeneratorImage::get_mapping_max);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, Image::get_class_static()),
			"set_image", "get_image");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "blur_enabled"), "set_blur_enabled", "is_blur_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mapping_enabled"), "set_mapping_enabled", "is_mapping_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "min_mapping"), "set_mapping_min", "get_mapping_min");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "max_mapping"), "set_mapping_max", "get_mapping_max");
}

} // namespace zylann::voxel

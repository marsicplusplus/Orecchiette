#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

enum class MaterialType {
	Emissive,
};

class Material {
	public:
		Material() {}
		virtual MaterialType getType() const = 0;
};

#endif // __MATERIAL_HPP__

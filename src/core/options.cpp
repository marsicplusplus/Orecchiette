#include "core/options.hpp"

Options::Options() {
	width = 512;
	height = 512;
	tileH = 8;
	tileW = 8;
	title = "Orecchiette";

	spp = 1;
}

Options::Options(std::string fp) {
	Options();
}

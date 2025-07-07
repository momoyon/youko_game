#include <tile.h>
#include <config.h>

Tile load_tile_from_sheet(Texture2D sheet, Vector2 id) {
	if (sheet.width % TILE_SIZE != 0) {
		log_error("Sheet's width is not divisible by TILE_SIZE: %d / %d != 0", sheet.width, TILE_SIZE);
		exit(1);
	}
	if (sheet.height % TILE_SIZE != 0) {
		log_error("Sheet's height is not divisible by TILE_SIZE: %d / %d != 0", sheet.height, TILE_SIZE);
		exit(1);
	}

	Vector2 offset = Vector2Multiply(v2xx(TILE_SIZE), id);
	if (offset.x > sheet.width) {
		log_error("id.x is out of bounds from sheet!: %.0f(%.0f) > %d", offset.x, id.x, sheet.width);
		exit(1);
	}
	if (offset.y > sheet.height) {
		log_error("id.y is out of bounds from sheet!: %.0f(%.0f) > %d", offset.y, id.y, sheet.height);
		exit(1);
	}

	Rectangle tex_rect = {
		.x = offset.x,
		.y = offset.y,
		.width = TILE_SIZE,
		.height = TILE_SIZE
	};

	Tile t = {
		.tex = sheet,
		.tex_rect = tex_rect
	};

	return t;
}

void draw_tile(Tile *tile) {
	
}

void draw_tiles(Tiles *tiles) {

}

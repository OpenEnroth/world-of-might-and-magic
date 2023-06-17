#pragma once

#include <span>
#include <array>
#include <vector>

#include "Library/Image/Palette.h"

class LODFile_IconsBitmaps;

class PaletteManager {
 public:
    void load(LODFile_IconsBitmaps *lod);

    /**
     * @param paletteId                 Palette identifier, a number in [0, 999].
     * @return                          Index for the provided palette identifier. Returned index can then be used
     *                                  for getting palette data from the return value of `paletteData` function.
     */
    int paletteIndex(int paletteId);

    /**
     * @return                          Span containing contiguous data for all loaded palettes.
     */
    std::span<Color> paletteData();

 private:
    static Palette createGrayscalePalette();
    static Palette createLoadedPalette(uint8_t *data);

 private:
    std::vector<int> _paletteIds;
    std::vector<Palette> _palettes;
};

extern PaletteManager *pPaletteManager;

// reused from unvsfz2

#ifndef FREE_TYPE_LIB_HPP
# define FREE_TYPE_LIB_HPP

#include <string>
#include <functional>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "claws/container/vect.hpp"
#include <iostream>

class Display;

class FreeTypeLib
{
private:
  FT_Library library;
  FT_Face face;
  char badPadding[8];
public:
  FreeTypeLib(std::string fontFile);
  FreeTypeLib(FreeTypeLib const &) = delete;
  ~FreeTypeLib();

  template<class A>
  void renderText(std::string text, A renderBuffer, unsigned int size, claws::vect<float, 2u> step)
  {
    FT_Set_Pixel_Sizes(face, 0, size);
    claws::vect<float, 2u> pen(0.0f, 0.0f);
    step[1] *= -1;

    for (auto c : text)
      {
	FT_GlyphSlot &slot(face->glyph);
	FT_Load_Char(face, static_cast<FT_ULong>(c), FT_LOAD_RENDER);
    	renderBuffer((pen + claws::vect<float, 2u>{float(slot->bitmap_left), -float(slot->bitmap_top)}) * step / float(size),
		     claws::vect<float, 2u>{float(slot->bitmap.pitch), float(slot->bitmap.rows)} * step / float(size),
		     slot->bitmap.buffer, {int(slot->bitmap.pitch), int(slot->bitmap.rows)});
	pen[0] += float(face->glyph->advance.x >> 6u);
	pen[1] += float(face->glyph->advance.y >> 6u);
      }
  }
};

#endif

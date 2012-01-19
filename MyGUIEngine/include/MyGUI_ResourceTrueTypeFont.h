/*!
	@file
	@author		Albert Semenov
	@date		06/2009
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_RESOURCE_TRUE_TYPE_FONT_H__
#define __MYGUI_RESOURCE_TRUE_TYPE_FONT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IFont.h"

#ifdef MYGUI_USE_FREETYPE
	#include <ft2build.h>
	#include FT_FREETYPE_H
#endif // MYGUI_USE_FREETYPE

namespace MyGUI
{

	class MYGUI_EXPORT ResourceTrueTypeFont :
		public IFont
	{
		MYGUI_RTTI_DERIVED( ResourceTrueTypeFont )

	public:
		ResourceTrueTypeFont();
		virtual ~ResourceTrueTypeFont();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		// Returns the glyph info for the specified code point, or the "Not Defined" glyph if the code point does not exist in this font.
		virtual GlyphInfo* getGlyphInfo(Char _id);

		virtual ITexture* getTextureFont();

		// получившаяся высота при генерации в пикселях
		virtual int getDefaultHeight();

	private:
		void addCodePoint(Char _codePoint);
		void removeCodePoint(Char _codePoint);

		void addCodePointRange(Char _first, Char _second);
		void removeCodePointRange(Char _first, Char _second);

		void clearCodePoints();

		void initialise();

		// The following variables are set directly from values specified by the user.
		std::string mSource; // Source (filename) of the font.
		float mSize; // Size of the font, in points (there are 72 points per inch).
		uint mResolution; // Resolution of the font, in pixels per inch.
		bool mAntialias; // Whether or not to anti-alias the font by copying its alpha channel to its luminance channel.
		float mSpaceWidth; // The width of a "Space" character, in pixels. If zero, the default width is used.
		float mTabWidth; // The width of the "Tab" special character, in pixels.
		int mOffsetHeight; // How far up to nudge text rendered in this font, in pixels. May be negative to nudge text down.

		// The following variables are calculated automatically.
		int mDefaultHeight; // The nominal height of the font in pixels.
		MyGUI::ITexture* mTexture; // The texture that contains all of the rendered glyphs in the font.

		// The following constants used to be mutable, but they no longer need to be. Do not modify their values!
		static const int mGlyphSpacing; // How far apart the glyphs are placed from each other in the font texture, in pixels.
		static const float mSelectedWidth; // The width of the "Selected" and "SelectedBack" special characters, in pixels.
		static const float mCursorWidth; // The width of the "Cursor" special character, in pixels.

#ifdef MYGUI_USE_FREETYPE

	private:
		// A map of code points to glyph indices.
		typedef std::map<Char, FT_UInt> CharMap;

		// A map of glyph indices to glyph info objects.
		typedef std::map<FT_UInt, GlyphInfo> GlyphMap;

		// A map of glyph heights to the set of paired glyph indices and glyph info objects that are of that height.
		typedef std::map<FT_Pos, std::set<std::pair<FT_UInt, GlyphInfo*> > > GlyphHeightMap;

		template<bool LAMode, bool Antialias>
		void initialiseFreeType();

		// Wraps the current texture coordinates _texX and _texY to the beginning of the next line if the specified glyph width
		// doesn't fit at the end of the current line. Automatically takes the glyph spacing into account.
		void autoWrapGlyphPos(int _glyphWidth, int _texWidth, int _lineHeight, int& _texX, int& _texY);

		// Creates a glyph with the specified glyph index and assigns it to the specified code point.
		// Automatically updates _glyphHeightMap, mCharMap, and mGlyphMap with data from the new glyph..
		int createGlyph(FT_UInt _glyphIndex, Char _codePoint, float _width, float _height, float _advance, float _bearingX, float _bearingY, GlyphHeightMap& _glyphHeightMap);

		// Creates a glyph with the specified index from the specified font face and assigns it to the specified code point.
		// Automatically updates _glyphHeightMap and _maxGlyphHeight with data from the newly created glyph.
		int createFaceGlyph(FT_UInt _glyphIndex, Char _codePoint, int _fontAscent, const FT_Face& _face, GlyphHeightMap& _glyphHeightMap, int& _maxGlyphHeight);

		// Renders all of the glyphs in _glyphHeightMap into the specified texture buffer using data from the specified font face.
		template<bool LAMode, bool Antialias>
		void renderGlyphs(const GlyphHeightMap& _glyphHeightMap, const FT_Library& _ftLibrary, const FT_Face& _face, uint8* _texBuffer, int _texWidth, int _texHeight);

		// Renders the glyph described by the specified glyph info according to the specified parameters.
		// Supports two types of rendering, depending on the value of UseBuffer: Texture block transfer and rectangular color fill.
		// The _luminance0 value is used for even-numbered columns (from zero), while _luminance1 is used for odd-numbered ones.
		template<bool LAMode, bool UseBuffer, bool Antialias>
		void renderGlyph(GlyphInfo& _info, uint8 _luminance0, uint8 _luminance1, uint8 _alpha, int _lineHeight, uint8* _texBuffer, int _texWidth, int _texHeight, int& _texX, int& _texY, uint8* _glyphBuffer = nullptr);

		CharMap mCharMap; // A map of code points to glyph indices.
		GlyphMap mGlyphMap; // A map of glyph indices to glyph info objects.

#endif // MYGUI_USE_FREETYPE

	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_TRUE_TYPE_FONT_H__

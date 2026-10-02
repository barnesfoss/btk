// Copyright Kameron Barnes
// SPDX-License-Identifier: MIT

#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>

namespace {
/// Lerp between colors.
///
/// This is a function used internally to prevent repetitiveness.
/// \param a The base uint8.
/// \param b The target uint8.
/// \param t The progress towards the target.
uint8_t clerp(uint8_t a, uint8_t b, float t) {
  return a + t * (b - a);
}
} // namespace

namespace btk {
/// \brief Represents a 3 channel color.
///
/// Stores the red, green, and blue channels as 8-bit unsigned integers.
struct Color3 {
  /// Constructs a color based on 3 values.
  ///
  /// \param r The red channel.
  /// \param g The green channel.
  /// \param b The blue channel.
  Color3(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};

  /// Constructs a color based on a provided hexadecimal.
  ///
  /// \param hex A hexadecimal representation of a color
  Color3(uint32_t hex)
      : r(static_cast<uint8_t>((hex >> 16) & 0xff)), g(static_cast<uint8_t>((hex >> 8) & 0xff)),
        b(static_cast<uint8_t>(hex & 0xff)) {}

  /// Performs linear interpolation between two colors.
  ///
  /// Takes this color, a provided target color, and an alpha value and creates a new Color using.
  /// the interpolated values.
  ///
  /// \param target The target color.
  /// \param alpha The progress towards the target.
  Color3 lerp(const Color3 &target, float alpha) const {
    return {clerp(r, target.r, alpha), clerp(g, target.g, alpha), clerp(b, target.b, alpha)};
  }

  /// Outputs the color in a human-readable format.
  ///
  /// \param out The output stream.
  /// \param color The color to format.
  friend std::ostream &operator<<(std::ostream &out, const Color3 &color) {
    out << "Color3(" << static_cast<int>(color.r) << "," << static_cast<int>(color.g) << ","
        << static_cast<int>(color.b) << ")";
    return out;
  };

  friend bool operator==(const Color3 &color, const Color3 &color2) {
    return (color.r == color2.r) & (color.g == color2.g) & (color.b == color2.b);
  };

  friend bool operator!=(const Color3 &color, const Color3 &color2) {
    return (color.r != color2.r) || (color.g != color2.g) || (color.b != color2.b);
  };

  friend Color3 operator+(const Color3 &color, const Color3 &color2) {
    return {static_cast<uint8_t>(std::clamp(color.r + color2.r, 0, 255)),
            static_cast<uint8_t>(std::clamp(color.g + color2.g, 0, 255)),
            static_cast<uint8_t>(std::clamp(color.b + color2.b, 0, 255))};
  }

  friend Color3 operator-(const Color3 &color, const Color3 &color2) {
    return {static_cast<uint8_t>(std::clamp(color.r - color2.r, 0, 255)),
            static_cast<uint8_t>(std::clamp(color.g - color2.g, 0, 255)),
            static_cast<uint8_t>(std::clamp(color.b - color2.b, 0, 255))};
  }

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

/// Represents an HTML color in hex
enum COLOR : uint32_t {
  AliceBlue = 0xF0F8FF,
  AntiqueWhite = 0xFAEBD7,
  Aqua = 0x00FFFF,
  Aquamarine = 0x7FFFD4,
  Azure = 0xF0FFFF,
  Beige = 0xF5F5DC,
  Bisque = 0xFFE4C4,
  Black = 0x000000,
  BlanchedAlmond = 0xFFEBCD,
  Blue = 0x0000FF,
  BlueViolet = 0x8A2BE2,
  Brown = 0xA52A2A,
  BurlyWood = 0xDEB887,
  CadetBlue = 0x5F9EA0,
  Chartreuse = 0x7FFF00,
  Chocolate = 0xD2691E,
  Coral = 0xFF7F50,
  CornflowerBlue = 0x6495ED,
  Cornsilk = 0xFFF8DC,
  Crimson = 0xDC143C,
  Cyan = 0x00FFFF,
  DarkBlue = 0x00008B,
  DarkCyan = 0x008B8B,
  DarkGoldenRod = 0xB8860B,
  DarkGray = 0xA9A9A9,
  DarkGreen = 0x006400,
  DarkKhaki = 0xBDB76B,
  DarkMagenta = 0x8B008B,
  DarkOliveGreen = 0x556B2F,
  DarkOrange = 0xFF8C00,
  DarkOrchid = 0x9932CC,
  DarkRed = 0x8B0000,
  DarkSalmon = 0xE9967A,
  DarkSeaGreen = 0x8FBC8F,
  DarkSlateBlue = 0x483D8B,
  DarkSlateGray = 0x2F4F4F,
  DarkTurquoise = 0x00CED1,
  DarkViolet = 0x9400D3,
  DeepPink = 0xFF1493,
  DeepSkyBlue = 0x00BFFF,
  DimGray = 0x696969,
  DodgerBlue = 0x1E90FF,
  FireBrick = 0xB22222,
  FloralWhite = 0xFFFAF0,
  ForestGreen = 0x228B22,
  Fuchsia = 0xFF00FF,
  Gainsboro = 0xDCDCDC,
  GhostWhite = 0xF8F8FF,
  Gold = 0xFFD700,
  GoldenRod = 0xDAA520,
  Gray = 0x808080,
  Green = 0x008000,
  GreenYellow = 0xADFF2F,
  HoneyDew = 0xF0FFF0,
  HotPink = 0xFF69B4,
  IndianRed = 0xCD5C5C,
  Indigo = 0x4B0082,
  Ivory = 0xFFFFF0,
  Khaki = 0xF0E68C,
  Lavender = 0xE6E6FA,
  LavenderBlush = 0xFFF0F5,
  LawnGreen = 0x7CFC00,
  LemonChiffon = 0xFFFACD,
  LightBlue = 0xADD8E6,
  LightCoral = 0xF08080,
  LightCyan = 0xE0FFFF,
  LightGoldenRodYellow = 0xFAFAD2,
  LightGray = 0xD3D3D3,
  LightGreen = 0x90EE90,
  LightPink = 0xFFB6C1,
  LightSalmon = 0xFFA07A,
  LightSeaGreen = 0x20B2AA,
  LightSkyBlue = 0x87CEFA,
  LightSlateGray = 0x778899,
  LightSteelBlue = 0xB0C4DE,
  LightYellow = 0xFFFFE0,
  Lime = 0x00FF00,
  LimeGreen = 0x32CD32,
  Linen = 0xFAF0E6,
  Magenta = 0xFF00FF,
  Maroon = 0x800000,
  MediumAquaMarine = 0x66CDAA,
  MediumBlue = 0x0000CD,
  MediumOrchid = 0xBA55D3,
  MediumPurple = 0x9370DB,
  MediumSeaGreen = 0x3CB371,
  MediumSlateBlue = 0x7B68EE,
  MediumSpringGreen = 0x00FA9A,
  MediumTurquoise = 0x48D1CC,
  MediumVioletRed = 0xC71585,
  MidnightBlue = 0x191970,
  MintCream = 0xF5FFFA,
  MistyRose = 0xFFE4E1,
  Moccasin = 0xFFE4B5,
  NavajoWhite = 0xFFDEAD,
  Navy = 0x000080,
  OldLace = 0xFDF5E6,
  Olive = 0x808000,
  OliveDrab = 0x6B8E23,
  Orange = 0xFFA500,
  OrangeRed = 0xFF4500,
  Orchid = 0xDA70D6,
  PaleGoldenRod = 0xEEE8AA,
  PaleGreen = 0x98FB98,
  PaleTurquoise = 0xAFEEEE,
  PaleVioletRed = 0xDB7093,
  PapayaWhip = 0xFFEFD5,
  PeachPuff = 0xFFDAB9,
  Peru = 0xCD853F,
  Pink = 0xFFC0CB,
  Plum = 0xDDA0DD,
  PowderBlue = 0xB0E0E6,
  Purple = 0x800080,
  RebeccaPurple = 0x663399,
  Red = 0xFF0000,
  RosyBrown = 0xBC8F8F,
  RoyalBlue = 0x4169E1,
  SaddleBrown = 0x8B4513,
  Salmon = 0xFA8072,
  SandyBrown = 0xF4A460,
  SeaGreen = 0x2E8B57,
  SeaShell = 0xFFF5EE,
  Sienna = 0xA0522D,
  Silver = 0xC0C0C0,
  SkyBlue = 0x87CEEB,
  SlateBlue = 0x6A5ACD,
  SlateGray = 0x708090,
  Snow = 0xFFFAFA,
  SpringGreen = 0x00FF7F,
  SteelBlue = 0x4682B4,
  Tan = 0xD2B48C,
  Teal = 0x008080,
  Thistle = 0xD8BFD8,
  Tomato = 0xFF6347,
  Turquoise = 0x40E0D0,
  Violet = 0xEE82EE,
  Wheat = 0xF5DEB3,
  White = 0xFFFFFF,
  WhiteSmoke = 0xF5F5F5,
  Yellow = 0xFFFF00,
  YellowGreen = 0x9ACD32,
};

/// \brief Represents a 4 channel color.
///
/// Stores the red, green, blue and alpha channels as 8-bit unsigned integers.
struct Color4 : Color3 {
  using Color3::Color3;

  /// Constructs a color from RGB and alpha channel values.
  ///
  /// \param r Red channel value.
  /// \param g Green channel value.
  /// \param b Blue channel value.
  /// \param a Alpha channel value
  Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : Color3{r, g, b}, a{a} {}

  /// \copydoc Color3::lerp(const Color3&, float) const
  Color4 lerp(const Color4 &target, float alpha) const {
    return {clerp(r, target.r, alpha),
            clerp(g, target.g, alpha),
            clerp(b, target.b, alpha),
            clerp(a, target.a, alpha)};
  }

  /// Outputs the color in a human-readable format.
  ///
  /// \param out The output stream.
  /// \param color The color to format.
  friend std::ostream &operator<<(std::ostream &out, const Color4 &color) {
    out << "Color4(" << static_cast<int>(color.r) << "," << static_cast<int>(color.g) << ","
        << static_cast<int>(color.b) << "," << static_cast<int>(color.a) << ")";
    return out;
  };

  uint8_t a;
};
} // namespace btk

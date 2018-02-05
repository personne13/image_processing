/* -*- mode: c++; c-basic-offset: 3 -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC, nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 *
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @file regionlut.h
 *
 * Definition of the region lut.
 */

#ifndef __REGIONLUT_H
#define __REGIONLUT_H

#define MAXCOLORS 256

#define ASSIGNED_COLORS 142

/**
 * Modifies the color with the value i.
 */
inline Uchar ShiftColor( Uchar color, int i ) {
   int x = i / ASSIGNED_COLORS;
   return abs(color - 10 * x);
}

inline void buildRegionLut( Uchar lut[3][MAXCOLORS] ) {   
   lut[0][0] = 0;
   lut[1][0] = 0;
   lut[2][0] = 0;
   
   for (int i = 1; i < MAXCOLORS; i++) { 

      switch(i % ASSIGNED_COLORS) {
	 case 0: { // AliceBlue
	    lut[0][i] = ShiftColor(240, i);
	    lut[1][i] = ShiftColor(248, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 1: { // Aqua
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 2: { // Red
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = 0;
	    lut[2][i] = 0;
	 } break;
	 case 3: { // Lime
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = 0;
	 } break;
	 case 4: { // Blue
	    lut[0][i] = 0;
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 5: { // Yellow
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = 0;
	 } break;
	 case 6: { // BlueViolet
	    lut[0][i] = ShiftColor(138, i);
	    lut[1][i] = ShiftColor(43, i);
	    lut[2][i] = ShiftColor(226, i);
	 } break;
	 case 7: { // BlanchedAlmond
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(235, i);
	    lut[2][i] = ShiftColor(205, i);
	 } break;
	 case 8: { // Azure
	    lut[0][i] = ShiftColor(240, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 9: { // Brown
	    lut[0][i] = ShiftColor(165, i);
	    lut[1][i] = ShiftColor(42, i);
	    lut[2][i] = ShiftColor(42, i);
	 } break;
	 case 10: { // Yellow 1
	    lut[0][i] = ShiftColor(180, i);
	    lut[1][i] = ShiftColor(180, i);
	    lut[2][i] = 0;
	 } break;
	 case 11: { // BurlyWood
	    lut[0][i] = ShiftColor(222, i);
	    lut[1][i] = ShiftColor(184, i);
	    lut[2][i] = ShiftColor(135, i);
	 } break;
	 case 12: { // CadetBlue
	    lut[0][i] = ShiftColor(95, i);
	    lut[1][i] = ShiftColor(158, i);
	    lut[2][i] = ShiftColor(160, i);
	 } break;
	 case 13: { // Chartreuse
	    lut[0][i] = ShiftColor(127, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = 0;
	 } break;
	 case 14: { // Chocolate
	    lut[0][i] = ShiftColor(210, i);
	    lut[1][i] = ShiftColor(105, i);
	    lut[2][i] = ShiftColor(30, i);
	 } break;
	 case 15: { // Coral
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(127, i);
	    lut[2][i] = ShiftColor(80, i);
	 } break;
	 case 16: { // CornflowerBlue
	    lut[0][i] = ShiftColor(100, i);
	    lut[1][i] = ShiftColor(149, i);
	    lut[2][i] = ShiftColor(237, i);
	 } break;
	 case 17: { // Cornsilk
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(248, i);
	    lut[2][i] = ShiftColor(220, i);
	 } break;
         case 18: { // Crimson
	    lut[0][i] = ShiftColor(220, i);
	    lut[1][i] = ShiftColor(20, i);
	    lut[2][i] = ShiftColor(60, i);
	 } break;
	 case 19: { // Cyan
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 20: { // DarkBlue
	    lut[0][i] = 0;
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(139, i);
	 } break;
	 case 21: { // DarkCyan
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(139, i);
	    lut[2][i] = ShiftColor(139, i);
	 } break;
	 case 22: { // DarkGoldenRod
	    lut[0][i] = ShiftColor(184, i);
	    lut[1][i] = ShiftColor(134, i);
	    lut[2][i] = ShiftColor(11, i);
	 } break;
	 case 23: { // DarkGray
	    lut[0][i] = ShiftColor(169, i);
	    lut[1][i] = ShiftColor(169, i);
	    lut[2][i] = ShiftColor(169, i);
	 } break;
	 case 24: { // DarkGreen
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(100, i);
	    lut[2][i] = 0;
	 } break;
	 case 25: { // DarkKhaki
	    lut[0][i] = ShiftColor(189, i);
	    lut[1][i] = ShiftColor(183, i);
	    lut[2][i] = ShiftColor(107, i);
	 } break;
	 case 26: { // DarkMagenta
	    lut[0][i] = ShiftColor(139, i);
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(139, i);
	 } break;
	 case 27: { // DarkOliveGreen
	    lut[0][i] = ShiftColor(85, i);
	    lut[1][i] = ShiftColor(107, i);
	    lut[2][i] = ShiftColor(47, i);
	 } break;
	 case 28: { // Darkorange
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(140, i);
	    lut[2][i] = 0;
	 } break;
	 case 29: { // DarkOrchid
	    lut[0][i] = ShiftColor(153, i);
	    lut[1][i] = ShiftColor(50, i);
	    lut[2][i] = ShiftColor(204, i);
	 } break;
	 case 30: { // DarkRed
	    lut[0][i] = ShiftColor(139, i);
	    lut[1][i] = 0;
	    lut[2][i] = 0;
	 } break;
	 case 31: { // DarkSalmon
	    lut[0][i] = ShiftColor(233, i);
	    lut[1][i] = ShiftColor(150, i);
	    lut[2][i] = ShiftColor(122, i);
	 } break;
	 case 32: { // DarkSeaGreen
	    lut[0][i] = ShiftColor(143, i);
	    lut[1][i] = ShiftColor(188, i);
	    lut[2][i] = ShiftColor(143, i);
	 } break;
	 case 33: { // DarkSlateBlue
	    lut[0][i] = ShiftColor(72, i);
	    lut[1][i] = ShiftColor(61, i);
	    lut[2][i] = ShiftColor(139, i);
	 } break;
	 case 34: { // DarkSlateGray
	    lut[0][i] = ShiftColor(47, i);
	    lut[1][i] = ShiftColor(79, i);
	    lut[2][i] = ShiftColor(79, i);
	 } break;
	 case 35: { // DarkTurquoise
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(206, i);
	    lut[2][i] = ShiftColor(209, i);
	 } break;
	 case 36: { // DarkViolet
	    lut[0][i] = ShiftColor(148, i);
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(211, i);
	 } break;
	 case 37: { // DeepPink
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(20, i);
	    lut[2][i] = ShiftColor(147, i);
	 } break;
	 case 38: { // DeepSkyBlue
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(191, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 39: { // Bisque
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(228, i);
	    lut[2][i] = ShiftColor(196, i);
	 } break;
	 case 40: { // Yellow 8
	    lut[0][i] = ShiftColor(102, i);
	    lut[1][i] = ShiftColor(102, i);
	    lut[2][i] = 0;
	 } break;
	 case 41: { // DimGrey
	    lut[0][i] = ShiftColor(105, i);
	    lut[1][i] = ShiftColor(105, i);
	    lut[2][i] = ShiftColor(105, i);
	 } break;
	 case 42: { // DodgerBlue
	    lut[0][i] = ShiftColor(30, i);
	    lut[1][i] = ShiftColor(144, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 43: { // FireBrick
	    lut[0][i] = ShiftColor(178, i);
	    lut[1][i] = ShiftColor(34, i);
	    lut[2][i] = ShiftColor(34, i);
	 } break;
	 case 44: { // FloralWhite
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(250, i);
	    lut[2][i] = ShiftColor(240, i);
	 } break;
	 case 45: { // ForestGreen
	    lut[0][i] = ShiftColor(34, i);
	    lut[1][i] = ShiftColor(139, i);
	    lut[2][i] = ShiftColor(34, i);
	 } break;
	 case 46: { // Fuchsia
	    lut[0][i] = ShiftColor(244, i);
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(161, i);
	 } break;
	 case 47: { // Gainsboro
	    lut[0][i] = ShiftColor(220, i);
	    lut[1][i] = ShiftColor(220, i);
	    lut[2][i] = ShiftColor(220, i);
	 } break;
	 case 48: { // GhostWhite
	    lut[0][i] = ShiftColor(240, i);
	    lut[1][i] = ShiftColor(240, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 49: { // Gold
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(215, i);
	    lut[2][i] = 0;
	 } break;
	 case 50: { // GoldenRod
	    lut[0][i] = ShiftColor(218, i);
	    lut[1][i] = ShiftColor(165, i);
	    lut[2][i] = ShiftColor(32, i);
	 } break;
	 case 51: { // Gray
	    lut[0][i] = ShiftColor(128, i);
	    lut[1][i] = ShiftColor(128, i);
	    lut[2][i] = ShiftColor(128, i);
	 } break;
	 case 52: { // Green
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(128, i);
	    lut[2][i] = 0;
	 } break;
	 case 53: { // GreenYellow
	    lut[0][i] = ShiftColor(173, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(47, i);
	 } break;
	 case 54: { // HoneyDew
	    lut[0][i] = ShiftColor(240, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(240, i);
	 } break;
	 case 55: { // HotPink
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(105, i);
	    lut[2][i] = ShiftColor(180, i);
	 } break;
	 case 56: { // IndianRed 
	    lut[0][i] = ShiftColor(205, i);
	    lut[1][i] = ShiftColor(92, i);
	    lut[2][i] = ShiftColor(92, i);
	 } break;
	 case 57: { // Indigo 
	    lut[0][i] = ShiftColor(75, i);
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(130, i);
	 } break;
	 case 58: { // Ivory
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(240, i);
	 } break;
	 case 59: { // Khaki
	    lut[0][i] = ShiftColor(240, i);
	    lut[1][i] = ShiftColor(230, i);
	    lut[2][i] = ShiftColor(140, i);
	 } break;
	 case 60: { // Lavender
	    lut[0][i] = ShiftColor(230, i);
	    lut[1][i] = ShiftColor(230, i);
	    lut[2][i] = ShiftColor(250, i);
	 } break;
	 case 61: { // LavenderBlush
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(240, i);
	    lut[2][i] = ShiftColor(245, i);
	 } break;
	 case 62: { // LawnGreen
	    lut[0][i] = ShiftColor(124, i);
	    lut[1][i] = ShiftColor(252, i);
	    lut[2][i] = 0;
	 } break;
	 case 63: { // LemonChiffon
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(250, i);
	    lut[2][i] = ShiftColor(205, i);
	 } break;
	 case 64: { // LightBlue
	    lut[0][i] = ShiftColor(173, i);
	    lut[1][i] = ShiftColor(216, i);
	    lut[2][i] = ShiftColor(230, i);
	 } break;
	 case 65: { // LightCoral
	    lut[0][i] = ShiftColor(240, i);
	    lut[1][i] = ShiftColor(128, i);
	    lut[2][i] = ShiftColor(128, i);
	 } break;
	 case 66: { // LightCyan
	    lut[0][i] = ShiftColor(224, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 67: { // LightGoldenRodYellow
	    lut[0][i] = ShiftColor(250, i);
	    lut[1][i] = ShiftColor(250, i);
	    lut[2][i] = ShiftColor(210, i);
	 } break;
	 case 68: { // LightGray
	    lut[0][i] = ShiftColor(211, i);
	    lut[1][i] = ShiftColor(211, i);
	    lut[2][i] = ShiftColor(211, i);
	 } break;
	 case 69: { // LightGreen
	    lut[0][i] = ShiftColor(144, i);
	    lut[1][i] = ShiftColor(238, i);
	    lut[2][i] = ShiftColor(144, i);
	 } break;
	 case 70: { // LightPink
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(182, i);
	    lut[2][i] = ShiftColor(193, i);
	 } break;
	 case 71: { // LightSalmon
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(160, i);
	    lut[2][i] = ShiftColor(122, i);
	 } break;
	 case 72: { // LightSeaGreen
	    lut[0][i] = ShiftColor(32, i);
	    lut[1][i] = ShiftColor(178, i);
	    lut[2][i] = ShiftColor(170, i);
	 } break;
	 case 73: { // LightSkyBlue
	    lut[0][i] = ShiftColor(135, i);
	    lut[1][i] = ShiftColor(206, i);
	    lut[2][i] = ShiftColor(250, i);
	 } break;
	 case 74: { // LightSlateGray
	    lut[0][i] = ShiftColor(119, i);
	    lut[1][i] = ShiftColor(136, i);
	    lut[2][i] = ShiftColor(153, i);
	 } break;
	 case 75: { // LightSteelBlue
	    lut[0][i] = ShiftColor(176, i);
	    lut[1][i] = ShiftColor(196, i);
	    lut[2][i] = ShiftColor(222, i);
	 } break;
	 case 76: { // LightYellow
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(224, i);
	 } break;
	 case 77: { // Aquamarine
	    lut[0][i] = ShiftColor(127, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(212, i);
	 } break;
	 case 78: { // LimeGreen
	    lut[0][i] = ShiftColor(50, i);
	    lut[1][i] = ShiftColor(205, i);
	    lut[2][i] = ShiftColor(50, i);
	 } break;
	 case 79: { // Linen
	    lut[0][i] = ShiftColor(250, i);
	    lut[1][i] = ShiftColor(240, i);
	    lut[2][i] = ShiftColor(230, i);
	 } break;
	 case 80: { // Magenta
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 81: { // Maroon
	    lut[0][i] = ShiftColor(128, i);
	    lut[1][i] = 0;
	    lut[2][i] = 0;
	 } break;
	 case 82: { // MediumAquaMarine
	    lut[0][i] = ShiftColor(102, i);
	    lut[1][i] = ShiftColor(205, i);
	    lut[2][i] = ShiftColor(170, i);
	 } break;
	 case 83: { // MediumBlue
	    lut[0][i] = 0;
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(205, i);
	 } break;
	 case 84: { // MediumOrchid
	    lut[0][i] = ShiftColor(186, i);
	    lut[1][i] = ShiftColor(85, i);
	    lut[2][i] = ShiftColor(211, i);
	 } break;
	 case 85: { // MediumPurple
	    lut[0][i] = ShiftColor(147, i);
	    lut[1][i] = ShiftColor(112, i);
	    lut[2][i] = ShiftColor(219, i);
	 } break;
	 case 86: { // MediumSeaGreen
	    lut[0][i] = ShiftColor(60, i);
	    lut[1][i] = ShiftColor(179, i);
	    lut[2][i] = ShiftColor(113, i);
	 } break;
	 case 87: { // WhiteSmoke
	    lut[0][i] = ShiftColor(245, i);
	    lut[1][i] = ShiftColor(245, i);
	    lut[2][i] = ShiftColor(245, i);
	 } break;
	 case 88: { // MediumSpringGreen
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(250, i);
	    lut[2][i] = ShiftColor(154, i);
	 } break;
	 case 89: { // MediumTurquoise
	    lut[0][i] = ShiftColor(72, i);
	    lut[1][i] = ShiftColor(209, i);
	    lut[2][i] = ShiftColor(204, i);
	 } break;
	 case 90: { // MediumVioletRed
	    lut[0][i] = ShiftColor(199, i);
	    lut[1][i] = ShiftColor(21, i);
	    lut[2][i] = ShiftColor(133, i);
	 } break;
	 case 91: { // MidnightBlue
	    lut[0][i] = ShiftColor(25, i);
	    lut[1][i] = ShiftColor(25, i);
	    lut[2][i] = ShiftColor(112, i);
	 } break;
	 case 92: { // MintCream
	    lut[0][i] = ShiftColor(245, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(250, i);
	 } break;
	 case 93: { // MistyRose
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(228, i);
	    lut[2][i] = ShiftColor(225, i);
	 } break;
	 case 94: { // Moccasin
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(228, i);
	    lut[2][i] = ShiftColor(181, i);
	 } break;
	 case 95: { // NavajoWhite
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(222, i);
	    lut[2][i] = ShiftColor(173, i);
	 } break;
	 case 96: { // Navy
	    lut[0][i] = 0;
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(128, i);
	 } break;
	 case 97: { // OldLace
	    lut[0][i] = ShiftColor(253, i);
	    lut[1][i] = ShiftColor(245, i);
	    lut[2][i] = ShiftColor(230, i);
	 } break;
	 case 98: { // Olive
	    lut[0][i] = ShiftColor(130, i);
	    lut[1][i] = ShiftColor(130, i);
	    lut[2][i] = 0;
	 } break;
	 case 99: { // OliveDrab
	    lut[0][i] = ShiftColor(107, i);
	    lut[1][i] = ShiftColor(142, i);
	    lut[2][i] = ShiftColor(35, i);
	 } break;
	 case 100: { // Orange
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(165, i);
	    lut[2][i] = 0;
	 } break;
	 case 101: { // Beige
	    lut[0][i] = ShiftColor(245, i);
	    lut[1][i] = ShiftColor(245, i);
	    lut[2][i] = ShiftColor(220, i);
	 } break;
	 case 102: { // Orchid
	    lut[0][i] = ShiftColor(218, i);
	    lut[1][i] = ShiftColor(112, i);
	    lut[2][i] = ShiftColor(214, i);
	 } break;
	 case 103: { // PaleGoldenRod
	    lut[0][i] = ShiftColor(238, i);
	    lut[1][i] = ShiftColor(232, i);
	    lut[2][i] = ShiftColor(170, i);
	 } break;
	 case 104: { // PaleGreen
	    lut[0][i] = ShiftColor(152, i);
	    lut[1][i] = ShiftColor(251, i);
	    lut[2][i] = ShiftColor(152, i);
	 } break;
	 case 105: { // PaleTurquoise
	    lut[0][i] = ShiftColor(175, i);
	    lut[1][i] = ShiftColor(238, i);
	    lut[2][i] = ShiftColor(238, i);
	 } break;
	 case 106: { // PaleVioletRed
	    lut[0][i] = ShiftColor(219, i);
	    lut[1][i] = ShiftColor(112, i);
	    lut[2][i] = ShiftColor(147, i);
	 } break;
	 case 107: { // PapayaWhip
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(239, i);
	    lut[2][i] = ShiftColor(213, i);
	 } break;
	 case 108: { // PeachPuff
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(218, i);
	    lut[2][i] = ShiftColor(185, i);
	 } break;
	 case 109: { // Peru
	    lut[0][i] = ShiftColor(205, i);
	    lut[1][i] = ShiftColor(133, i);
	    lut[2][i] = ShiftColor(63, i);
	 } break;
	 case 110: { // Pink
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(192, i);
	    lut[2][i] = ShiftColor(203, i);
	 } break;
	 case 111: { // Plum
	    lut[0][i] = ShiftColor(221, i);
	    lut[1][i] = ShiftColor(160, i);
	    lut[2][i] = ShiftColor(221, i);
	 } break;
	 case 112: { // PowderBlue
	    lut[0][i] = ShiftColor(176, i);
	    lut[1][i] = ShiftColor(224, i);
	    lut[2][i] = ShiftColor(230, i);
	 } break;
	 case 113: { // Purple
	    lut[0][i] = ShiftColor(128, i);
	    lut[1][i] = 0;
	    lut[2][i] = ShiftColor(128, i);
	 } break;
         case 114: { // AntiqueWhite
	    lut[0][i] = ShiftColor(250, i);
	    lut[1][i] = ShiftColor(235, i);
	    lut[2][i] = ShiftColor(215, i);
	 } break;
	 case 115: { // RosyBrown
	    lut[0][i] = ShiftColor(188, i);
	    lut[1][i] = ShiftColor(143, i);
	    lut[2][i] = ShiftColor(143, i);
	 } break;
	 case 116: { // RoyalBlue
	    lut[0][i] = ShiftColor(65, i);
	    lut[1][i] = ShiftColor(105, i);
	    lut[2][i] = ShiftColor(225, i);
	 } break;
	 case 117: { // SaddleBrown
	    lut[0][i] = ShiftColor(139, i);
	    lut[1][i] = ShiftColor(69, i);
	    lut[2][i] = ShiftColor(19, i);
	 } break;
	 case 118: { // Salmon
	    lut[0][i] = ShiftColor(250, i);
	    lut[1][i] = ShiftColor(128, i);
	    lut[2][i] = ShiftColor(114, i);
	 } break;
	 case 119: { // SandyBrown
	    lut[0][i] = ShiftColor(244, i);
	    lut[1][i] = ShiftColor(164, i);
	    lut[2][i] = ShiftColor(96, i);
	 } break;
	 case 120: { // SeaGreen
	    lut[0][i] = ShiftColor(46, i);
	    lut[1][i] = ShiftColor(139, i);
	    lut[2][i] = ShiftColor(87, i);
	 } break;
	 case 121: { // SeaShell
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(245, i);
	    lut[2][i] = ShiftColor(238, i);
	 } break;
	 case 122: { // Sienna
	    lut[0][i] = ShiftColor(160, i);
	    lut[1][i] = ShiftColor(82, i);
	    lut[2][i] = ShiftColor(45, i);
	 } break;
	 case 123: { // Silver
	    lut[0][i] = ShiftColor(192, i);
	    lut[1][i] = ShiftColor(192, i);
	    lut[2][i] = ShiftColor(192, i);
	 } break;
	 case 124: { // SkyBlue
	    lut[0][i] = ShiftColor(135, i);
	    lut[1][i] = ShiftColor(206, i);
	    lut[2][i] = ShiftColor(235, i);
	 } break;
	 case 125: { // SlateBlue
	    lut[0][i] = ShiftColor(106, i);
	    lut[1][i] = ShiftColor(90, i);
	    lut[2][i] = ShiftColor(205, i);
	 } break;
	 case 126: { // SlateGray
	    lut[0][i] = ShiftColor(112, i);
	    lut[1][i] = ShiftColor(128, i);
	    lut[2][i] = ShiftColor(144, i);
	 } break;
	 case 127: { // Snow
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(250, i);
	    lut[2][i] = ShiftColor(250, i);
	 } break;
	 case 128: { // SpringGreen
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(127, i);
	 } break;
	 case 129: { // SteelBlue
	    lut[0][i] = ShiftColor(70, i);
	    lut[1][i] = ShiftColor(130, i);
	    lut[2][i] = ShiftColor(180, i);
	 } break;
	 case 130: { // Tan
	    lut[0][i] = ShiftColor(210, i);
	    lut[1][i] = ShiftColor(180, i);
	    lut[2][i] = ShiftColor(140, i);
	 } break;
	 case 131: { // Teal
	    lut[0][i] = 0;
	    lut[1][i] = ShiftColor(128, i);
	    lut[2][i] = ShiftColor(128, i);
	 } break;
	 case 132: { // Thistle
	    lut[0][i] = ShiftColor(216, i);
	    lut[1][i] = ShiftColor(191, i);
	    lut[2][i] = ShiftColor(216, i);
	 } break;
	 case 133: { // Tomato
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(99, i);
	    lut[2][i] = ShiftColor(71, i);
	 } break;
	 case 134: { // Turquoise
	    lut[0][i] = ShiftColor(64, i);
	    lut[1][i] = ShiftColor(224, i);
	    lut[2][i] = ShiftColor(208, i);
	 } break;
	 case 135: { // Violet
	    lut[0][i] = ShiftColor(238, i);
	    lut[1][i] = ShiftColor(130, i);
	    lut[2][i] = ShiftColor(238, i);
	 } break;
	 case 136: { // Wheat
	    lut[0][i] = ShiftColor(245, i);
	    lut[1][i] = ShiftColor(222, i);
	    lut[2][i] = ShiftColor(179, i);
	 } break;
	 case 137: { // White
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(255, i);
	    lut[2][i] = ShiftColor(255, i);
	 } break;
	 case 138: { // MediumSlateBlue
	    lut[0][i] = ShiftColor(123, i);
	    lut[1][i] = ShiftColor(104, i);
	    lut[2][i] = ShiftColor(238, i);
	 } break;
	 case 139: { // OrangeRed
	    lut[0][i] = ShiftColor(255, i);
	    lut[1][i] = ShiftColor(69, i);
	    lut[2][i] = 0;
	 } break;
	 case 140: { // Magenta 2
	    lut[0][i] = ShiftColor(148, i);
	    lut[1][i] = ShiftColor(71, i);
	    lut[2][i] = ShiftColor(107, i);
	 } break;
	 case 141: { // YellowGreen
	    lut[0][i] = ShiftColor(154, i);
	    lut[1][i] = ShiftColor(205, i);
	    lut[2][i] = ShiftColor(50, i);
	 }
      }
   }
 }

#undef ASSIGNED_COLORS
 
#endif

/***************************************************************************
 *   Copyright (C) 2021 by pilar   *
 *   pilarb@unex.es   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QtCore>

namespace imageprocess{
	extern void copiar(uchar * imgO, uchar * imgD);
	extern void borrar(uchar * imgD);
	extern void cambiarContrasteFCuadrada(uchar * imgO, uchar * imgD);
	extern void cambiarContrasteFInversa(uchar * imgO, uchar * imgD);
	extern void cambiarEscalaGrises(uchar * imgO, uchar * imgD, uchar minO, uchar maxO, uchar minD, uchar maxD);
    extern void umbralizar(uchar * imgO, uchar * imgD, uchar uMax, uchar uMin);
	extern void filtroLineal(uchar * imgO, int * kernel, int norm, uchar * imgD);
    extern void ecualizarHistograma(int * histoOrig, uchar * tablaLUT);
    extern void aplicarTablaLUT(uchar * imgO, uchar * tablaLUT, uchar * imgD);

}

#endif

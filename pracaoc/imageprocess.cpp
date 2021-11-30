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

#include "imageprocess.h"

#define WITHOUT_SSE

void imageprocess::copiar(uchar * imgO, uchar * imgD)
{

#ifdef WITHOUT_SSE
    asm volatile(
        "mov %0, %%rsi ;"
        "mov %1, %%rdi ;"
        "mov $320*240, %%rcx ;"
    "bcopia: ;"
        "mov (%%rsi), %%al ;"
        "mov %%al, (%%rdi) ;"
        "inc %%rsi ;"
        "inc %%rdi ;"
        "loop bcopia ;"

        :
        : "m" (imgO), "m" (imgD)
        : "%rax", "%rcx", "%rsi", "%rdi", "memory"

    );
#else
    asm volatile(
        "mov %0, %%rsi ;"
        "mov %1, %%rdi ;"
        "mov $320*240/16, %%rcx ;"
    "bcopia: ;"
        "movdqa (%%rsi), %%xmm0 ;"
        "movdqa %%xmm0, (%%rdi) ;"
        "add $16, %%rsi ;"
        "add $16, %%rdi ;"
        "loop bcopia ;"

        :
        : "m" (imgO), "m" (imgD)
        : "%rcx", "%rsi", "%rdi", "%xmm0", "memory"

    );

#endif

}

void imageprocess::borrar(uchar * imgD)
{

#ifdef WITHOUT_SSE
    asm volatile(
        ";"

        :
        : "m" (imgD)
        : "memory"

    );

#else
    asm volatile(
        ";"

        :
        : "m" (imgD)
        : "memory"

    );

#endif

}

void imageprocess::cambiarContrasteFCuadrada(uchar * imgO, uchar * imgD)
{

#ifdef WITHOUT_SSE
    asm volatile(
        ";"
		
        :
        : "m" (imgO),	"m" (imgD)
        : "memory"
    );
#else
    asm volatile(
        ";"

        :
        : "m" (imgO),	"m" (imgD)
        : "memory"
    );

#endif

}

void imageprocess::cambiarContrasteFInversa(uchar * imgO, uchar * imgD)
{

#ifdef WITHOUT_SSE
    asm volatile(
        ";"
		
        :
        : "m" (imgO),	"m" (imgD)
        : "memory"
    );

#else
    asm volatile(
        ";"

        :
        : "m" (imgO),	"m" (imgD)
        : "memory"
    );

#endif

}

void imageprocess::cambiarEscalaGrises(uchar * imgO, uchar * imgD, uchar minO, uchar maxO, uchar minD, uchar maxD)
{

#ifdef WITHOUT_SSE
    asm volatile(
        ";"
		
        :
        : "m" (imgO), "m" (imgD), "m" (minO), "m" (maxO), "m" (minD), "m" (maxD)
        : "memory"
    );
#else
    asm volatile(
        ";"

        :
        : "m" (imgO), "m" (imgD), "m" (minO), "m" (maxO), "m" (minD), "m" (maxD)
        : "memory"
    );

#endif

}


void imageprocess::umbralizar(uchar * imgO, uchar * imgD, uchar uMax, uchar uMin)
{
    asm volatile(
        ";"

        :
        : "m" (imgO), "m" (imgD), "m" (uMax), "m" (uMin)
        : "memory"

    );


}


void imageprocess::filtroLineal(uchar * imgO, int * kernel, int norm, uchar * imgD)
{
	
    asm volatile(
        ";"

        :
        : "m" (imgO), "m" (kernel), "m"(norm), "m" (imgD)
        : "memory"
    );
  
}


void imageprocess::ecualizarHistograma(int * histoOrig, uchar * tablaLUT)
{
    int * acumHisto;

    acumHisto = new int[256];

    asm volatile(
        ";"

        :
        : "m" (histoOrig), "m" (tablaLUT), "m" (acumHisto)
        : "memory"
    );

}


void imageprocess::aplicarTablaLUT(uchar * imgO, uchar * tablaLUT, uchar * imgD)
{
    asm volatile(
        ";"

        :
        : "m" (imgO), "m" (tablaLUT), "m" (imgD)
        : "memory"
    );

}


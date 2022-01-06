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

        "mov %0, %%rsi ;"
        "mov $320*240, %%rcx ;"

        "bborrar: ;"

            "movl $0, (%%rsi) ;"

            "inc %%rsi ;"
            "loop bborrar ;"

        :
        : "m" (imgD)
        : "%rcx", "%rsi", "memory"

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

        "mov %0, %%rsi ;"
        "mov %1, %%rdi ;"
        "mov $320*240, %%rcx ;"

        "bcontrastecuadrado: ;"

            "mov (%%rsi), %%al;"
            "mul %%al;"

            "mov $255, %%bl;"
            "div %%bl;"

            "mov %%al, (%%rdi);"

            "inc %%rsi ;"
            "inc %%rdi ;"
            "loop bcontrastecuadrado ;"
		
        :
        : "m" (imgO),	"m" (imgD)
        : "%rbx", "%rax", "%rcx", "%rsi", "%rdi", "memory"
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
        "mov %0, %%rsi ;"
        "mov %1, %%rdi ;"
        "mov $320*240, %%rcx ;"
        "bcontrasteinversa: ;"

            "mov (%%rsi), %%al;"

            "mov $255, %%bl;"
            "sub %%al, %%bl;"

            "mov %%bl, (%%rdi);"

            "inc %%rsi ;"
            "inc %%rdi ;"
            "loop bcontrasteinversa ;"
		
        :
        : "m" (imgO),	"m" (imgD)
        : "%rbx", "%rax", "%rcx", "%rsi", "%rdi", "memory"
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
        "mov %0, %%rsi ;"
        "mov %1, %%rdi ;"
        "mov %2, %%r8b ;"
        "mov %3, %%bl ;"
        "mov %4, %%dl ;"
        "mov %5, %%r9b ;"

        "mov %%bl, %%r10b;"
        "sub %%r8b, %%r10b;"          // rangoO = maxO - minO

        "mov %%r9b, %%r11b;"
        "sub %%dl, %%r11b;"         // rangoD = maxD - minD

        "mov $320*240, %%rcx ;"
        "bescalagrises: ;"

            "mov (%%rsi), %%al;"

            "sub %%r8b, %%al;"     // g - minO

            "mul %%r11b;"            // (g-minO) * rangoD

            "div %%r10b;"            // ((g-minO)*rangoD) / rangoO

            "add %%dl, %%al;"     // (((g-minO)*rangoD)/rangoO) + minD

            "mov %%al, (%%rdi);"

            "inc %%rsi ;"
            "inc %%rdi ;"
            "loop bescalagrises ;"
		
        :
        : "m" (imgO), "m" (imgD), "m" (minO), "m" (maxO), "m" (minD), "m" (maxD)
        : "%rax", "%rbx", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11", "%r12", "%rsi", "%rdi", "memory"
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
        "mov %0, %%rsi ;"
        "mov %1, %%rdi ;"
        "mov %2, %%r8b;"        // uMax
        "mov %3, %%r9b;"        // uMin

        "mov $320*240, %%rcx ;"
        "bumbralizar: ;"

            "mov %%r8b, %%al;"
            "cmp %%al, (%%rsi);"
            "jbe sino1;"
                "movb $255, (%%rdi);"
                "jmp fin;"

            "sino1: ;"
                "mov %%r9b, %%al;"
                "cmp %%al, (%%rsi);"
                "jb si2;"
                    "movb $127, (%%rdi);"
                    "jmp fin;"
                "si2: ;"
                    "movb $0, (%%rdi);"

            "fin: ;"
            "inc %%rsi ;"
            "inc %%rdi ;"
            "loop bumbralizar ;"


        :
        : "m" (imgO), "m" (imgD), "m" (uMax), "m" (uMin)
        : "%rax", "%rbx", "%rcx", "%r8", "%r9", "%rsi", "%rdi", "memory"

    );


}


void imageprocess::filtroLineal(uchar * imgO, int * kernel, int norm, uchar * imgD)
{
	
    asm volatile(
        "mov %0, %%rsi;"                                        //img0 = rsi
        "mov %1, %%r8;"                                         // kernel
        "mov %2, %%r9d;"                                        // norm
        "mov %3, %%rdi;"                                        //imgD = rdi

        "mov $240, %%r10;"                                      // f = 240
        "mov $320, %%r11;"                                      // c = 320
        "bfiltrolineal_general1: "

                "bfiltrolineal_general2: ;"
                    "mov %%r8, %%r12;"                          // dirKernel = kernel
                    "xor %%r13, %%r13;"                           // acum = 0

                    // BUCLE ESPECIFICO 1
                    "mov $-1, %%r14b;"                          // kf = -1
                    "bfiltrolineal_especifico_1: ;"

                        //BUCLE ESPECIFICO 2
                        "mov $-1, %%r15b;"                      // kc = -1
                        "bfiltrolineal_especifico_2: ;"

                            //SENTENCIAS DE BUCLE ESPECIFICO 2
                            "mov %%r10, %%rax;"
                            "add %%r14, %%rax;"                 // fp = f+kf

                            "mov %%r11, %%rbx;"
                            "add %%r15, %%rbx;"                 // cp = c+kc

                            // COMPARACION
                            "cmp $0, %%rbx;"
                            "jl comparacion_especifica_fin;"
                            "cmp $320, %%rbx;"
                            "jge comparacion_especifica_fin;"
                            "cmp $0, %%rax;"
                            "jl comparacion_especifica_fin;"
                            "cmp $240, %%rax;"
                            "jge comparacion_especifica_fin;"

                                "mov $320, %%rdx;"
                                "mul %%rdx;"
                                "add %%rbx, %%rax;"               // offPixel = fp*320 + cp
                                "add (%%rdi), %%rax;"            // g = dirOrig + offPixel
                                "mul %%r12;"
                                "add %%rax, %%r13;"             // acum = acum + g*dirKernel


                            "comparacion_especifica_fin: ;"
                            "add $4, %%r12;"


                        "inc %%r15b;"
                        "cmp %1, %%r15;"
                        "jbe bfiltrolineal_especifico_2;"

                    "inc %%r14b;"
                    "cmp %1, %%r14;"
                    "jbe bfiltrolineal_especifico_1;"

                    // SENTENCIAS ANTES DE OTRO CICLO DEL BUCLE GENERAL 2
                    "xor %%rax, %%rax;"
                    "mov %%r13, %%rax;"
                    "mov %%r9, %%rbx;"

                    "div %%rbx;"

                    "mov %%rax, %%r13;"

                    "cmp $0, %%r13;"
                    "jg comparacion_fin_1;"
                    "mov $0, %%r13;"
                    "comparacion_fin_1: ;"
                    "cmp $255, %%r13;"
                    "jb comparacion_fin_2;"
                    "mov $255, %%r13;"
                    "comparacion_fin_2: ;"

                    "mov %%r13, (%%rdi);"
                    "inc %%rdi;"


                // PARTE DEL BUCLE GENERAL 2
                "dec %%r11b;"
                "cmp %0, %%r11b;"
                "jg bfiltrolineal_general2;"

        // PARTE DEL BUCLE GENERAL 1
        "dec %%r10b;"
        "cmp %0, %%r10b;"
        "jg bfiltrolineal_general1;"

        :
        : "m" (imgO), "m" (kernel), "m"(norm), "m" (imgD)
        : "%rsi", "%rdi", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%rax", "%rbx", "%rcx", "%rdx", "memory"
    );
  
}


void imageprocess::ecualizarHistograma(int * histoOrig, uchar * tablaLUT)
{
    int * acumHisto;

    acumHisto = new int[256];

    asm volatile(
        "mov %0, %%rsi;"            //dirHOrirg
        "mov %2, %%rdi;"            //dirHAcum
        "xor %%r9, %%r9;"           //acum

        "mov $256, %%rcx;"
        "becualizar_1: ;"
                "mov (%%rsi, %%rcx, 4), %%rax;"
                "add %%rax, %%r9;"
                "mov %%r9, (%%rsi, %%rcx, 4);"
        "loop becualizar_1;"

        "mov %1, %%r10;"            //dirLUT

        "mov $256, %%rcx;"
        "becualizar_2: ;"
                "mov $320, %%rax;"
                "mov $240, %%rdx;"
                "mul %%rdx;"
                "mov %%rax, %%rdx;" //320*240

                "mov (%%rdi, %%rcx, 4), %%rax;"
                "mov $256, %%rbx;"
                "mul %%rbx;"

                "div %%rdx;"        //g = (256*[dirHAcum + n*4])/(320*240);

                "cmp %0, %%al;"
                "jle secualizar_1;"
                    "dec %%rax;"
                "secualizar_1: ;"

                "mov %%rax, (%%r10, %%rcx);"

        "loop becualizar_2;"



        :
        : "m" (histoOrig), "m" (tablaLUT), "m" (acumHisto)
        : "%rsi", "%rdi", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%rax", "%rbx", "%rcx", "%rdx", "memory"
    );

}


void imageprocess::aplicarTablaLUT(uchar * imgO, uchar * tablaLUT, uchar * imgD)
{
    asm volatile(
        "mov %0, %%rsi;"
        "mov %1, %%r8;"
        "mov %2, %%rdi;"

        "mov $320*240, %%rcx ;"
        "baplicartablalut: ;"

                "xor %%rax, %%rax;"
                "mov (%%rsi), %%al;"
                "add (%%r8, %%rax), %%al;"

                "mov %%al, (%%rdi);"

                "inc %%rsi ;"
                "inc %%rdi ;"
                "loop baplicartablalut;"

        :
        : "m" (imgO), "m" (tablaLUT), "m" (imgD)
        : "%r8", "%r9", "%rax", "%rbx", "%rcx", "memory"
    );

}


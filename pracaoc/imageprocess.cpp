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

        "mov $3200, %%r8 ;"
        "mov $73600, %%r9 ;"
        "bcontrastecuadrado: ;"

            "cmp $0, %%r8 ;"
            "jle s_contrasteCuadrado_siguiente;"
            "movb $0, (%%rdi) ;"
            "jmp s_contrasteCuadrado_fin ;"

            "s_contrasteCuadrado_siguiente: "
            "cmp $0, %%r9 ;"
            "jg s_contrasteCuadrado_cambiar;"
            "movb $0, (%%rdi) ;"
            "jmp s_contrasteCuadrado_fin ;"

            "s_contrasteCuadrado_cambiar: "

            "mov (%%rsi), %%al;"
            "mul %%al;"

            "mov $255, %%bl;"
            "div %%bl;"

            "mov %%al, (%%rdi);"

            "s_contrasteCuadrado_fin: "

            "dec %%r8 ;"
            "dec %%r9 ;"

            "inc %%rsi ;"
            "inc %%rdi ;"
            "loop bcontrastecuadrado ;"
		
        :
        : "m" (imgO),	"m" (imgD)
        : "%rbx", "%rax", "%rcx", "%rsi", "%rdi", "%r8", "%r9", "memory"
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
        "mov $240, %%rcx ;"
        "mov $0, %%r9;"
        "bcontrasteinversa: ;"

            "cmp $0, %%r9 ;"
            "je s_bool_1;"
            "cmp $0, %%r9 ;"
            "jne s_bool_0;"

            "s_bool_0: "
                "mov $0, %%r9 ;"
                "jmp s_bool_fin ;"
            "s_bool_1: "
                "mov $1, %%r9 ;"
            "s_bool_fin: ;"

            "mov $320, %%r8 ;"
            "bcontrasteinversa_2: "

                "cmp $0, %%r9;"
                "je s_fin_contraste ;"

                "mov (%%rsi), %%al;"

                "mov $255, %%bl;"
                "sub %%al, %%bl;"

                "mov %%bl, (%%rdi);"

                "s_fin_contraste: "

                "inc %%rsi ;"
                "inc %%rdi ;"


            "dec %%r8;"
            "cmp $0, %%r8;"
            "jg bcontrasteinversa_2 ;"

        "dec %%rcx;"
        "cmp $0, %%rcx;"
        "jg bcontrasteinversa ;"
		
        :
        : "m" (imgO),	"m" (imgD)
        : "%rbx", "%rax", "%rcx", "%rdx", "%r8", "%r9", "%rsi", "%rdi", "memory"
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
        "sub %%r8b, %%r10b;"                /// rangoO = maxO - minO

        "mov %%r9b, %%r11b;"
        "sub %%dl, %%r11b;"                 /// rangoD = maxD - minD

        "mov $0, %%r9;"
        "mov $320*240, %%rcx ;"
        "bescalagrises: ;"

            "mov %%rcx, %%rax;"
            "mov $2, %%ebx;"
            "xor %%rdx, %%rdx;"
            "div %%ebx;"
            "cmp $0, %%rdx;"
            "je s_cambiarEscalaGrises_NoAplicar ;"

                "mov (%%rsi), %%al;"

                "sub %%r8b, %%al;"              /// g - minO

                "mul %%r11b;"                   /// (g-minO) * rangoD

                "xor %%rdx, %%rdx;"
                "div %%r10b;"                   /// ((g-minO)*rangoD) / rangoO

                "add %%dl, %%al;"               /// (((g-minO)*rangoD)/rangoO) + minD

                "mov %%al, (%%rdi);"

                "jmp s_cambiarEscalaGrises_fin ;"

            "s_cambiarEscalaGrises_NoAplicar: "

                "cmp $0, %%r9 ;"
                "je s_escala_bool_1;"
                "cmp $0, %%r9 ;"
                "jne s_escala_bool_0;"

                "s_escala_bool_0: "
                    "mov $0, %%r9 ;"
                    "movb $255, (%%rdi) ;;"
                    "jmp s_escala_bool_fin ;"
                "s_escala_bool_1: "
                    "mov $1, %%r9 ;"
                    "movb $0, (%%rdi) ;"
                "s_escala_bool_fin: ;"

            "s_cambiarEscalaGrises_fin: "

            "inc %%rsi ;"
            "inc %%rdi ;"
            "loop bescalagrises ;"
		
        :
        : "m" (imgO), "m" (imgD), "m" (minO), "m" (maxO), "m" (minD), "m" (maxD)
        : "%rax", "%rbx", "%rcx", "%rdx", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%rsi", "%rdi", "memory"
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
// r10d=(int)f  r11d=(int)c  r12=(*)kernel  rdx=(short)acum  r14d=(int)kf
// r15d=(int)kc

// al=rXb=1byte=uchar  ax=rXw=2byte=short  eax=rXd=4byte=int  rax=rX=8byte=*(dir memoria)
    asm volatile(
        "mov %0, %%rsi;"                                        /// dirOrig = img0
        "mov %3, %%rdi;"                                        /// dirDest = imgD

        "mov $0, %%r10;"                                        /// f = 0
        "bfiltrolineal_general1: "

                "mov $0, %%r11;"                                /// c = 0
                "bfiltrolineal_general2: ;"
                    "mov %1, %%r12;"                            /// dirKernel = kernel
                    "mov $0, %%rdx;"                            /// acum = 0

                    // BUCLE ESPECIFICO 1
                    "mov $-1, %%r14;"                           /// kf = -1
                    "bfiltrolineal_especifico_1: ;"

                        //BUCLE ESPECIFICO 2
                        "mov $-1, %%r15;"                       /// kc = -1
                        "bfiltrolineal_especifico_2: ;"

                            "lea (%%r10, %%r14), %%r13;"        /// fp = f + kf
                            "lea (%%r11, %%r15), %%r8;"         /// cp = c + kc

                            "cmp $0, %%r8;"
                            "jl b_especifico_and;"
                            "cmp $320, %%r8;"
                            "jge b_especifico_and;"
                            "cmp $0, %%r13;"
                            "jl b_especifico_and;"
                            "cmp $240, %%r13;"
                            "jge b_especifico_and;"

                                "mov %%r13, %%rax;"             /// rax = fp
                                "imul $320, %%rax;"             /// rax = fp * 320
                                "add %%r8, %%rax;"              /// offPixel = fp * 320 + cp

                                "xor %%rbx, %%rbx;"
                                "mov (%%rsi, %%rax), %%bl;"     /// g = [offPixel + dirOrig]

                                "mov (%%r12), %%ecx;"           /// rcx = [dirKernel]
                                "imul %%ecx, %%ebx;"            /// ebx = g * [dirKernel]
                                "add %%rbx, %%rdx;"             /// acum += g * [dirKernel]

                            "b_especifico_and: "

                            "add $4, %%r12;"                    /// dirKernel += 4

                        "inc %%r15;"
                        "cmp $1, %%r15;"
                        "jle bfiltrolineal_especifico_2;"

                    "inc %%r14;"
                    "cmp $1, %%r14;"
                    "jle bfiltrolineal_especifico_1;"

                    // SENTENCIAS ANTES DE OTRO CICLO DEL BUCLE GENERAL 2

                    "mov %%rdx, %%rax;"                         /// eax = acum
                    "mov %2, %%r9d;"                            /// r9d = norm

                    "cltd;"
                    "idiv %%r9d;"                               /// eax = acum / norm

                    "cmp $0, %%eax;"
                    "jge s_filtro_1;"
                        "mov $0, %%eax;"
                    "s_filtro_1: ;"

                    "cmp $255, %%eax;"
                    "jle s_filtro_2;"
                        "mov $255, %%eax;"
                    "s_filtro_2: ;"

                    "mov %%al, (%%rdi);"                        /// [dirDest] = acum
                    "inc %%rdi;"

                // PARTE DEL BUCLE GENERAL 2
                "inc %%r11d;"
                "cmp $320, %%r11d;"
                "jl bfiltrolineal_general2;"

        // PARTE DEL BUCLE GENERAL 1
        "inc %%r10d;"
        "cmp $240, %%r10d;"
        "jl bfiltrolineal_general1;"

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
        "mov %0, %%rsi;"                                        /// dirHOrig
        "mov %2, %%rdi;"                                        /// dirHAcum
                                                                
        "mov $0, %%r9;"                                         /// acum
                                                                
        "mov $0, %%rcx;"                                        
        "becualizar_1: ;"                                       
                "mov (%%rsi, %%rcx, 4), %%eax;"                 /// eax = [dirHorig + (n*4)]
                "add %%eax, %%r9d;"                             /// acum += [dirHorig+(n+4)]
                                                                
                "mov %%r9d, (%%rdi, %%rcx, 4);"                 /// [dirHAcum+(n*4)] = acum
        "inc %%rcx;"                                            
        "cmp $256, %%rcx;"                                      
        "jl becualizar_1;"                                      
                                                                
        "mov %1, %%r10;"                                        /// dirLut = tablaLut
                                                                
        "mov $0, %%rcx;"                                        
        "becualizar_2: ;"                                       
                "mov (%%rdi, %%rcx, 4), %%eax;"                 /// eax = [dirHAcum + (n*4)]
                "mov $256, %%ebx;"                              /// ebx = 256
                "mul %%ebx;"                                    /// eax = [dirHAcum + (n*4)] * 256
                                                                
                "mov $320*240, %%ebx;"                          /// ebx = 320*240
                                                                
                "xor %%rdx, %%rdx;"                             
                "div %%ebx;"                                    /// eax = ([dirHAcum + (n*4)] * 256) / 320*240
                                                                
                "cmp $0, %%eax;"                                
                "jle secualizar_1;"                             
                "dec %%eax;"                                    
                "secualizar_1: ;"                               
                                                                
                "mov %%al, (%%r10, %%rcx);"                     /// [dirLut + n] = g

        "inc %%rcx;"
        "cmp $256, %%rcx;"
        "jl becualizar_2;"



        :
        : "m" (histoOrig), "m" (tablaLUT), "m" (acumHisto)
        : "%rsi", "%rdi", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%rax", "%rbx", "%rcx", "%rdx", "memory"
    );

}


void imageprocess::aplicarTablaLUT(uchar * imgO, uchar * tablaLUT, uchar * imgD)
{
    asm volatile(
        "mov %0, %%rsi;"                                        /// dirOrig
        "mov %1, %%r8;"                                         /// dirLut
        "mov %2, %%rdi;"                                        /// dirDest
                                                                
        "mov $320*240, %%rcx ;"                                 
        "baplicartablalut: ;"                                   
                                                                
                "xor %%rax, %%rax;"                             
                "mov (%%rsi), %%al;"                            /// al = [dirOrig]
                "mov (%%r8, %%rax), %%al;"                      /// al = [dirLut + dirOrig]
                                                                
                "mov %%al, (%%rdi);"                            /// [rdi] = gDest

                "inc %%rsi ;"
                "inc %%rdi ;"
                "loop baplicartablalut;"


        :
        : "m" (imgO), "m" (tablaLUT), "m" (imgD)
        : "%r8", "%r9", "%rax", "%rbx", "%rcx", "memory"
    );

}


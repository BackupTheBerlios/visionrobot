/*! \file ocr_code.h
    \brief Cabecera del modulo Ocr
    \author Diego Sanchez
    \version 0.1
 */  
    
/*! \mainpage
  \section intro Introduccion
  Por comentar...

  
  \section licencia Licencia
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/ 


#ifndef _OCR_CODE_H_
#define _OCR_CODE_H_
#include "filtro_gestos_sdk.h"


//const int NUMPTOS=16;
#define NUMPTOS 16

typedef struct{
        char m_id;
        double m_list[NUMPTOS];
}hexadecagon_t;

typedef struct{
        int m_top;
        hexadecagon_t** m_list;
}constellation_t;

typedef struct{
        char*** blocks;
        int* list;
        constellation_t* dataBase;
}pack_init_t;


pack_init_t* ocr_init(const char* path);
int ocr_free_pack_init(pack_init_t* packInit);
const char* ocr_semantic_match(filtro_gestos_in_imagen_t* dibujo, pack_init_t* packInit);

#endif

		



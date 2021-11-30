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


#ifndef PRACAOC_H
#define PRACAOC_H

#include <ui_mainForm.h>
#include <ui_filtroForm.h>
#include <ui_escalaForm.h>
#include <ui_umbralForm.h>
#include <QtCore>
#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <iostream>
#include <new>
#include "imageprocess.h"


using namespace Ui;

class QFiltroFormW : public QDialog, public QFiltroForm
{
	Q_OBJECT

public:
	QFiltroFormW(QDialog *parent=0) : QDialog(parent){
		setupUi(this);
	}
};

class QEscalaFormW : public QDialog, public QEscalaForm
{
	Q_OBJECT

public:
	QEscalaFormW(QDialog *parent=0) : QDialog(parent){
		setupUi(this);
	}
};

class QUmbralFormW : public QDialog, public QUmbralForm
{
    Q_OBJECT

public:
    QUmbralFormW(QDialog *parent=0) : QDialog(parent){
        setupUi(this);
    }
};



class pracAOC:public QWidget
{
    Q_OBJECT

public:
    pracAOC();
    ~pracAOC();
		
		

private:

		QMainForm * mainWin;
		QFiltroFormW * filtroWin;
		QEscalaFormW * escalaWin;
        QUmbralFormW * umbralWin;
		QImage * qimgOrig, * qimgDest;
		uchar * imgO, * imgD;
		int histoOrig[256], histoDest[256];
		int maxGreyLevel, minGreyLevel;
		int mascara[9];
		uchar umbralOtsu;
		
		QVector<QRgb> ctable;

		bool seleccion;
		int xIni, yIni, xFin, yFin;
		
		void computeHistogram(uchar * img, int histo[256]);
        void drawHistogram(QRect qrect, int histo[256], QPainter * painter);

protected:

		void closeEvent(QCloseEvent *event);

public slots:

		void paintEvent(QPaintEvent *);
		void cargar();
		void guardar();
		void copiarOrigen();
		void copiarDestino();
		void borrar();		
		void cambiarContrasteFCuadrada();
		void cambiarContrasteFInversa();
		void cambiarEscalaGrises();
		void ecualizarHistograma();
		void filtroLineal();
		void umbralizar();

		void aceptaCambiarEscalaGrises();
		void cancelaCambiarEscalaGrises();
		
		void aceptaFiltroLineal();
		void cancelaFiltroLineal();

        void aceptaUmbralizar();
        void cancelaUmbralizar();

};




#endif

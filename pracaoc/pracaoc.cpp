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


#include "pracaoc.h"


pracAOC::pracAOC(): QWidget()
{
	mainWin=new QMainForm();
	mainWin->setupUi(this);

	filtroWin=new QFiltroFormW();
	escalaWin=new QEscalaFormW();
    umbralWin=new QUmbralFormW();
	
	ctable.resize(256);
	for(int i=0; i < 256; i++)
		ctable[i] = qRgb(i,i,i);

    imgO=new uchar[320*240 + 32];
    imgD=new uchar[320*240 + 32];

    imgO=(uchar *) (((long unsigned int) imgO + 0x20) & 0xFFFFFFFFFFFFFFE0);
    imgD=(uchar *) (((long unsigned int) imgD + 0x20) & 0xFFFFFFFFFFFFFFE0);
	
	for(int i=0; i<320*240;i++)
	{
		imgO[i]=0;
		imgD[i]=0;
	}

	qimgOrig = new QImage(imgO,320, 240, QImage::Format_Indexed8);
	qimgOrig->setColorTable(ctable);

	qimgDest = new QImage(imgD,320, 240, QImage::Format_Indexed8);
	qimgDest->setColorTable(ctable);

	
	computeHistogram(imgO, histoOrig);
	computeHistogram(imgD, histoDest);

	seleccion=false;

	connect ( mainWin->pushButtonCargar, SIGNAL (clicked()), this, SLOT( cargar() ) );
	connect ( mainWin->pushButtonGuardar, SIGNAL (clicked()), this, SLOT( guardar() ) );
	connect ( mainWin->pushButtonCopiarO, SIGNAL (clicked()), this, SLOT( copiarOrigen() ) );
	connect ( mainWin->pushButtonCopiarD, SIGNAL (clicked()), this, SLOT( copiarDestino() ) );
	connect ( mainWin->pushButtonBorrar, SIGNAL (clicked()), this, SLOT( borrar() ) );
	connect ( mainWin->pushButtonContrasteFCuadrada, SIGNAL (clicked()), this, SLOT( cambiarContrasteFCuadrada() ) );
	connect ( mainWin->pushButtonFInversa, SIGNAL (clicked()), this, SLOT( cambiarContrasteFInversa() ) );
	connect ( mainWin->pushButtonEscalaGrises, SIGNAL (clicked()), this, SLOT( cambiarEscalaGrises() ) );
	connect ( mainWin->pushButtonEcualizar, SIGNAL (clicked()), this, SLOT( ecualizarHistograma() ) );
	connect ( mainWin->pushButtonFiltroLineal, SIGNAL (clicked()), this, SLOT( filtroLineal() ) );
	connect ( mainWin->pushButtonUmbralizar, SIGNAL (clicked()), this, SLOT( umbralizar() ) );
	connect ( mainWin->pushButtonSalir, SIGNAL (clicked()), this, SLOT( close() ) );

 	connect ( escalaWin->pushButtonAceptar, SIGNAL (clicked()), this, SLOT( aceptaCambiarEscalaGrises() ) );
 	connect ( escalaWin->pushButtonCancelar, SIGNAL (clicked()), this, SLOT( cancelaCambiarEscalaGrises() ) );
 	connect ( escalaWin, SIGNAL (finished(int)), this, SLOT( cancelaCambiarEscalaGrises() ) );
	
 	connect ( filtroWin->pushButtonAceptar, SIGNAL (clicked()), this, SLOT( aceptaFiltroLineal() ) );
 	connect ( filtroWin->pushButtonCancelar, SIGNAL (clicked()), this, SLOT( cancelaFiltroLineal() ) );
 	connect ( filtroWin, SIGNAL (finished(int)), this, SLOT( cancelaFiltroLineal() ) );

    connect ( umbralWin->pushButtonAceptar, SIGNAL (clicked()), this, SLOT( aceptaUmbralizar() ) );
    connect ( umbralWin->pushButtonCancelar, SIGNAL (clicked()), this, SLOT( cancelaUmbralizar() ) );
    connect ( umbralWin, SIGNAL (finished(int)), this, SLOT( cancelaUmbralizar() ) );

}


pracAOC::~pracAOC()
{
}

void pracAOC::computeHistogram(uchar * img, int histo[256])
{
	int h, i;
	
	for(h=0; h<256;h++)
		histo[h]=0;
	
	for(i=0; i<320*240;i++)
		histo[img[i]]++;
  
}

void pracAOC::drawHistogram(QRect rect, int histo[256], QPainter * painter)
{
	int h, y, max;
	
    painter->setPen(Qt::red);
	
	max=0;
    for(h=0; h<256; h++)
		if(histo[h]>max)
            max=histo[h];
	
	if(max>0)
        for(h=0; h<=255; h++)
		{
			y=(int) rint(((float)histo[h]*rect.height())/((float) max));
            painter->drawLine(QLine(rect.x()+h, rect.y()+rect.height(),rect.x()+h, rect.y()+rect.height()-y));

		}
}

void pracAOC::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	painter.drawImage(QPoint(mainWin->frameOrig->x(), mainWin->frameOrig->y()), *qimgOrig);
	painter.drawImage(QPoint(mainWin->frameDest->x(), mainWin->frameDest->y()), *qimgDest);
	
    painter.fillRect(QRect(mainWin->frameHistoOrig->x(), mainWin->frameHistoOrig->y(), mainWin->frameHistoOrig->width(), mainWin->frameHistoOrig->height()), Qt::white);
    drawHistogram(QRect(mainWin->frameHistoOrig->x(), mainWin->frameHistoOrig->y(), mainWin->frameHistoOrig->width(), mainWin->frameHistoOrig->height()), histoOrig, &painter);
    painter.fillRect(QRect(mainWin->frameHistoDest->x(), mainWin->frameHistoDest->y(), mainWin->frameHistoDest->width(), mainWin->frameHistoDest->height()), Qt::white);
    drawHistogram(QRect(mainWin->frameHistoDest->x(), mainWin->frameHistoDest->y(), mainWin->frameHistoDest->width(), mainWin->frameHistoDest->height()), histoDest, &painter);
	painter.end();
}

void pracAOC::cargar()
{
    QImage qimgRead;
	
	QString fn = QFileDialog::getOpenFileName(this,"Elija la imagen a cargar",".", "Images (*.png *.xpm *.jpg)");
	if(qimgRead.load(fn))
	{
		QImage qimgCRead(qimgRead.width(),qimgRead.height(),QImage::Format_Indexed8);
		qimgCRead=qimgRead.convertToFormat(QImage::Format_Indexed8, ctable);
        qimgCRead = qimgCRead.scaled(QSize(320,240));

		for(int y=0; y<240 && y<qimgCRead.height();y++)
			for(int x=0; x<320 && x<qimgCRead.width(); x++)
				imgO[y*320+x]=(qimgCRead.bits())[(y*qimgCRead.width()+x)];

		computeHistogram(imgO, histoOrig);
		update();
	}
}

void pracAOC::guardar()
{
	QImage qimgWrite(320,240,QImage::Format_Indexed8);
    QString fn = QFileDialog::getSaveFileName(this,"Elija la imagen a cargar",".", "Images (*.png *.xpm *.jpg)");
	qimgWrite.setColorTable(ctable);
	for(int y=0; y<240;y++)
		for(int x=0; x<320; x++)
			(qimgWrite.bits())[(y*320+x)]=imgD[y*320+x];

	qimgWrite.save(fn);
}


void pracAOC::copiarOrigen()
{
	imageprocess::copiar(imgO,imgD);	
	computeHistogram(imgD, histoDest);
	update();
}

void pracAOC::copiarDestino()
{
	imageprocess::copiar(imgD,imgO);	
	computeHistogram(imgO, histoOrig);	
	update();
}


void pracAOC::borrar()
{
	imageprocess::borrar(imgD);
	computeHistogram(imgD, histoDest);
	update();
}

void pracAOC::cambiarContrasteFCuadrada()
{
	imageprocess::cambiarContrasteFCuadrada(imgO, imgD);
	computeHistogram(imgD, histoDest);
	update();
	
}

void pracAOC::cambiarContrasteFInversa()
{
	imageprocess::cambiarContrasteFInversa(imgO, imgD);
	computeHistogram(imgD, histoDest);
	update();
  
}

void pracAOC::cambiarEscalaGrises()
{
	if(mainWin->pushButtonEscalaGrises->isChecked())
	{
		for(int h=255; h>=0; h--)
			if(histoOrig[h]>0)
			{
				maxGreyLevel=h;
				break;
			}
		for(int h=0; h<256; h++)
			if(histoOrig[h]>0)
			{
				minGreyLevel=h;
				break;
			}
		escalaWin->horizontalScrollBarMax->setValue(maxGreyLevel);
		escalaWin->horizontalScrollBarMin->setValue(minGreyLevel);
 		escalaWin->show();
	}
 	else
 		escalaWin->hide();
  
}

void pracAOC::ecualizarHistograma()
{
	uchar tablaLUT[256];
	
	imageprocess::ecualizarHistograma(histoOrig, tablaLUT);
	imageprocess::aplicarTablaLUT(imgO, tablaLUT, imgD);
	computeHistogram(imgD, histoDest);
	update();
}

void pracAOC::filtroLineal()
{
	if(mainWin->pushButtonFiltroLineal->isChecked())
		filtroWin->show();
	else
		filtroWin->hide();
  
  
}

void pracAOC::umbralizar()
{
    if(mainWin->pushButtonUmbralizar->isChecked())
        umbralWin->show();
    else
        umbralWin->hide();

}

void pracAOC::aceptaUmbralizar()
{
    imageprocess::umbralizar(imgO,imgD,umbralWin->horizontalScrollBarUM->value(),umbralWin->horizontalScrollBarUm->value());
    computeHistogram(imgD, histoDest);
    update();
    mainWin->pushButtonUmbralizar->setChecked(false);
    umbralWin->hide();

}

void pracAOC::cancelaUmbralizar()
{
    mainWin->pushButtonUmbralizar->setChecked(false);
    umbralWin->hide();
}


void pracAOC::aceptaCambiarEscalaGrises()
{
	mainWin->pushButtonEscalaGrises->setChecked(false);
	escalaWin->hide();
    if(maxGreyLevel>minGreyLevel)
    {
        imageprocess::cambiarEscalaGrises(imgO, imgD, minGreyLevel, maxGreyLevel, escalaWin->horizontalScrollBarMin->value(), escalaWin->horizontalScrollBarMax->value());
    }
	computeHistogram(imgD, histoDest);
	update();

}

void pracAOC::cancelaCambiarEscalaGrises()
{
 	mainWin->pushButtonEscalaGrises->setChecked(false);
 	escalaWin->hide();
}
	
void pracAOC::aceptaFiltroLineal()
{
 	mainWin->pushButtonFiltroLineal->setChecked(false);
 	filtroWin->hide();
	mascara[0]=filtroWin->filterV11->value();
	mascara[1]=filtroWin->filterV12->value();
	mascara[2]=filtroWin->filterV13->value();
	mascara[3]=filtroWin->filterV21->value();
	mascara[4]=filtroWin->filterV22->value();
	mascara[5]=filtroWin->filterV23->value();
	mascara[6]=filtroWin->filterV31->value();
	mascara[7]=filtroWin->filterV32->value();
	mascara[8]=filtroWin->filterV33->value();
	int norm = filtroWin->filterNorm->value();


	imageprocess::filtroLineal(imgO, mascara, norm, imgD);
	computeHistogram(imgD, histoDest);
	update();

}

void pracAOC::cancelaFiltroLineal()
{
 	mainWin->pushButtonFiltroLineal->setChecked(false);
 	filtroWin->hide();
}

void pracAOC::closeEvent(QCloseEvent *event)
{
    (void)event;

	delete filtroWin;
	delete escalaWin;
	delete mainWin;

}

#include "convert.h"
#include <QDebug>

#include <QFileInfo>
#include <QImage>

Convert::Convert(QString srcFileName, QString outFileDir, QString outFileName, bool dither)
{
    this->dither = dither;
    this->out_name = outFileName;
    this->path = srcFileName;
    this->outpit_dir = outFileDir;
    
    QImage img(this->path);
    this->w = img.width();
    this->h = img.height();
    
    QFileInfo inf(this->path);
    if (inf.fileName().split(".").back().toLower() == "png") {
        
    }
    else if (inf.fileName().split(".").back().toLower() == "bmp") {
        
    }
    else if (inf.fileName().split(".").back().toLower() == "jpg") {
        
    }
    else if (inf.fileName().split(".").back().toLower() == "jpeg") {
        
    }
    else {
        qDebug() << "is a not supported image type. use png, jpg, jpeg or bmp";
        return;
    }
    
    this->r_earr.clear();
    this->g_earr.clear();
    this->b_earr.clear();

    this->r_nerr = 0;
    this->g_nerr = 0;
    this->b_nerr = 0;
}

void Convert::convert(quint8 cf, quint8 alpha)
{
    this->cf = cf;
    this->alpha = alpha;
    
    if (this->cf == CF_RAW || this->cf == CF_RAW_ALPHA || this->cf == CF_RAW_CHROMA) {
        QFile fil(this->path);
        if (!fil.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open file!";
        }
        
        QDataStream in(&fil);
        int i = 0;
        while (!fil.atEnd())
        {
            in.readRawData(&d_out[i], 1);
            qDebug() << QString("0x%1").arg(d_out[i], 2, 16, QLatin1Char('0')).toUpper();
        }
        
        fil.close();
        return;
    }
    
    quint16 palette_size = 0;
    if(this->cf == CF_INDEXED_1_BIT) palette_size = 2;
    if(this->cf == CF_INDEXED_2_BIT) palette_size = 4;
    if(this->cf == CF_INDEXED_4_BIT) palette_size = 16;
    if(this->cf == CF_INDEXED_8_BIT) palette_size = 256;
    
    if (palette_size) {
        
    }
}

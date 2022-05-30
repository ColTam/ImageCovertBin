#ifndef CONVERT_H
#define CONVERT_H

#include <QDataStream>
#include <QObject>


#define CF_TRUE_COLOR_332 0      /*Helper formats. Used internally*/
#define CF_TRUE_COLOR_565 1
#define CF_TRUE_COLOR_565_SWAP 2
#define CF_TRUE_COLOR_888 3
#define CF_ALPHA_1_BIT 4
#define CF_ALPHA_2_BIT 5
#define CF_ALPHA_4_BIT 6
#define CF_ALPHA_8_BIT 7
#define CF_INDEXED_1_BIT 8
#define CF_INDEXED_2_BIT 9
#define CF_INDEXED_4_BIT 10
#define CF_INDEXED_8_BIT 11
#define CF_RAW 12
#define CF_RAW_ALPHA 13
#define CF_RAW_CHROMA 12

#define CF_TRUE_COLOR 100          /*Helper formats is C arrays contains all treu color formats (usin in "download")*/
#define CF_TRUE_COLOR_ALPHA 101
#define CF_TRUE_COLOR_CHROMA 102

class Convert
{
public:
    explicit Convert(QString srcFileName, QString outFileDir, QString outFileName, bool dither);
    
    bool dither;
    int w;
    int h;
    quint8 cf;
    quint8 alpha;
    quint32 chroma;
    QVector<char> d_out;
    QDataStream img;
    QString out_name;
    QString outpit_dir;
    QString path;
    
    /*Helper variables*/
    quint8 r_act;
    quint8 b_act;
    quint8 g_act;

    /*For dithering*/
    QVector<quint8> r_earr;  /*Classification error for next row of pixels*/
    QVector<quint8> g_earr;
    QVector<quint8> b_earr;

    quint8 r_nerr;  /*Classification error for next pixel*/
    quint8 g_nerr;
    quint8 b_nerr;
    
    void convert(quint8 cf, quint8 alpha = 0);
    
signals:
    
};

#endif // CONVERT_H

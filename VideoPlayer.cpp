#include "VideoPlayer.h"
#include "ui_VideoPlayer.h"

extern "C" {
    #include"libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libavcodec/avcodec.h"
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


#ifndef PHOTO_FRAME_H
#define PHOTO_FRAME_H

#include "stdafx.h"

extern Mat image[];
extern int currentstep;
namespace Ui {
class photo_frame;
}

class photo_frame : public QDialog
{
    Q_OBJECT

public:
    explicit photo_frame(QWidget *parent = 0);
    ~photo_frame();

    void frame_load(Mat);
    void frame_update(Mat);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    QImage Mat2QImage(Mat src);


private slots:
    void on_zoomin_btn_clicked();

    void on_zoomout_btn_clicked();

    void on_fit_btn_clicked();

    void on_ori_btn_clicked();

    void on_clockwise_btn_clicked();

    void on_c_clockwise_btn_clicked();

private:
    Ui::photo_frame *ui;

    QPixmap Pixmap;

};

#endif // PHOTO_FRAME_H

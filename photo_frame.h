#ifndef PHOTO_FRAME_H
#define PHOTO_FRAME_H

#include "stdafx.h"

extern Mat image[];
extern int step;
extern int pers_triger;
namespace Ui {
class photo_frame;
}

class photo_frame : public QDialog
{
    Q_OBJECT

public:
    explicit photo_frame(QWidget *parent=0);
    ~photo_frame();

    void frame_load(Mat);//프레임 로드
    void frame_update(Mat);//프레임 업데이트
    //마우스에 관련한 QT 함수
    void mousePressEvent(QMouseEvent *event);//마우스 누를 때
    void mouseMoveEvent(QMouseEvent *event);//마우스 움직일 때
    void mouseReleaseEvent(QMouseEvent *event);//마우스 놓을 때
    //원근변환 관련 함수
    Mat getPerspectiveMatrix(); //원근 변환 그리기
    void drawSelectedPoints(); //점 찍는 함수
    void onMouse(int , int , int , int , void*);

public slots:
    QImage Mat2QImage(Mat src);


private slots:
    void on_zoomin_btn_clicked();//줌인
    void on_zoomout_btn_clicked();//줌아웃
    void on_clockwise_btn_clicked();//시계 방향
    void on_c_clockwise_btn_clicked();//시계 반대 방향
    void on_fit_btn_clicked();//화면에 딱 맞추기
    void on_ori_btn_clicked();//원래 크기로

private:
    Ui::photo_frame *ui;

    QPixmap Pixmap;

};

#endif // PHOTO_FRAME_H

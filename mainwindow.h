#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"
#include "affdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();

public slots:




private:
    Ui::MainWindow *ui;
    QTimer *myTimer;

private slots:
    void calc_Histo(const Mat&, Mat &, int, int);
    void draw_histo(Mat, Mat &, Size);
    Mat make_palatte(int);
    void draw_histo_hue(Mat, Mat &, Size);

    void photo_window_update(Mat, int);//화면 업데이트
    void un_redo();//버튼 활성화
    void detectAndDisplay(Mat);//얼굴 찾기
    void FLEX(Mat);//선글라스 씌우기
    void overlayImage(const Mat &, const Mat &, Mat &, Point2i );

    int search_valueIdx(Mat, int);
    void on_contrast_slider_sliderReleased();//명암 슬라이더
    void on_set_con_btn_clicked();//명암 초기화 버튼
    void on_brightness_slider_sliderReleased();//밝기 슬라이더
    void on_set_bright_btn_clicked();//밝기 초기화 버튼
    void on_eros_btn_clicked();//침식 버튼
    void on_dilat_btn_clicked();//팽창 버튼
    void on_open_btn_clicked();//열기 버튼
    void on_close_btn_clicked();//닫기 버튼
    void on_affin_btn_clicked();//아핀 회전
    void on_perspec_btn_clicked();//원근 변환
    void on_actionOpen_File_triggered();//파일열기
    void on_actionSave_File_triggered();//파일닫기
    void on_actionUndo_triggered();//언두
    void on_actionRedo_triggered();//리두
    void on_sharpen_btn_clicked();//라플라시안 적용
    void on_blur_btn_clicked();//가우시안 필터 적용
    void on_magic_set_btn_clicked();//옵션박스
    void on_gray_btn_clicked();//회색 스케일
    void on_edge_btn_clicked();//에지 검출
    void on_normal_btn_clicked();//정규화
    void on_equal_btn_clicked();//평활화
    void on_stret_btn_clicked();//스트레칭
    void on_draw_btn_clicked();//히스토그램 맞춰서 그려주기
    void on_actionSharpen_triggered();
    void on_actionBlur_triggered();
    void on_actionGray_level_triggered();
    void on_actionAffine_Rotation_triggered();
    void on_actionEdge_detection_triggered();
    void on_actionPerspective_Transform_triggered();

    void on_actionErosion_triggered();
    void on_actionDilation_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionNormalize_triggered();
    void on_actionEqualize_triggered();
    void on_actionStretching_triggered();
    void on_actionDraw_Histogram_triggered();
};

#endif // MAINWINDOW_H

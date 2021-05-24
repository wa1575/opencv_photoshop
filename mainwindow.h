#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:




private:
    Ui::MainWindow *ui;
    QTimer *myTimer;


private slots:
    void calc_Histo(const Mat&, Mat &, int , int );
    void draw_histo(Mat, Mat &, Size);
    Mat make_palatte(int);
    void draw_histo_hue(Mat, Mat &, Size);

    void photo_window_update(Mat, int);
    void un_redo();
    void detectAndDisplay(Mat);
    void FLEX(Mat);
    void overlayImage(const Mat &, const Mat &, Mat &, Point2i );


    void on_actionOpen_File_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionSave_File_triggered();
    void on_sharpen_btn_clicked();
    void on_blur_btn_clicked();
    void on_magic_set_btn_clicked();
    void on_contrast_slider_sliderReleased();
    void on_brightness_slider_sliderReleased();
    void on_set_bright_btn_clicked();
    void on_set_con_btn_clicked();
    void on_gray_btn_clicked();
    void on_edge_btn_clicked();
    void on_eros_btn_clicked();
    void on_dilat_btn_clicked();
    void on_open_btn_clicked();
    void on_close_btn_clicked();
    void on_normal_btn_clicked();
    void on_equal_btn_clicked();
    void on_stret_btn_clicked();
    void on_draw_btn_clicked();
    void on_affin_btn_clicked();
    void on_perspec_btn_clicked();
};

#endif // MAINWINDOW_H

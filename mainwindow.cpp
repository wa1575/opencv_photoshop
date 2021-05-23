#include "mainwindow.h"
#include "ui_mainwindow.h"

Mat image[100];
int currentstep=0;
//얼굴과 눈 간격을 찾게 해주는 소스
String face_cascade_name = "C://opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "C://opencv/sources/data/lbpcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String glassesImage = "sunglasses.png";//선글라스 이미지
Mat glasses;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Tools"));
    //opencv 버전에 따른 예외처리
    if(!face_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml") )
        ui->label_2->setText("--(!)Error loading face cascade\n");
    if(!eyes_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml") )
        ui->label_2->setText("--(!)Error loading eyes cascade\n");

    ui->brightness_slider->setMinimum(-20);
    ui->brightness_slider->setMaximum(20);

    ui->contrast_slider->setMinimum(-20);
    ui->contrast_slider->setMaximum(20);

    ui->brightness_slider->setValue(0);
    ui->contrast_slider->setValue(0);

    myTimer = new QTimer(this);
    myTimer->start(500);                   //1초 간격 타이머
    connect(myTimer,SIGNAL(timeout()),this,SLOT(un_redo()));
}


MainWindow::~MainWindow()
{   //저장하기
    QMessageBox message(QMessageBox::NoIcon, "Save", "Do you want to save photo?", QMessageBox::Yes | QMessageBox::No, NULL);
    if(message.exec() == QMessageBox::Yes)
    {
        if(!image[currentstep].empty()){
            QString Simgpath=QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("PNG(*.png);;JPG(*.jpg)"));
            if(!Simgpath.isNull()){
                imwrite(Simgpath.toStdString(),image[currentstep]);
            }
        }
    }
    for(int i=0;i<100;i++){
        image[i].release();
    }
    delete ui;
}
//qt 화면에 표시해주는 함수
void MainWindow::photo_window_update(Mat frame,int r){
    static photo_frame P_frame;
    P_frame.frame_update(frame);
    P_frame.show();
    if(r==1){
        P_frame.frame_load(frame);
    }
    else if(r==0){
        P_frame.frame_update(frame);
    }
}

//undu와 redu 함수
void MainWindow::un_redo(){
    if(image[currentstep].empty()){
        ui->blur_btn->setEnabled(false);
        ui->brightness_slider->setEnabled(false);
        ui->contrast_slider->setEnabled(false);
        ui->magicbox->setEnabled(false);
        ui->magic_set_btn->setEnabled(false);
        ui->set_bright_btn->setEnabled(false);
        ui->set_con_btn->setEnabled(false);
        ui->sharpen_btn->setEnabled(false);
        ui->edge_btn->setEnabled(false);
        ui->gray_btn->setEnabled(false);
        ui->close_btn->setEnabled(false);
        ui->open_btn->setEnabled(false);
        ui->dilat_btn->setEnabled(false);
        ui->eros_btn->setEnabled(false);
        ui->draw_btn->setEnabled(false);
        ui->stret_btn->setEnabled(false);
        ui->equal_btn->setEnabled(false);
        ui->normal_btn->setEnabled(false);
    }
    else{
        ui->blur_btn->setEnabled(true);
        ui->brightness_slider->setEnabled(true);
        ui->contrast_slider->setEnabled(true);
        ui->magicbox->setEnabled(true);
        ui->magic_set_btn->setEnabled(true);
        ui->set_bright_btn->setEnabled(true);
        ui->set_con_btn->setEnabled(true);
        ui->sharpen_btn->setEnabled(true);
        ui->edge_btn->setEnabled(true);
        ui->gray_btn->setEnabled(true);
        ui->close_btn->setEnabled(true);
        ui->open_btn->setEnabled(true);
        ui->dilat_btn->setEnabled(true);
        ui->eros_btn->setEnabled(true);
        ui->draw_btn->setEnabled(true);
        ui->stret_btn->setEnabled(true);
        ui->equal_btn->setEnabled(true);
        ui->normal_btn->setEnabled(true);
    }
    if(currentstep>0)
        ui->actionUndo->setEnabled(true);
    else
        ui->actionUndo->setEnabled(false);

    if(!image[currentstep+1].empty())
        ui->actionRedo->setEnabled(true);
    else
        ui->actionRedo->setEnabled(false);
    ui->label->setText("Step: "+QString::number(currentstep));
}

//이미지 열기
void MainWindow::on_actionOpen_File_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Select an Image"),"/home","Images(*.png *.bmp *.jpg)");
    if(!filename.isNull()){
        for(int i=0;i<100;i++){
            image[i].release();
        }
        image[0]=imread(filename.toStdString());
        currentstep=0;
        photo_window_update(image[currentstep],1);
        un_redo();
    }
}


void MainWindow::on_actionUndo_triggered()
{
    currentstep--;
    un_redo();
    photo_window_update(image[currentstep],0);
}
void MainWindow::on_actionRedo_triggered()
{
    currentstep++;
    un_redo();
    photo_window_update(image[currentstep],0);
}

void MainWindow::on_actionSave_File_triggered()
{
    if(image[currentstep].empty()){
        QMessageBox msgBox;
        msgBox.setText("No flie loaded");
        msgBox.exec();

    }
    else{
        QString Simgpath=QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("PNG(*.png);;JPG(*.jpg)"));
        if(!Simgpath.isNull()){
            imwrite(Simgpath.toStdString(),image[currentstep]);
        }
    }
}

//blur버튼 클릭 - 뿌옇게
void MainWindow::on_blur_btn_clicked()
{   //가우시안blur 발동
    GaussianBlur(image[currentstep],image[currentstep+1],Size(9,9),0,0);
    currentstep++;
    //적용
    photo_window_update(image[currentstep],0);
    un_redo();
}

//선명하게 - 라플라시안
void MainWindow::on_sharpen_btn_clicked()
{
    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);//라플라시안
    filter2D(image[currentstep], image[currentstep+1], image[currentstep].depth(), kernel);
    currentstep++;
    photo_window_update(image[currentstep],0);
    un_redo();
}

//옵션박스
void MainWindow::on_magic_set_btn_clicked()
{
    int index;
    index= ui->magicbox->currentIndex();
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    switch(index){
    case 0:
        if(image[currentstep].channels()!=1){
            cvtColor(image[currentstep],image[currentstep+1], COLOR_BGR2GRAY,1);
            currentstep++;
        }
        break;
    case 1:
        if(image[currentstep].channels()==1){
            equalizeHist(image[currentstep],image[currentstep+1]); //평활화
            currentstep++;
        }
        else{
            //회색 채널이 아니면 에러가 나게
            QMessageBox::about(NULL,"Attention","Convert to gray image first.");
        }
        break;
    case 2:
        //x방향 기울기
        Sobel( image[currentstep], grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );
        //y방향 기울기
        Sobel( image[currentstep], grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT );
        convertScaleAbs( grad_y, abs_grad_y );
        //그라디언트 병합
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, image[currentstep+1] );
        currentstep++;
        break;
    case 3:
        detectAndDisplay(image[currentstep]);
        break;
    case 4:
        FLEX(image[currentstep]);
        break;
    default:
        break;
    }
    photo_window_update(image[currentstep],0);
    un_redo();
}


void MainWindow::on_contrast_slider_sliderReleased()
{
    int value=ui->contrast_slider->value();
    double alpha=double(value+10)/10;
    int beta=0;
    Mat preview;
    image[currentstep].convertTo(preview, -1, alpha, beta);

    photo_window_update(preview,0);
    un_redo();
}


void MainWindow::on_brightness_slider_sliderReleased()
{
    int value=ui->brightness_slider->value();
    double alpha=1;
    int beta=value*10; //이정도가 적당함
    Mat preview;
    image[currentstep].convertTo(preview, -1, alpha, beta);

    photo_window_update(preview,0);
    un_redo();
}


void MainWindow::on_set_bright_btn_clicked()
{
    ui->brightness_slider->setValue(0);
    int value=ui->brightness_slider->value();
    double alpha=1;
    int beta=value*10;
    image[currentstep].convertTo(image[currentstep+1], -1, alpha, beta);
    currentstep++;

    photo_window_update(image[currentstep],0);
    ui->brightness_slider->setValue(0);
}

void MainWindow::on_set_con_btn_clicked()
{
    ui->contrast_slider->setValue(0);
    int value=ui->contrast_slider->value();
    double alpha=double(value+10)/10;
    int beta=0;

    image[currentstep].convertTo(image[currentstep+1], -1, alpha, beta);
    currentstep++;

    photo_window_update(image[currentstep],0);
    ui->contrast_slider->setValue(0);
}

//얼굴찾는 함수 사용법
void MainWindow::detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //얼굴감지
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(10, 10) );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 0, 0, 255 ), 2, 8, 0 );
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        /*
        //각 얼굴마다 눈 차지
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        for ( size_t j = 0; j < eyes.size(); j++ ){
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
        */
    }
    image[currentstep+1]=frame.clone();
    currentstep++;
}


void MainWindow::FLEX(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    vector<Point> points;
    //선글라스 찾기
    glasses = imread(glassesImage, IMREAD_UNCHANGED);
    if (glasses.empty()) {
        cout << "Could not read image - " << glassesImage << endl;
    }

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //얼굴감지
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(10, 10) );
    for ( size_t i = 0; i < faces.size(); i++ ){
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        //각 얼굴마다 눈 차지
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        for ( size_t j = 0; j < eyes.size(); j++ ){
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            //썬글라스 추가하기
            Point p(eye_center.x, eye_center.y);
            points.push_back(p);
         }
        if(points.size()==2){
            Point center1 = points[0];
            Point center2 = points[1];

            if(center1.x > center2.x){
                Point temp;
                temp = center1;
                center1=center2;
                center2=temp;
            }//눈 위치가 2개로 검출된 경우 x좌표 기준 정렬
         int width = abs(center2.x-center1.x);//눈 위치가 가로가 아닐경우
         int height = abs(center2.y-center1.y);//가로와 세로이로 판명
        if(width > height){
            float imgScale = width / 600.0;// 눈사이 간격과 알 사이 간격 계산
            int w = glasses.cols * imgScale;
            int h = glasses.rows * imgScale;
            int offsetX= 600*imgScale;
            int offsetY= 450*imgScale;

            Mat resized_glasses;
            cv::resize(glasses, resized_glasses, Size(w,h), 0, 0); //cv::붙여야 에러없음?
            overlayImage(image[currentstep], resized_glasses, frame, Point(center1.x - offsetX, center1.y - offsetY));
        }
        }
    }
    //결과 클론해서 넣기
    image[currentstep+1]=frame.clone();
    currentstep++;
}

//이미지 오버레이 하는 법 참조 https://docs.opencv.org/3.3.1/db/deb/tutorial_display_image.html
void MainWindow::overlayImage(const Mat &background, const Mat &foreground,Mat &output, Point2i location){
        background.copyTo(output);
        // 위치가 나타내는 행 아니면 최소 0에서 시작
        for (int y = std::max(location.y, 0); y < background.rows; ++y){
            int fY = y - location.y;
            // 행이 붙일 이미지보다 커지면 끝냅니다.
            if (fY >= foreground.rows) {
                break;
            }

            // 위치로 표시된 열에서 시작하거나 location x가 음수이면 0에서 시작합니다.
            for (int x = std::max(location.x, 0); x < background.cols; ++x)
            {
                int fX = x - location.x;
                //열이 붙일 이미지보다 커지면 끝냅니다.
                if (fX >= foreground.cols) {
                    break;
                }
                // 4번째(알파채널)을 사용하여 앞쪽 픽셀에 붙여 붙일 사진의 불투명도(opacity)를 결정합니다.
                double opacity =
                    ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])/ 255.;
                // 불투명도를 적용하여 앞쪽 픽셀에다가 사진을 결합시킵니다.
                // 다만, 불투명도가 0보다 큰 경우에만 적용됩니다.
                for (int c = 0; opacity > 0 && c < output.channels(); ++c){
                    unsigned char foregroundPx =
                        foreground.data[fY * foreground.step + fX * foreground.channels() + c];
                    unsigned char backgroundPx =
                        background.data[y * background.step + x * background.channels() + c];
                    output.data[y*output.step + output.channels()*x + c] =
                        backgroundPx * (1. - opacity) + foregroundPx * opacity;
                }
            }
        }
    }

void MainWindow::on_gray_btn_clicked()
{
    //기본
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    //gray 전환
    if(image[currentstep].channels()!=1){
        cvtColor(image[currentstep],image[currentstep+1], COLOR_BGR2GRAY,1);
        currentstep++;
    }
    //update하기
    photo_window_update(image[currentstep],0);
    un_redo();

}


void MainWindow::on_edge_btn_clicked()
{
    //기본
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    //x방향 기울기
    Sobel( image[currentstep], grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    //y방향 기울기
    Sobel( image[currentstep], grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    //그라디언트 병합
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, image[currentstep+1] );
    currentstep++;

    //update하기
    photo_window_update(image[currentstep],0);
    un_redo();

}


void MainWindow::on_eros_btn_clicked()
{

}


void MainWindow::on_dilat_btn_clicked()
{

}


void MainWindow::on_open_btn_clicked()
{

}


void MainWindow::on_close_btn_clicked()
{

}


void MainWindow::on_normal_btn_clicked()
{

}


void MainWindow::on_equal_btn_clicked()
{

}


void MainWindow::on_stret_btn_clicked()
{

}


void MainWindow::on_draw_btn_clicked()
{
    //기본
    Mat hist, hist_img, hue_hist, hue_hist_img;
    Mat HSV_img, HSV_arr[3];

      //흑백일 시 계산 및 그리기 함수
    if(image[currentstep].channels()==1){
        calc_Histo(image[currentstep], hist, 256, 256);
        draw_histo(hist, hist_img, Size(256,200));

        imshow("his_img", hist_img);
    }
     //컬러일 시 계산 및 그리기 함수
    else{
        cvtColor(image[currentstep], HSV_img, COLOR_BGR2HSV);
        split(HSV_img, HSV_arr);
        calc_Histo(HSV_arr[0], hue_hist, 18, 180);
        draw_histo(hue_hist, hue_hist_img, Size(360,200));

        imshow("hue_his_img", hue_hist_img);
    }
    //update하기
    photo_window_update(image[currentstep],0);
    un_redo();
}



void MainWindow::calc_Histo(const Mat& image, Mat &hist, int bins, int range_max=256){
    int histSize[] = { bins };
    float range[] = {0, (float)range_max };
    int channels[] = { 0 };
    const float* ranges[] = { range };

    calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
}

void MainWindow::draw_histo(Mat hist, Mat &hist_img, Size size= Size(256,200)){
    hist_img = Mat(size, CV_8U, Scalar(255));
    float bin = (float)hist_img.cols / hist.rows;
    normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX);

    for(int i=0; i<hist.rows; i++){
        float start_x = i*bin;// 막대 사각형 시작 x 좌표
        float end_x = (i+1) * bin; // 막대 사각형 종료 x 좌표
        Point2f pt1(start_x,0);
        Point2f pt2(end_x, hist.at<float>(i));
        //막대 사각형 그리기
        if(pt2.y >0) rectangle(hist_img, pt1, pt2, Scalar(0), -1);
    }
    flip(hist_img, hist_img, 0);//뒤집기
}
//컬러 그리기
Mat MainWindow:: make_palatte(int rows){
    Mat hsv(rows, 1, CV_8UC3);
    for(int i =0; i< rows;i++){
        uchar hue = saturate_cast<uchar>((float)i/rows*180);
        hsv.at<Vec3b>(i) = Vec3b(hue,255,255);
    }
    cvtColor(hsv, hsv, COLOR_HSV2BGR);
    return hsv;
}
void MainWindow::draw_histo_hue(Mat hist, Mat &hist_img, Size size = Size(256,200)){
    Mat hsv_palatte = make_palatte(hist.rows);

    hist_img = Mat(size, CV_8UC3, Scalar(255,255,255));
    float bin = (float)hist_img.cols / hist.rows;
    normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX); //정규화

    for(int i=0; i<hist.rows; i++){
        float start_x = i*bin;// 막대 사각형 시작 x 좌표
        float end_x = (i+1) * bin; // 막대 사각형 종료 x 좌표
        Point2f pt1(start_x,0);
        Point2f pt2(end_x, hist.at<float>(i));
        //막대 사각형 그리기
        Scalar color = hsv_palatte.at<Vec3b>(i);
        if(pt2.y >0) rectangle(hist_img, pt1, pt2, color, -1);
    }
    flip(hist_img, hist_img, 0);//뒤집기

}

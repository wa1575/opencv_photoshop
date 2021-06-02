#include "photo_frame.h"
#include "ui_photo_frame.h"

//줌 관련 변수
float scope = 1; //줌 배율
//이미지의 픽셀 너비, 높이 계산하기 위한 변수
int pix_width = 0; //이미지 넓이 초기화
int pix_height = 0;//이미지 높이 초기화
//마우스 이벤트 시작점에 관한 변수
int start_x;
int start_y;

photo_frame::photo_frame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::photo_frame)
{    
    ui->setupUi(this);//ui팝업
    ui->label->setMouseTracking(true);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

}
//프레임에 이미지 불러오기 QT형식
void photo_frame::frame_load(Mat frame){
    if(!frame.empty()){
        QString size;//사이즈 확인
        Pixmap=QPixmap::fromImage(Mat2QImage(frame));
        //QT에서 이미지의 너비와 높이 받기
        pix_width = Pixmap.width();
        pix_height= Pixmap.height();
        size = QString::number(pix_width) + "x" + QString::number(pix_height);
        this->setWindowTitle(size);//타이틀 이름은 사이즈로 표시
        int wind_height = 900;//초기값 900 고정
        int wind_width = pix_width * wind_height / pix_height;
        //윈도우에 화면 크기 및 출력위치
        this->setGeometry(100,100,wind_width,wind_height);
        on_fit_btn_clicked();
    }
}
//프레임에 이미지 업데이트 해주기
void photo_frame::frame_update(Mat frame){
    if(!frame.empty()){
        Pixmap=QPixmap::fromImage(Mat2QImage(frame));
        QString size;
        pix_width =Pixmap.width();//다시 이미지 너비를 받아옴
        pix_height=Pixmap.height();//다시 이미지 높이를 받아옴
        size = QString::number(pix_width) + "x" + QString::number(pix_height);
        this->setWindowTitle(size);//사이즈 조정 후 이름 변경
        int pix_height_t = float(pix_height) * scope;//줌 한 정도 만큼 이미지 업데이트
        QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
        ui->label->setPixmap(ZPixmap);//UI에 반영
    }
}

QImage photo_frame::Mat2QImage(const Mat src){
    if(src.type()==CV_8UC1){//qt에서 이미지 불러와서 버퍼에 넣기
        QVector<QRgb> colorTable;
        for(int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        const uchar *qImageBuffer = (const uchar*)src.data;
        QImage img(qImageBuffer,src.cols,src.rows,src.step,QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img; //경로에서 이미지 파일을 불러와 이미지로 반환
    }
    if(src.type()==CV_8UC3){//컬러라면 컬러(rgb)로 이미지 불러옴
        const uchar *qImageBuffer = (const uchar*)src.data;
        QImage img(qImageBuffer,src.cols,src.rows,src.step,QImage::Format_RGB888);//RGB로 이미지 불러오기
        return img.rgbSwapped();
    }
    else{
        cout<<"occurred Image loading Error!" << endl;
        return QImage();
    }
}

photo_frame::~photo_frame()
{
    delete ui;
}


//줌인 버튼 클릭 시
void photo_frame::on_zoomin_btn_clicked()
{   //최대로 확대할 수 있는 부분을 정해놓음
    if(scope < 2){
        scope = scope + 0.05;}
    int pix_height_t = int( pix_height * scope );
    QPixmap ZPixmap = Pixmap.scaledToHeight( pix_height_t );
    ui->label->setPixmap( ZPixmap );
}
//줌 아웃 버튼 클릭 시
void photo_frame::on_zoomout_btn_clicked()
{   //최소로 축소할 수 있는 부분을 정해놓음
    if(scope > 0.1){
        scope = scope - 0.05;}
    int pix_height_t = int(pix_height*scope);

    QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
    ui->label->setPixmap(ZPixmap);

}
//이미지를 화면에 딱 맞추기
void photo_frame::on_fit_btn_clicked()
{   //숫자는 화면 보면서 조정
    float pix_height_f = float(ui->scrollArea->height())-40;//대략적인 화면 UI 추출
    scope = pix_height_f/float(pix_height);//화면 크기에 맞추게 만들기
    int pix_height_t = pix_height * scope; //화면 고정
    QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
    if(ui->scrollArea->width() < ZPixmap.width()){
        if(!this->isMaximized()){//최대가 아니라면 딱 맞출 수 있게 키워 줌
       this->resize(ZPixmap.width()+20, this->height());}
    }
    ui->label->setPixmap(ZPixmap);//업데이트
}
//이미지의 원래 크기로 돌려놓기
void photo_frame::on_ori_btn_clicked()
{
    scope = 1;//줌 배율 1로 복귀
    int pix_height_t = int(pix_height*scope);//배율에 다시 맞추기
    QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
    ui->label->setPixmap(ZPixmap);
}


void photo_frame::mouseMoveEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    //최소한 int형이므로 1은 나오도록 만듬
    int P_x = (ui->scrollArea->width() - pix_width*scope)/2 + 1;
    int P_y = (ui->scrollArea->height() - pix_height*scope)/2 + 1;

    int realx = (x-P_x) / scope;
    int realy = (y-P_y-ui->scrollArea->y()) / scope;
    if(realx>Pixmap.width())
        realx = Pixmap.width();
    else if(realx < 0)
        realx = 0;
    if(realy > Pixmap.height())
        realy = Pixmap.height();
    else if(realx < 0)
        realy = 0;
    if(pers_triger != 1){
    Mat drawrec;
    drawrec=image[step].clone();
    rectangle(drawrec, Point(start_x,start_y), Point(realx,realy), Scalar(0,255,0), 3);
    frame_update(drawrec);
    }
}


Point2f p[4];
int nPoint = 0;
Mat disply_image;
//원근 매트릭스 구하기
Mat photo_frame::getPerspectiveMatrix() {
    Mat m;
    if (nPoint == 4) {
        int L1 = sqrt(pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y, 2));//거리 계산1
        int L2 = sqrt(pow(p[2].x - p[1].x, 2) + pow(p[2].y - p[1].y, 2));//거리 계산2
        Point2f p_new[4];
        p_new[1] = p[1];
        p_new[0].x = p[1].x;
        p_new[0].y = p[1].y - L1;
        p_new[2].x = p[1].x + L2;
        p_new[2].y = p[1].y;
        p_new[3].x = p[1].x + L2;
        p_new[3].y = p[1].y - L1;
        m = getPerspectiveTransform(p, p_new);
    }
    return m;
}

void photo_frame::drawSelectedPoints() {
    int size_srect = 20;
    for (int i = 0; i < nPoint; i++) {
        Rect rect(p[i].x - size_srect / 2, p[i].y - size_srect / 2, size_srect, size_srect);
        disply_image=image[step].clone();
        rectangle(disply_image, rect, Scalar(0, 0, 255), 8); //크기
    }
    frame_update(disply_image);
}


void photo_frame::mousePressEvent(QMouseEvent *event){
        //qt에서 x, y 좌표 가져오기
        int x = event -> x();
        int y = event -> y();
        int P_x=(ui->scrollArea->width()-pix_width * scope)/2 + 1; //scope 비율에 맞춤
        int P_y=(ui->scrollArea->height()-pix_height * scope)/2 + 1;

        start_x=(x-P_x) / scope;//배율에 맞춤 x좌표 조율
        start_y=(y-P_y-ui->scrollArea->y()) / scope;//배율에 맞춤 y좌표 조율
        if(start_x > Pixmap.width())//x 좌표값 보정
            start_x = Pixmap.width();
        else if(start_x < 0)
            start_x=0;
        if(start_y > Pixmap.height())//y 좌표값 보정
            start_y = Pixmap.height();
        else if(start_y < 0)
            start_y = 0;
        if(pers_triger == 1){
            p[nPoint].x = start_x, p[nPoint].y = start_y;
            nPoint = (nPoint + 1) > 4 ? 4 : (nPoint + 1);//사각형으로 만들기
            drawSelectedPoints();
            if (nPoint == 4) {
                Mat m = getPerspectiveMatrix();
                warpPerspective(image[step], image[step+1], m, Size(image[step].cols, image[step].rows));
                step++;
                frame_load(image[step]);
            }
        }
}

void photo_frame::mouseReleaseEvent(QMouseEvent *event){
    if(pers_triger!=1){
    int x = event->x();
    int y = event->y();
    int P_x=(ui->scrollArea->width() - pix_width*scope) /2 + 1;
    int P_y=(ui->scrollArea->height() - pix_height*scope)/2 + 1;
    int end_pointX = (x-P_x)/scope;
    int end_pointY = (y-P_y-ui->scrollArea->y())/scope;
    if(end_pointX > Pixmap.width())
        end_pointX = Pixmap.width();
    else if(end_pointX < 0)
        end_pointX = 0;
    if(end_pointY > Pixmap.height())
        end_pointY = Pixmap.height();
    else if(end_pointY<0)
        end_pointY=0;
    //qt로 확인창 띄우기
    QMessageBox message(QMessageBox::NoIcon, "Cut", "Do you want to cutting the Area?", QMessageBox::Yes | QMessageBox::No, NULL);
    if(message.exec() == QMessageBox::Yes)
    {
        image[step+1] = image[step](Range(start_y,end_pointY), Range(start_x,end_pointX)).clone();//영역 잘라내기
        step++;
        frame_load(image[step]);
    }
    else
        frame_update(image[step]);
    }
    if(pers_triger==1 && nPoint == 4 ){
        pers_triger=0;
        nPoint = 0;
        for(int i =0; i<4;i++){
            p[i] = Point2f(0.0,0.0);
        }
    }
}


//회전함수 시계방향
void photo_frame::on_clockwise_btn_clicked()
{
    rotate(image[step], image[step+1], ROTATE_90_CLOCKWISE);
    step++;
    frame_load(image[step]);
}

//회전함수 반시계방향
void photo_frame::on_c_clockwise_btn_clicked()
{
    rotate(image[step], image[step+1], ROTATE_90_COUNTERCLOCKWISE);
    step++;
    frame_load(image[step]);

}


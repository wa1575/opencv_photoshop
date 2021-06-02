#include "photo_frame.h"
#include "ui_photo_frame.h"

//�� ���� ����
float scope = 1; //�� ����
//�̹����� �ȼ� �ʺ�, ���� ����ϱ� ���� ����
int pix_width = 0; //�̹��� ���� �ʱ�ȭ
int pix_height = 0;//�̹��� ���� �ʱ�ȭ
//���콺 �̺�Ʈ �������� ���� ����
int start_x;
int start_y;

photo_frame::photo_frame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::photo_frame)
{    
    ui->setupUi(this);//ui�˾�
    ui->label->setMouseTracking(true);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

}
//�����ӿ� �̹��� �ҷ����� QT����
void photo_frame::frame_load(Mat frame){
    if(!frame.empty()){
        QString size;//������ Ȯ��
        Pixmap=QPixmap::fromImage(Mat2QImage(frame));
        //QT���� �̹����� �ʺ�� ���� �ޱ�
        pix_width = Pixmap.width();
        pix_height= Pixmap.height();
        size = QString::number(pix_width) + "x" + QString::number(pix_height);
        this->setWindowTitle(size);//Ÿ��Ʋ �̸��� ������� ǥ��
        int wind_height = 900;//�ʱⰪ 900 ����
        int wind_width = pix_width * wind_height / pix_height;
        //�����쿡 ȭ�� ũ�� �� �����ġ
        this->setGeometry(100,100,wind_width,wind_height);
        on_fit_btn_clicked();
    }
}
//�����ӿ� �̹��� ������Ʈ ���ֱ�
void photo_frame::frame_update(Mat frame){
    if(!frame.empty()){
        Pixmap=QPixmap::fromImage(Mat2QImage(frame));
        QString size;
        pix_width =Pixmap.width();//�ٽ� �̹��� �ʺ� �޾ƿ�
        pix_height=Pixmap.height();//�ٽ� �̹��� ���̸� �޾ƿ�
        size = QString::number(pix_width) + "x" + QString::number(pix_height);
        this->setWindowTitle(size);//������ ���� �� �̸� ����
        int pix_height_t = float(pix_height) * scope;//�� �� ���� ��ŭ �̹��� ������Ʈ
        QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
        ui->label->setPixmap(ZPixmap);//UI�� �ݿ�
    }
}

QImage photo_frame::Mat2QImage(const Mat src){
    if(src.type()==CV_8UC1){//qt���� �̹��� �ҷ��ͼ� ���ۿ� �ֱ�
        QVector<QRgb> colorTable;
        for(int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        const uchar *qImageBuffer = (const uchar*)src.data;
        QImage img(qImageBuffer,src.cols,src.rows,src.step,QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img; //��ο��� �̹��� ������ �ҷ��� �̹����� ��ȯ
    }
    if(src.type()==CV_8UC3){//�÷���� �÷�(rgb)�� �̹��� �ҷ���
        const uchar *qImageBuffer = (const uchar*)src.data;
        QImage img(qImageBuffer,src.cols,src.rows,src.step,QImage::Format_RGB888);//RGB�� �̹��� �ҷ�����
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


//���� ��ư Ŭ�� ��
void photo_frame::on_zoomin_btn_clicked()
{   //�ִ�� Ȯ���� �� �ִ� �κ��� ���س���
    if(scope < 2){
        scope = scope + 0.05;}
    int pix_height_t = int( pix_height * scope );
    QPixmap ZPixmap = Pixmap.scaledToHeight( pix_height_t );
    ui->label->setPixmap( ZPixmap );
}
//�� �ƿ� ��ư Ŭ�� ��
void photo_frame::on_zoomout_btn_clicked()
{   //�ּҷ� ����� �� �ִ� �κ��� ���س���
    if(scope > 0.1){
        scope = scope - 0.05;}
    int pix_height_t = int(pix_height*scope);

    QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
    ui->label->setPixmap(ZPixmap);

}
//�̹����� ȭ�鿡 �� ���߱�
void photo_frame::on_fit_btn_clicked()
{   //���ڴ� ȭ�� ���鼭 ����
    float pix_height_f = float(ui->scrollArea->height())-40;//�뷫���� ȭ�� UI ����
    scope = pix_height_f/float(pix_height);//ȭ�� ũ�⿡ ���߰� �����
    int pix_height_t = pix_height * scope; //ȭ�� ����
    QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
    if(ui->scrollArea->width() < ZPixmap.width()){
        if(!this->isMaximized()){//�ִ밡 �ƴ϶�� �� ���� �� �ְ� Ű�� ��
       this->resize(ZPixmap.width()+20, this->height());}
    }
    ui->label->setPixmap(ZPixmap);//������Ʈ
}
//�̹����� ���� ũ��� ��������
void photo_frame::on_ori_btn_clicked()
{
    scope = 1;//�� ���� 1�� ����
    int pix_height_t = int(pix_height*scope);//������ �ٽ� ���߱�
    QPixmap ZPixmap = Pixmap.scaledToHeight(pix_height_t);
    ui->label->setPixmap(ZPixmap);
}


void photo_frame::mouseMoveEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    //�ּ��� int���̹Ƿ� 1�� �������� ����
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
//���� ��Ʈ���� ���ϱ�
Mat photo_frame::getPerspectiveMatrix() {
    Mat m;
    if (nPoint == 4) {
        int L1 = sqrt(pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y, 2));//�Ÿ� ���1
        int L2 = sqrt(pow(p[2].x - p[1].x, 2) + pow(p[2].y - p[1].y, 2));//�Ÿ� ���2
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
        rectangle(disply_image, rect, Scalar(0, 0, 255), 8); //ũ��
    }
    frame_update(disply_image);
}


void photo_frame::mousePressEvent(QMouseEvent *event){
        //qt���� x, y ��ǥ ��������
        int x = event -> x();
        int y = event -> y();
        int P_x=(ui->scrollArea->width()-pix_width * scope)/2 + 1; //scope ������ ����
        int P_y=(ui->scrollArea->height()-pix_height * scope)/2 + 1;

        start_x=(x-P_x) / scope;//������ ���� x��ǥ ����
        start_y=(y-P_y-ui->scrollArea->y()) / scope;//������ ���� y��ǥ ����
        if(start_x > Pixmap.width())//x ��ǥ�� ����
            start_x = Pixmap.width();
        else if(start_x < 0)
            start_x=0;
        if(start_y > Pixmap.height())//y ��ǥ�� ����
            start_y = Pixmap.height();
        else if(start_y < 0)
            start_y = 0;
        if(pers_triger == 1){
            p[nPoint].x = start_x, p[nPoint].y = start_y;
            nPoint = (nPoint + 1) > 4 ? 4 : (nPoint + 1);//�簢������ �����
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
    //qt�� Ȯ��â ����
    QMessageBox message(QMessageBox::NoIcon, "Cut", "Do you want to cutting the Area?", QMessageBox::Yes | QMessageBox::No, NULL);
    if(message.exec() == QMessageBox::Yes)
    {
        image[step+1] = image[step](Range(start_y,end_pointY), Range(start_x,end_pointX)).clone();//���� �߶󳻱�
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


//ȸ���Լ� �ð����
void photo_frame::on_clockwise_btn_clicked()
{
    rotate(image[step], image[step+1], ROTATE_90_CLOCKWISE);
    step++;
    frame_load(image[step]);
}

//ȸ���Լ� �ݽð����
void photo_frame::on_c_clockwise_btn_clicked()
{
    rotate(image[step], image[step+1], ROTATE_90_COUNTERCLOCKWISE);
    step++;
    frame_load(image[step]);

}


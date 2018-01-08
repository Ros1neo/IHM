#include <QKeyEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

bool KEY_CTRL_PRESSED = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->render->addWidget(&_gl);
    _gl.setFocus();

    if(TP_ID == 1)
    {
        ui->combo_scenes->addItem("Scene 1");
        ui->combo_scenes->addItem("Scene 2");
        ui->combo_scenes->addItem("Scene 3");
        ui->combo_scenes->addItem("Scene 4");
        ui->combo_scenes->addItem("Scene 5");
    }
    else if(TP_ID == 2)
    {
        ui->combo_scenes->addItem("Scene 1");
        ui->combo_scenes->addItem("Scene 2");
        ui->combo_scenes->addItem("Scene 3");
    }
    else if(TP_ID == 3)
        ui->combo_scenes->addItem("Scene 1");
    else
    {
        std::cerr << "ERROR : TP_ID unknown" << std::endl;
        exit(0);
    }


    // mouse move event work without click
    setMouseTracking(true);
    centralWidget()->setMouseTracking(true);
    _gl.setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_combo_scenes_currentIndexChanged(const QString &arg1)
{
    _gl.setScene(QString(arg1).remove("Scene ").toInt());
    _gl.setFocus();
}

void MainWindow::on_btn_reset_scene_clicked()
{
    _gl.setScene(ui->combo_scenes->currentText().remove("Scene ").toInt());
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(DISPLAY_KEYBOARD_EVENTS) cout << "key press event : " << event->key() << endl;

    switch (event->key()) {
    case Qt::Key_Escape:
        exit(EXIT_SUCCESS);
        break;
    case Qt::Key_Control:
        KEY_CTRL_PRESSED = true;
        break;

    case (Qt::Key_Up):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(0.0,1.0,0.0)));
        break;
    case (Qt::Key_Down):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(0.0,-1.0,0.0)));
        break;
    case (Qt::Key_Left):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(-1.0,0.0,0.0)));
        break;
    case (Qt::Key_Right):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(1.0,0.0,0.0)));
        break;
    case (Qt::Key_Z):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(0.0,1.0,0.0)));
        break;
    case (Qt::Key_S):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(0.0,-1.0,0.0)));
        break;
    case (Qt::Key_Q):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(-1.0,0.0,0.0)));
        break;
    case (Qt::Key_D):
        _gl.scene()->object(_gl._selected_id)->setTranslation(Translation3f(_gl.scene()->camera()->viewMatrix().linear().inverse()*Vector3f(1.0,0.0,0.0)));
        break;
    }
    _gl.update();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(DISPLAY_KEYBOARD_EVENTS) cout << "key release event : " << event->key() << endl;

    switch (event->key()) {
    case Qt::Key_Control:
        KEY_CTRL_PRESSED = false;
        break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(DISPLAY_MOUSE_EVENTS) cout << "mouse press event : " << event->button() << endl;

    // relative pos in the render frame
    int x = event->pos().x() - _gl.pos().x();
    int y = event->pos().y() - _gl.pos().y();

    // color picking
    if(x >= 0 && x < _gl.camera()->vpWidth() && TP_ID >= 2)
        if(y >= 0 && y < _gl.camera()->vpHeight())
            _gl.colorPicking(Vector2i(x, y));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //if(DISPLAY_MOUSE_EVENTS) cout << "mouse move event : " << event->pos().x()-(float)_gl.scene()->camera()->vpWidth()/2 << " " << event->pos().y()-(float)_gl.scene()->camera()->vpWidth()/2 << endl;
    Vector2f mouse_pos = Vector2f(event->pos().x()-(float)_gl.scene()->camera()->vpWidth()/2, event->pos().y()-(float)_gl.scene()->camera()->vpHeight()/2);
    Rotation2D<float> rot2((float)3.141592/2);
    cout << "mouse move event : " << mouse_pos.x() << " " << (rot2*mouse_pos).x() << endl;
    cout << "mouse move event : " << mouse_pos.y() << " " << (rot2*mouse_pos).y() << endl;

    _gl.scene()->object(_gl._selected_id)->setRotation(Affine3f(AngleAxis<float>(0.01,Vector3f((mouse_pos).x(),(mouse_pos).y(),0.0).normalized())));
    _gl.update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(DISPLAY_MOUSE_EVENTS) cout << "mouse release event : " << event->button() << endl;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(DISPLAY_MOUSE_EVENTS) cout << "mouse wheel event : " << event->delta() << endl;
}

#include "mapbutton.h"

mapButton::mapButton(int No, int x, int y)
{
    if(No != 0)
        connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonChange()));

    set(No, x, y);
}

void mapButton::set(int No, int x, int y) {
    //按钮编号,0为空
    this->No = No;
    this->x = x;
    this->y = y;

    if(No != 0) {
        //样式设置
        setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        setCheckable(1);//按钮两种状态
        //图片路径
        QString res = ":/items/item/";
        res.append(QString::number(No)+ ".png");

        //设置按钮样式
        setStyleSheet("QPushButton{border-image: url(" + res + ")}" //加载按钮图片
                      "QPushButton:hover{background:yellow}");    //鼠标划过：背景变黄
        setEnabled(1);

    } else {
        setTransparent();
        /*setFlat(1);*/ //测试，实际改成setVisible(0)
        //setVisible(0);
    }


}
void mapButton::buttonChange(){ //发送坐标
    emit aButtonClicked(x,y);
}


//状态函数
void mapButton::setClicked(bool state) {
    //emit aButtonClicked();

    if(state) {
        QString res = ":/items/item/";
        res.append(QString::number(No)+ ".png");
        setStyleSheet("QPushButton{background:yellow}"
                      "QPushButton{border-image: url(" + res + ")}");
    } else {

        QString res = ":/items/item/";
        res.append(QString::number(No)+ ".png");
        setStyleSheet("QPushButton{border-image: url(" + res + ")}" //加载按钮图片
                      "QPushButton:hover{background:yellow}");    //鼠标划过：背景变黄

    }

}

void mapButton::setTransparent(){
    setStyleSheet("background:transparent");
    setEnabled(0);

}

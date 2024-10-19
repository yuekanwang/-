#include "Gamescene.h"
#include "ui_gamescene.h"
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <algorithm>
#include <math.h>
#include<QMessageBox>
Gamescene::Gamescene(QWidget *parent)//构造函数（初始化游戏）
    : QWidget(parent)
    , ui(new Ui::Gamescene)
{
    ui->setupUi(this);
    this->setWindowTitle("中国象棋对战");//窗口名字设置为“中国象棋对战”
    this->setFixedSize(995,678);//固定好窗口的分辨率大小

    for(int i=0;i<32;i++)
    {
        stone[i].initialize(i);//(initialize)初始化棋子
    }

    selectid=-1;//初始化（未选择）
    redtrue=true;//初始化（红方先行）

    connect(ui->pushButton_3,&QPushButton::clicked,[=](){
        Regretstone();
    });//悔棋按钮连接

    //游戏对战页面背景音乐
    scenemusic=new QMediaPlayer(this);//播放器
    control=new QAudioOutput(this);//控制器
    scenemusic->setAudioOutput(control);//播放器输入由控制器控制
    control->setVolume(0.3);//音量
    scenemusic->setSource(QUrl("qrc:/Music/SceneMusic2.mp3"));
    scenemusic->setLoops(-1);

    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        if(!scenemusic->isPlaying())
        {
            scenemusic->play();
            ui->pushButton_2->setText("音乐打开");
        }
        else
        {
            scenemusic->stop();
            ui->pushButton_2->setText("音乐关闭");
        }
    });
    scenemusic->play();
}

Gamescene::~Gamescene()
{

    delete ui;
}




void Gamescene::Regretpacket()
{
    std::vector<Stone> Stoneboard_now;//记录当前棋盘
    for(int i=0;i<32;i++)
    {
        Stoneboard_now.push_back(stone[i]);//把每个棋子的情况弄进去
    }
    Regret.push(Stoneboard_now);//把当前棋盘弄进悔棋栈
}


void Gamescene::Regretstone()
{
    std::vector<Stone> Stoneboard_last;//装载上一步棋盘
    if(!Regret.empty())
    {
        Stoneboard_last=Regret.top();
        Regret.pop();

        for(int i=0;i<32;i++)
        {
            stone[i]=Stoneboard_last[i];
        }
        redtrue=!redtrue;
        update();
    }
    else
    {
        QMessageBox test;
        test.setWindowTitle("悔棋失败！");
        test.setText("没有能悔的棋子了。");
        test.exec();
    }

    return;
}



int Gamescene::getStoneId(int row,int col)
{
    for(int i=0;i<32;i++)
    {
        if(row==stone[i].row&&col==stone[i].col&&stone[i].death==false)
            return i;
    }
    return -1;
}

bool Gamescene::isdead(int id)
{
    if(id==-1)return true;
    else
        return stone[id].death;
}



void Gamescene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QPixmap pixmap(":/mainscene/GameSceneprint2.png");
    painter.drawPixmap(this->rect(),pixmap);//贴上深沉的背景

    QPen pen=QPen(QBrush(Qt::SolidPattern),4);
    painter.setPen(pen);//画笔变粗

    offset=60;//距离界面的边距
    d=(45/30)*offset;//间距为50px
    r=d/2;//棋子半径为d/2

    //***绘画棋盘***//
    //绘画10条横线
    int gz=100;//棋盘向右移动100
    for(int i = 0; i < 10; i++)
    {
        painter.drawLine(QPoint(offset+gz, offset+i*d), QPoint(offset+8*d+gz, offset+i*d));
    }

    //***绘画竖线***

    //左右两条边线
    painter.drawLine(QPoint(offset+gz,offset),QPoint(offset+gz,offset+9*d));
    painter.drawLine(QPoint(offset+gz+8*d,offset),QPoint(offset+gz+8*d,offset+9*d));

    //中间的线
    for(int i=0;i<8;i++)
    {
        painter.drawLine(QPoint(offset+gz+i*d, offset), QPoint(offset+i*d+gz, offset+4*d));
    }//上半图

    for(int i=0;i<8;i++)
    {
        painter.drawLine(QPoint(offset+gz+i*d, offset+5*d), QPoint(offset+i*d+gz, offset+9*d));
    }//下半图

    //***将军家的斜线***
    painter.drawLine(QPoint(offset+gz+3*d, offset), QPoint(offset+5*d+gz, offset+2*d));
    painter.drawLine(QPoint(offset+gz+5*d, offset), QPoint(offset+3*d+gz, offset+2*d));

    painter.drawLine(QPoint(offset+gz+3*d, offset+9*d), QPoint(offset+5*d+gz, offset+7*d));
    painter.drawLine(QPoint(offset+gz+5*d, offset+9*d), QPoint(offset+3*d+gz, offset+7*d));

    //**"楚河汉界"**先设方形，然后在方形里面写字
    QRect rect1(offset+d+gz,offset+4*d+10,d,d);
    QRect rect2(offset+2*d+gz,offset+4*d+10,d,d);

    QRect rect3(-(offset+6*d+gz),-(offset+5*d)+10,d,d);
    QRect rect4(-(offset+7*d+gz),-(offset+5*d)+10,d,d);

    painter.setFont(QFont("隶书",r,800));
    painter.drawText(rect1,"楚",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect2,"河",QTextOption(Qt::AlignHCenter));
    painter.save();
    painter.translate(0,0);
    painter.rotate(180);
    painter.drawText(rect3,"汉",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect4,"界",QTextOption(Qt::AlignHCenter));
    painter.restore();

    for(int i=0;i<32;i++)
    {
        drawStone(painter,i);
    }

    //画提示回合的框。这个框本质是按钮，只是点了没用
    if(redtrue)
    {
        ui->pushButton->setText("红方回合");
        ui->pushButton->setStyleSheet("color:red;font: 30pt 华文行楷;background-color:#ffbb46;border-radius:40px;border-style: solid; border-width: 2px; border-radius: 10px; border-color: red;");
        ui->pushButton_3->setStyleSheet("color:black;font: 30pt 华文行楷;background-color:#ffbb46;border-radius:40px;border-style: solid; border-width: 2px; border-radius: 10px; border-color: black;");
    }
    else
    {
        ui->pushButton->setText("黑方回合");
        ui->pushButton->setStyleSheet("color:black;font: 30pt 华文行楷;background-color:#ffbb46;border-radius:40px;border-style: solid; border-width: 2px; border-radius: 10px; border-color: black;");
        ui->pushButton_3->setStyleSheet("color:red;font: 30pt 华文行楷;background-color:#ffbb46;border-radius:40px;border-style: solid; border-width: 2px; border-radius: 10px; border-color: red;");
    }
}



QPoint Gamescene::center(int row, int col)//象棋的棋盘的坐标转换成界面坐标
{
    QPoint p(col*d+offset+gz,row*d+offset);
    return p;
}



QPoint Gamescene::center(int id)
{
    QPoint p(stone[id].col*d+offset+gz,stone[id].row*d+offset);
    return p;
}



void Gamescene::drawStone(QPainter&painter,int id)
{
    if(stone[id].death)//判断棋子有无死
        return ;

    painter.setPen(QPen(Qt::black,4,Qt::SolidLine));

    if(selectid==id)//判断是否有选择棋子
    {
        painter.setBrush(QBrush(QColor(160, 121, 9)));
        //rgb(1, 202, 209)//蓝色
        //rgb(160, 121, 9)//黑黄色
    }
    else
    {
        painter.setBrush(QBrush(QColor(255, 183, 58)));
    }
    painter.drawEllipse(center(id),r,r);//画棋子的圆形

    //画棋子颜色
    if(id < 16)
        painter.setPen(QPen(Qt::red,4,Qt::SolidLine));
    else
        painter.setPen(QPen(Qt::black,4,Qt::SolidLine));

    QRect rect(center(id).x()-r,center(id).y()-r,d,d);

    //设一个方，将其置于棋子的圆内，然后写字
    painter.setFont(QFont("楷体",r*1.3,2000));
    painter.drawText(rect,stone[id].getText(stone[id].red), QTextOption(Qt::AlignCenter));
    //绘画圆形里面的汉字

}



bool Gamescene::getRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0; row <= 9; row++)
    {
        for(col = 0; col <= 8; col++)
        {
            QPoint temp = center(row, col);
            int x = temp.x()-pt.x();
            int y = temp.y()-pt.y();

            if(x*x+y*y < r*r)
                return true;
        }
    }
    return false;
}


//鼠标点击事件
void Gamescene::mousePressEvent(QMouseEvent *ev)
{
    QPoint pt = ev->pos();
    //将pt转化成棋盘的像行列值
    //判断这个行列值上面有没有棋子
    int row, col;

    //点击棋盘外面就不做处理
    if(!getRowCol(pt, row, col))
        return;

    clicked = -1;
    int i;

    //判断是哪一个棋子被选中，根据ID（这里的局部i）来记录下来
    for(i = 0; i <= 31; i++)
    {
        if(stone[i].row == row && stone[i].col == col && stone[i].death == false)
            break;
    }

    if(i < 32)
    {
        clicked = i;//选中的棋子的ID
        //qDebug()<<stone[i].row<<' '<<stone[i].col;
    }

    if(selectid == -1)//选中棋子
    {
        if(clicked != -1)
        {
            if(redtrue == stone[clicked].red)//如果所点击的棋子是自己方，则选中
                selectid = clicked;
        }
    }
    else if(selectid!=-1&&clicked!=-1&&stone[selectid].red==stone[clicked].red)//想换个棋子选
    {
        selectid = clicked;
    }

    else//走棋子
    {
        if(canMove(selectid,clicked, row, col ))
        {
             Regretpacket();//记录当前棋盘

            int row_=stone[selectid].row;//记录原本的位置，若是触发“送将”则回溯
            int col_=stone[selectid].col;
            // 移动棋子的逻辑
            stone[selectid].row = row;
            stone[selectid].col = col;

            //当isDefeated输入的参数为0时，为“送将”函数。输入的参数为1时，为“被将”函数。

            // if(isDefeated(0))//一方移动完成后，判断是否送将，是则回溯
            // {
            //     stone[selectid].row = row_;
            //     stone[selectid].col = col_;
            //     if(clicked != -1)
            //         stone[clicked].death = false;
            //     //winMessageBox("提示", "本局结束，黑方胜利.");//test winmessageBox函数
            //     update();
            //     return ;

            // }
            if(clicked != -1)
                stone[clicked].death = true;
            selectid = -1;// 取消选择
            //落子的声音
            stonemovemusic =new QSoundEffect(this);
            stonemovemusic->setSource(QUrl::fromLocalFile(":/Music/StoneKill.wav"));
            stonemovemusic->setLoopCount(1);
            stonemovemusic->play();
            if(isDefeated(1)&&!stone[4].death&&!stone[20].death)
            {
                //这里弄个声音提示将军了
                attackmusic =new QSoundEffect(this);
                attackmusic->setSource(QUrl::fromLocalFile(":/Music/attack.wav"));
                attackmusic->setLoopCount(1);
                attackmusic->play();
            }
            whoWin();
            if(redtrue)//红方回合，要悔棋就是黑方悔，所以搞成黑色
                ui->pushButton_3->setStyleSheet("color:black;font: 30pt 华文行楷;background-color:#ffbb46;border-radius:40px;border-style: solid; border-width: 2px; border-radius: 10px; border-color: black;");

            else
                ui->pushButton_3->setStyleSheet("color:red;font: 30pt 华文行楷;background-color:#ffbb46;border-radius:40px;border-style: solid; border-width: 2px; border-radius: 10px; border-color: red;");

            //判断完再换边
            redtrue = !redtrue;// 轮到对方

        }

    }
    update();// 刷新界面

}



//总的移动规则，选中准备下的棋子，被杀的棋子， 准备移动到的目的行列值
bool Gamescene::canMove(int moveId, int killId, int row, int col)//棋子走法
{
    //1.确定是选择其它棋子还是走棋
    //2.是否需要使用到canMoveXXX()来做限制
    //3.罗列出所有情况，和需要的得到的结果值 ==>  然后进行中间的逻辑层判断

    //tm这段注释别删，后面可能要用到
    if(stone[moveId].red==stone[killId].red)
    {
        if(killId==-1)
        {
            switch(stone[moveId].ty)//根据选中的棋子，来选择对应的走法
            {
            case Stone::JIANG:
                return canMoveJIANG(moveId, killId, row, col);
            case Stone::SHI:
                return canMoveSHI(moveId, killId, row, col);
            case Stone::XIANG:
                return canMoveXIANG(moveId, killId, row, col);
            case Stone::MA:
                return canMoveMA(moveId, killId, row, col);
            case Stone::CHE:
                return canMoveCHE(moveId, killId, row, col);
            case Stone::PAO:
                return canMovePAO(moveId, killId, row, col);
            case Stone::BING:
                return canMoveBING(moveId, killId, row, col);

            }
        }
        selectid=killId;
        update();
        return false;
    }

    else
    {
        switch(stone[moveId].ty)//根据选中的棋子，来选择对应的走法
        {
        case Stone::JIANG:
            return canMoveJIANG(moveId, killId, row, col);
        case Stone::SHI:
            return canMoveSHI(moveId, killId, row, col);
        case Stone::XIANG:
            return canMoveXIANG(moveId, killId, row, col);
         case Stone::MA:
             return canMoveMA(moveId, killId, row, col);
            case Stone::CHE:
                return canMoveCHE(moveId, killId, row, col);
            case Stone::PAO:
                return canMovePAO(moveId, killId, row, col);
            case Stone::BING:
                return canMoveBING(moveId, killId, row, col);

        }
        return true;
    }
}



bool Gamescene::canMoveJIANG(int moveId, int killId, int row, int col)
{

    // 检查将军是否面对面
    if (face())
    {
        // 如果两个将军之间没有棋子，那么将军的走法与车一致
        if (canMoveCHE(moveId, killId, row, col))
            return true;
    }

    //将军不能超出营地
    if(stone[moveId].red)//红方
    {

        if(row > 2 || col < 3 )
            return false;
        if(col>5)
            return false;
    }
    else//黑方
    {
        if(row<7||col<3)
            return false;
        if(col>5)
            return false;
    }

    //计算将军行与列分别走的步数，并加起来只能等于一
    int dr=abs(stone[moveId].row-row);
    int dc=abs(stone[moveId].col-col);

    int d=dr*10+dc;
    //为何要dr*10呢？假设填了(dr==1||dc==1),那么当dr=2，dc=1的时候，也能走
    //所以要让他们相加为d，行走了一步，算10；列走了一步，算1.
    if(d==1||d==10)
        return true;

    return false;

}



bool Gamescene::canMoveSHI(int moveId, int killId, int row, int col)
{
    //对于士行走范围的限制
    if(stone[moveId].red)//红方
    {

        if(row > 2 || col < 3 )
            return false;
        if(col>5)
            return false;
    }
    else//黑方
    {
        if(row<7||col<3)
            return false;
        if(col>5)
            return false;
    }
    int dr=abs(stone[moveId].row-row);//行走的步数

    int dc=abs(stone[moveId].col-col);//列走的步数
    int d=dr*10+dc;
    if(d==11)//用将的走法原理类似
        return true;

    return false;
}



 bool Gamescene::canMoveXIANG(int moveId, int killId, int row, int col)
{
    if(stone[moveId].red)
    {
        if(row>4)
            return false;
    }
    else
    {
        if(row<5)
            return false;
    }

    int dr=abs(stone[moveId].row-row);//行走的步数
    int dc=abs(stone[moveId].col-col);//列走的步数

    if(dr==2&&dc==2)
    {
        //计算象脚的位置
        int kr=(stone[moveId].row+row)/2;
        int kc=(stone[moveId].col+col)/2;

        if(getStoneId(kr,kc)!=-1)//如果象脚有棋子，那么不允许移动
            return false;

        return true;
    }

    return false;
}



 bool Gamescene::canMoveMA(int moveId, int killId, int row, int col)
{
    int dr=abs(stone[moveId].row-row);//行走的步数
    int dc=abs(stone[moveId].col-col);//列走的步数

    if((dr==1&&dc==2)||(dr==2&&dc==1))
    {
        //计算马脚的位置
        int kr,kc;
        if(dr==1&&dc==2)//马横着跳
        {
            kr=stone[moveId].row;
            kc=(stone[moveId].col+col)/2;
        }
        else//马竖着跳
        {
            kr=(stone[moveId].row+row)/2;
            kc=stone[moveId].col;
        }

        if(getStoneId(kr,kc)!=-1)//如果马脚有棋子，那么不允许移动
            return false;

        return true;
    }
    return false;
}



bool Gamescene::canMoveBING(int moveId, int killId, int row, int col)
{
    int r = stone[moveId].row;
    int c = stone[moveId].col;

    // 兵在红方
    if(stone[moveId].red)
    {
        // 兵未过河
        if(r <= 4)
        {
            // 只能向前走一步
            if(row == r + 1 && col == c)
                return true;
        }
        else // 兵已过河
        {
            // 可以向前、左、右走一步
            if((row == r + 1 && col == c) || (row == r && col == c + 1) || (row == r && col == c - 1))
                return true;
        }
    }
    else // 兵在黑方
    {
        // 兵未过河
        if(r >= 5)
        {
            // 只能向前走一步
            if(row == r - 1 && col == c)
                return true;
        }
        else // 兵已过河
        {
            // 可以向前、左、右走一步
            if((row == r - 1 && col == c) || (row == r && col == c + 1) || (row == r && col == c - 1))
                return true;
        }
    }

    return false;
}



bool Gamescene::canMoveCHE(int moveId, int killId, int row, int col)
{
    int r = stone[moveId].row;
    int c = stone[moveId].col;

    // 车只能在同一行或同一列移动
    if (r != row && c != col)
        return false;

    // 检查路径上是否有其他棋子
    int count = getStoneCountAtLine(r, c, row, col);
    //qDebug()<<"车的"<<' '<<count;
    if (count != 0)
        return false;

    return true;
}

int Gamescene::getStoneCountAtLine(int row1,int col1,int row2,int col2)
{
    int ret=0;//记录直线上的棋子数量(不包括终点和起点）

    //若是两个点一致或是两个点不在同一直线上，则返回-1结束
    if((row1==row2&&col1==col2)||(row1!=row2&&col1!=col2))
        return -1;

    if(row1==row2)
    {
        for(int col=std::min(col1,col2)+1;col<std::max(col1,col2);col++)
            if(getStoneId(row1,col)!=-1)
            {
                ret++;
               //qDebug()<<"中间的棋子"<<row1<<' '<<col<<' '<<stone[getStoneId(row1,col)].ty;
            }
    }
    else if(col1==col2)
    {
        for(int row=std::min(row1,row2)+1;row<std::max(row1,row2);row++)
            if(getStoneId(row,col1)!=-1)
            {   ret++;
                //qDebug()<<"中间的棋子"<<row<<' '<<col1<<' '<<stone[getStoneId(row,col1)].ty;
            }
    }
    return ret;
}

bool Gamescene::canMovePAO(int moveId, int killId, int row, int col)
{
    int r = stone[moveId].row;
    int c = stone[moveId].col;

    // 炮只能在同一行或同一列移动
    if (r != row && c != col)
        return false;

    // 获取路径上棋子的数量
    int count = getStoneCountAtLine(r, c, row, col);

    if (killId == -1)
    {
        // 如果没有要吃的棋子，路径上不能有其他棋子
        if (count != 0)
            return false;
    }
    else
    {
        // 如果有要吃的棋子，路径上必须有且只有一个棋子
        if (count != 1)
            return false;
    }

    return true;
}

void Gamescene::reset() // 分出胜负之后重置界面
{
    // 重置所有棋子的状态
    for(int i=0;i<32;i++)
    {
        stone[i].initialize(i);//(initialize)初始化棋子
    }

    selectid = -1;
    clicked = -1;
    //redtrue = true;

    // 刷新界面
    update();
}
bool Gamescene::face() // 将军面对面
{
    int colBlack = stone[4].col;
    int colRed = stone[20].col;
    int rowBlack = stone[4].row;
    int rowRed = stone[20].row;

    bool colEmpty = true;
    if (colBlack == colRed) {
        for (int row = rowBlack + 1; row < rowRed; ++row) {
            if (getStoneId(row, colBlack) != -1) {
                colEmpty = false;  // 将之间有棋子；非此列为空
                break;
            }
        }
    } else {
        colEmpty = false;
    }

    return colEmpty;
}


bool Gamescene::isDefeated(bool f)
{
    //f==0时，默认是黑方回合，判断是否有红棋可以直接攻击到将军,以改变关键变量来控制
    int generalId = 20,i=16,j=0; // 黑将
    if (redtrue)//站在红方的角度
        generalId = 4;

    //f==1时，用来判断下完一步棋后，是否有棋子可以将军到对面,以改变关键变量来控制
    if(f==true)
    {
        i=0,j=16;
        generalId = 4;
        if(redtrue)
            generalId = 20;
    }
    int i_=i+16,j_=j+16;

    int row = stone[generalId].row; // 当前回合方的将军row
    int col = stone[generalId].col; // 当前回合方的将军col

    if(redtrue)
    {
        for(;i<i_;i++)
        {
            if(canMove(i,generalId,row,col)&&!stone[i].death)
            {
                //qDebug()<<stone[i].row<<' '<<stone[i].col;
                return true;
            }
        }
    }
    else
    {
        for(;j<j_;j++)
        {
            if(canMove(j,generalId,row,col)&&!stone[j].death)
            {
                //qDebug()<<stone[j].row<<' '<<stone[j].col;
                return true;
            }
        }
    }

    return false;
}



void Gamescene::whoWin()
{
    if(!stone[4].death&&stone[20].death)
    {
        Lorekmusic =new QSoundEffect(this);
        Lorekmusic->setSource(QUrl::fromLocalFile(":/Music/Lore.wav"));
        Lorekmusic->setLoopCount(1);
        Lorekmusic->play();
        redtrue=true;//调整开局先手为红方
        reset();
        winMessageBox("提示", "本局结束，红方胜利.");
    }

    if(stone[4].death&&!stone[20].death)
    {
        Lorekmusic =new QSoundEffect(this);
        Lorekmusic->setSource(QUrl::fromLocalFile(":/Music/Lore.wav"));
        Lorekmusic->setLoopCount(1);
        Lorekmusic->play();
         redtrue=true;//调整开局先手为红方
        reset();
        winMessageBox("提示", "本局结束，黑方胜利.");
    }
}
void Gamescene::winMessageBox(QString title, QString msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}


void Gamescene::withDraw()//悔棋
{
    QMessageBox test;
    test.exec();
}

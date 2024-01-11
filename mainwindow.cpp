#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QVector>
#include <QString>
#include <stack>
#include <string>
#include <algorithm>

QString ChangeType(double);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f1 = new About;
    f2 = new Instructions;
    ui->calculateResult->setFontPointSize(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

class Uncertainty
{
private:
    double Upow(double data, double u, double n)
    {
        return qPow(data, n) * qSqrt(qAbs(n) * qPow(u/data, 2));
    }
public:
    double data = 0;
    double uncertainty = 0;

    Uncertainty(double a, double b)
    {
        data = a;
        uncertainty = b;
    }
    Uncertainty(double a)
    {
        data = a;
    }
    Uncertainty(){}
    bool operator ==(const Uncertainty& un2)
    {
        return (this->data==un2.data) && (this->uncertainty==un2.uncertainty);
    }
    Uncertainty operator +(const Uncertainty& un2)
    {
        Uncertainty r;
        r.data = this->data + un2.data;
        r.uncertainty = qSqrt(qPow(this->uncertainty, 2) + qPow(un2.uncertainty, 2));
        return r;
    }
    Uncertainty operator -(const Uncertainty& un2)
    {
        Uncertainty r;
        r.data = this->data - un2.data;
        r.uncertainty = qSqrt(qPow(this->uncertainty, 2) + qPow(un2.uncertainty, 2));
        return r;
    }
    Uncertainty operator *(const Uncertainty& un2)
    {
        Uncertainty r;
        r.data = this->data * un2.data;
        if(un2.uncertainty == 0)
            r.uncertainty = this->uncertainty * un2.data;
        else if(this->uncertainty == 0)
            r.uncertainty = this->data * un2.uncertainty;
        else
            r.uncertainty = qAbs(r.data) * qSqrt(qPow(this->uncertainty / this->data, 2) + qPow(un2.uncertainty / un2.data, 2));
        return r;
    }
    Uncertainty operator /(const Uncertainty& un2)
    {
        Uncertainty r;
        r.data = this->data / un2.data;
        if(un2.uncertainty == 0)
            r.uncertainty = this->uncertainty / un2.data;
        else if(this->uncertainty == 0)
            r.uncertainty = this->data * Upow(un2.data, un2.uncertainty,-1);
        else
            r.uncertainty = qAbs(r.data) * qSqrt(qPow(this->uncertainty / this->data, 2) + qPow(un2.uncertainty / un2.data, 2));
        return r;
    }
};

class Calculator
{
    private:
        std::string Polish;
        Uncertainty X,Y,Z;
        QString result;
    public:
        Calculator(){}
        void Input(Uncertainty x,Uncertainty y, Uncertainty z)
        {
            X = x;
            Y = y;
            Z = z;
        }
        QString& getResult(std::string exp)
        {
            exp.erase(remove_if(exp.begin(), exp.end(), isspace), exp.end()); //跳過空格
            if (exp == "")
            {
                result = "請輸入運算式";
                return result;
            }
            Polish.clear();
            std::stack<char> s1,s2;
            s1.push('#');         //以#號作為標記
            char temp;
            for(int i=0; i<(int)exp.length(); i++)
            {                 //生成逆波蘭式
                if ((exp[i] >= '0' && exp[i] <= '9') || exp[i] == '.' || (exp[i] >= 'X' && exp[i] <= 'Z'))
                {
                    s2.push(exp[i]);
                }
                else if (exp[i] == '+' || exp[i] == '-' ||exp[i] == '*' ||exp[i] == '/')
                {
                    if(i == (int)exp.length()-1){result = "Error";return result;}
                    s2.push('#');
                    temp = s1.top();
                    if (temp == '#' || (temp == '+' || temp == '-') || (exp[i] == '*' || exp[i] == '/'))
                    {
                        s1.push(exp[i]);
                    }
                    else
                    {
                        while (!(temp=='#' || temp == '(' || (temp == '+' || temp == '-') || (exp[i] == '*' || exp[i] == '/')))
                        {
                            s2.push(temp);
                            s1.pop();
                            temp = s1.top();
                        }
                        s1.push(exp[i]);
                    }
                }
                else if(exp[i] == ')'|| exp[i] == '(')
                {
                    if(exp[i] == '(')
                    {
                        s1.push(exp[i]);
                        if(i == (int)exp.length()-1){result = "Error";return result;}
                    }
                    else
                    {
                        temp = s1.top();
                        while (temp != '(' )
                        {
                            s2.push(temp);
                            s1.pop();
                            temp = s1.top();
                        }
                        s1.pop();
                    }
                }
                else
                {
                    if(exp[i] == 'x')
                    {
                        result = "請將字元 x 換成 X";
                    }
                    else if(exp[i] == 'y')
                    {
                        result = "請將字元 y 換成 Y";
                    }
                    else if(exp[i] == 'z')
                    {
                        result = "請將字元 z 換成 Z";
                    }
                    else
                    {
                        result = "Error\n字元 " + (QString)exp[i] + " 無法解讀";
                    }
                    return result;
                }
            }
            while(s1.top()!='#')
            {
                s2.push(s1.top());
                s1.pop();
            }
            for(;s2.size()>=1;)
            {
                Polish.push_back(s2.top());
                s2.pop();
            }
            bool cal = false;
            Uncertainty temp2;
            Uncertainty temp3;
            Uncertainty temp1(0);
            std::stack<Uncertainty> s3;
            int sign = 0;
            int n = 0;
            for(int i = Polish.length()-1; i >= 0; i--)
            {   //進行逆向波蘭式的運算
                if(Polish[i] == '.')
                {
                    sign = 2;
                    n = 0;
                }
                if(Polish[i] >= '0' && Polish[i] <= '9')
                {
                    if(sign == 2)
                    {
                        n++;
                        temp1.data = temp1.data + (float)(Polish[i]-'0')/qPow(10,n);
                    }
                    else
                    {
                        temp1.data = temp1.data*10 +  Polish[i]-'0';
                        sign = 1;
                    }
                }
                if(Polish[i] >= 'X' && Polish[i] <= 'Z')
                {
                    if(Polish[i] == 'X')
                    {
                        temp1 = X;
                    }
                    else if(Polish[i] == 'Y')
                    {
                        temp1 = Y;
                    }
                    else if(Polish[i] == 'Z')
                    {
                        temp1 = Z;
                    }
                    sign = 1;
                }
                if((Polish[i] == '#'||Polish[i] == '+'||Polish[i] == '-'||Polish[i] == '*'||Polish[i] == '/') || (i == 0 && (sign == 1 || sign == 2)))
                {
                    if(sign == 1 || sign == 2)
                    {
                        s3.push(temp1);
                        temp1 = 0;
                        sign = 0;
                    }
                }
                if(Polish[i] == '+'||Polish[i] == '-'||Polish[i] == '*'||Polish[i] == '/')   //計算
                {
                    cal = true;
                    temp2 = s3.top();
                    s3.pop();
                    temp3 = s3.top();
                    s3.pop();
                    switch(Polish[i])
                    {
                        case '+':
                            s3.push(temp3+temp2);break;
                        case '-':
                            s3.push(temp3-temp2);break;
                        case '*':
                            s3.push(temp3*temp2);break;
                        case '/':
                            s3.push(temp3/temp2);break;
                    }
                    result = ChangeType(s3.top().data);
                    result.append(" ± "+ChangeType(s3.top().uncertainty));
                }
            }
            if (cal == false)
            {
                result = "請輸入運算式";
            }
            return result;
        }
};

int digit1 = 2;
int digit2 = 2;
int state1 = 0;
int state2 = 0;
int Index = 0;
int showD = 2;
bool Scientific = false;
double average = 0;
double MStandard_Deviation = 0;
double Standard_Deviation = 0;
double Type_A_uncertainty = 0;
double Type_B_uncertainty = 0;
double Type_C_uncertainty = 0;
QString content;
QString minValue;
QStringList list;

QString ChangeType(double data)
{
    if(Index == 0)
    {
        showD = digit1;
        Scientific = state1;
    }
    else if(Index == 1)
    {
        showD = digit2;
        Scientific = state2;
    }

    QString first, last;
    int n = 0;
    if(Scientific)
    {
        while(qAbs(data) > 10)
        {
            data /= 10;
            n++;
        }
        while(abs(data) < 1 && data != 0)
        {
            data *= 10;
            n--;
        }
        first = QString::number(data, 'd', showD);
        if(n != 0)
        {
            last = QString::number(n, 'd', 0);
            first.append("x10^("+last+")");
        }
        return first;
    }
    else
    {
        first = QString::number(data, 'd', showD);
        return first;
    }
}

double ifpercentage(double d, QString u)
{
    double r = 0;
    if(u.endsWith('%', Qt::CaseInsensitive))
    {
        u.truncate(u.length()-1);
        r = d * u.toDouble() / 100;
    }
    else
    {
        r = u.toDouble();
    }
    return r;
}

void MainWindow::on_EnterButton_clicked()
{
    ui->calculateResult->setPlainText("");

    content = ui->inputTextEdit->toPlainText();
    minValue = ui->minimumValue->text();
    list = content.split(QRegExp("[\r\n]"),Qt::SkipEmptyParts);

    QVector<double> data(list.count());
    bool needRecalculate = false;
    average = 0;
    MStandard_Deviation = 0;
    Standard_Deviation = 0;
    Type_A_uncertainty = 0;
    Type_B_uncertainty = minValue.toDouble() / (2 * qSqrt(3));
    Type_C_uncertainty = 0;
    QString str[10];

    for(int i=0;i<data.size();i++)
    {
        data[i] = list[i].toDouble();
        average += data[i];
    }
    average /= data.size();

    str[0] = ChangeType(average);
    ui->calculateResult->append("算數平均數："+str[0]);
    
    for(int i=0;i<data.size();i++)
    {
        MStandard_Deviation += pow(data.at(i) - average, 2);
    }
    Standard_Deviation = MStandard_Deviation / (data.size() - 1);
    MStandard_Deviation /= data.size();
    Standard_Deviation = qSqrt(Standard_Deviation);
    MStandard_Deviation = qSqrt(MStandard_Deviation);

    str[1] = ChangeType(MStandard_Deviation);
    str[2] = ChangeType(Standard_Deviation);
    ui->calculateResult->append("母體標準偏差："+str[1]+"\n(樣本)標準偏差："+str[2]);

    double min = average - 3 * Standard_Deviation;
    double max = average + 3 * Standard_Deviation;

    str[7] = ChangeType(min);
    str[8] = ChangeType(max);
    ui->calculateResult->append("合理數據範圍："+str[7]+"~"+str[8]);

    for(double d : data)
    {
    	if(d > max || d < min)
    	{
            str[9] = QString::number(d, 'd', showD);
            ui->calculateResult->append("去除數據："+str[9]);
    		data.removeOne(d);
    		needRecalculate = true;
		}
	}
	
	if(needRecalculate)
	{
        ui->calculateResult->append("重新計算中...");

		average = 0;
		MStandard_Deviation = 0;
    	Standard_Deviation = 0;
    	for(int i=0;i<data.size();i++)
	    {
	        average += data[i];
	    }
    	average /= data.size();

        str[0] = ChangeType(average);
        ui->calculateResult->append("算數平均數：" + str[0]);
	    
	    for(int i=0;i<data.size();i++)
	    {
	        MStandard_Deviation += pow(data.at(i) - average, 2);
	    }
	    Standard_Deviation = MStandard_Deviation / (data.size() - 1);
	    MStandard_Deviation /= data.size();
	    Standard_Deviation = qSqrt(Standard_Deviation);
	    MStandard_Deviation = qSqrt(MStandard_Deviation);

        str[1] = ChangeType(MStandard_Deviation);
        str[2] = ChangeType(Standard_Deviation);
        ui->calculateResult->append("母體標準偏差："+str[1]+"\n(樣本)標準偏差："+str[2]);
	}
    
    Type_A_uncertainty = Standard_Deviation / qSqrt(data.size());
    Type_C_uncertainty = qSqrt(pow(Type_A_uncertainty, 2) + pow(Type_B_uncertainty, 2));

    str[3] = ChangeType(Type_A_uncertainty);
    str[4] = ChangeType(Type_B_uncertainty);
    str[5] = ChangeType(Type_C_uncertainty);
    ui->calculateResult->append("(A類)隨機不確定度："+str[3]+"\n(B類)系統不確定度："+str[4]+"\n(C類)組合不確定度："+str[5]);

    str[6] = ChangeType((Type_C_uncertainty / abs(average))*100);//%
    ui->calculateResult->append("最終計算結果："+str[0]+" ± "+str[5]+"\n不確定度百分比："+str[0]+"(1 ± "+str[6]+"%)");

    data.clear();
}

void MainWindow::on_about_triggered()
{
    f1->resize(300,400);
    f1->setMinimumSize(264, 320);
    f1->setMaximumSize(720, 840);
    f1->setWindowTitle("關於Uncertainty Calculator");
    f1->setWindowIcon(QIcon(":/icon_256x256.ico"));
    f1->show();
}

void MainWindow::on_instructions_triggered()
{
    f2->resize(264,320);
    f2->setMinimumSize(264, 320);
    f2->setMaximumSize(720, 840);
    f2->setWindowTitle("使用說明");
    f2->setWindowIcon(QIcon(":/icon_256x256.ico"));
    f2->show();
}

void MainWindow::on_equalsButton_clicked()
{
    Calculator c;

    QString F;
    Uncertainty x,y,z;
    x.data = (ui->Xdata->text()).toDouble();
    y.data = (ui->Ydata->text()).toDouble();
    z.data = (ui->Zdata->text()).toDouble();

    x.uncertainty = ifpercentage(x.data, ui->Xuncertainty->text());
    y.uncertainty = ifpercentage(y.data, ui->Yuncertainty->text());
    z.uncertainty = ifpercentage(z.data, ui->Zuncertainty->text());
    F = ui->Formula->text();

    c.Input(x,y,z);
    QString result = c.getResult((F.toStdString()));
    ui->calculateResult_2->setText(result);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    digit1 = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    digit2 = arg1;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Index = index;
}

void MainWindow::on_Scientific_notation_stateChanged(int state)
{
    state1 = state;
}

void MainWindow::on_Scientific_notation_2_stateChanged(int state)
{
    state2 = state;
}

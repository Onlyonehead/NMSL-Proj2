#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::on_pushButton_displayCamera_clicked()
{
    QStringList msg;
    msg.append("displayCamera");
    sendMessage(msg);
}

void MainClient::on_pushButton_clearCamera_clicked()
{

    for(int i = 0; i < 5; i++){
        set0_array[i] = 0;
        set1_array[i] = 0;
        set2_array[i] = 0;
    }

    ui->label_traffic->setText("-");
    ui->label_redlight->setText("-");
    ui->label_overspeed->setText("-");

    ui->tableWidget_left->setRowCount(0);
    ui->tableWidget_right->setRowCount(0);
    this->left.clear();
    this->right.clear();

    QLineSeries *series=new QLineSeries;

    QChart *chart=new QChart;
    chart->setTitle("Traffic");
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%u");
    axisX->setGridLineVisible(true);
    axisX->setTickCount(10);
    axisX->setMinorTickCount(3);

    chart->setAxisX(axisX, series);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    QBarSet *set0 = new QBarSet("Traffic");
    QBarSet *set1 = new QBarSet("Redlight");
    QBarSet *set2 = new QBarSet("Overspeed");

    QPercentBarSeries *series2 = new QPercentBarSeries();
    series2->append(set0);
    series2->append(set1);
    series2->append(set2);

    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    chart2->setTitle("Offence Percentage");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "1" << "2" << "3" << "4" << "5";
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);

    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView_2->setChart(chart2);
    ui->chartView_2->setRenderHint(QPainter::Antialiasing);

}

void MainClient::on_button_right_clicked()
{
    if(this->left.trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }

    if(ui->tableWidget_right->rowCount() == 5){
        QMessageBox::warning(this,"警告", "\n达到上限！",QMessageBox::Close);
        return ;
    }

    int count = ui->tableWidget_right->rowCount();
    ui->tableWidget_right->insertRow(count);
    ui->tableWidget_right->setItem(count, 0, new QTableWidgetItem("Camera ID: " + this->left));
    ui->tableWidget_right->setRowCount(count + 1);

    ui->tableWidget_left->removeRow(ui->tableWidget_left->currentRow());

    QDate current_date = QDate::currentDate();

    QChart *chart;
    QChart *chart2;
    chart2 = new QChart;

    QLineSeries *series = new QLineSeries;
    QPercentBarSeries *series2 = new QPercentBarSeries();

    series->setName(this->left);

    if(ui->tableWidget_right->rowCount() == 1){
        chart = new QChart;
//        series->setPen(QPen(Qt::blue, 2, Qt::SolidLine));

    }else if(ui->tableWidget_right->rowCount() == 2){
        chart = ui->chartView->chart();
//        series->setPen(QPen(Qt::green, 2, Qt::SolidLine));

    }else if(ui->tableWidget_right->rowCount() == 3){
        chart = ui->chartView->chart();
//        series->setPen(QPen(Qt::yellow, 2, Qt::SolidLine));

    }else if(ui->tableWidget_right->rowCount() == 4){
        chart = ui->chartView->chart();
//        series->setPen(QPen(Qt::red, 2, Qt::SolidLine));

    }else{
        chart = ui->chartView->chart();
//        series->setPen(QPen(Qt::black, 2, Qt::SolidLine));

    }

    QBarSet *set0 = new QBarSet("Traffic");
    QBarSet *set1 = new QBarSet("Redlight");
    QBarSet *set2 = new QBarSet("Overspeed");

    QMap<int, int> map;
    for(QStringList list : this->traffic){
        if(list.at(0) == this->left){
            QString cam_date_str = list.at(1);
            QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            QDate cam_date;
            cam_date.setDate(year, month, day);
            int count;
            count = (int)cam_date.daysTo(current_date);
            if(count <= 10 && count >=0){
                map.insert(count, list.at(2).toInt());
            }
        }
    }

    QMap<int, int> map2;
    for(QStringList list : this->redlight){
        if(list.at(0) == this->left){
            QString cam_date_str = list.at(1);
            QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            QDate cam_date;
            cam_date.setDate(year, month, day);
            int count;
            count = (int)cam_date.daysTo(current_date);
            if(count <= 4 && count >=0){
                map2.insert(count, list.at(2).toInt());
            }
        }
    }

    QMap<int, int> map3;
    for(QStringList list : this->overspeed){
        if(list.at(0) == this->left){
            QString cam_date_str = list.at(1);
            QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            QDate cam_date;
            cam_date.setDate(year, month, day);
            int count;
            count = (int)cam_date.daysTo(current_date);
            if(count <= 4 && count >=0){
                map3.insert(count, list.at(2).toInt());
            }
        }
    }

    QMap<int, int>::const_iterator i;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    for (i = map.constBegin(); i != map.constEnd(); ++i) {
        *series << QPointF(i.key(), i.value());
        if(set0->count() < 5){
            set0_array[set0->count()] += i.value();
            count0 += set0_array[set0->count()];
            *set0 << set0_array[set0->count()];
        }
    }

    for (i = map2.constBegin(); i != map2.constEnd(); ++i) {
        set1_array[set1->count()] += i.value();
        count1 += set1_array[set1->count()];
        *set1 << set1_array[set1->count()];
    }

    for (i = map3.constBegin(); i != map3.constEnd(); ++i) {
        set2_array[set2->count()] += i.value();
        count2 += set2_array[set2->count()];
        *set2 << set2_array[set2->count()];
    }


    ui->label_traffic->setText(QString::number(count0));
    ui->label_redlight->setText(QString::number(count1));
    ui->label_overspeed->setText(QString::number(count2));

    series2->append(set0);
    series2->append(set1);
    series2->append(set2);

    chart->setTitle("Traffic");
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%u");
    axisX->setGridLineVisible(true);
    axisX->setTickCount(10);
    axisX->setMinorTickCount(3);

    chart->setAxisX(axisX, series);

    chart2->addSeries(series2);
    chart2->setTitle("Offence Percentage");
    chart2->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "1" << "2" << "3" << "4" << "5";
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);

    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartView_2->setChart(chart2);
    ui->chartView_2->setRenderHint(QPainter::Antialiasing);

    this->left.clear();
}

void MainClient::on_button_left_clicked()
{
    if(this->right.trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }

    int count = ui->tableWidget_left->rowCount();
    ui->tableWidget_left->insertRow(count);
    ui->tableWidget_left->setItem(count, 0, new QTableWidgetItem("Camera ID: " + this->right));
    ui->tableWidget_left->setRowCount(count + 1);

    ui->tableWidget_right->removeRow(ui->tableWidget_right->currentRow());


    QDate current_date = QDate::currentDate();

    QChart *chart = new QChart;

    QChart *chart2;
    chart2 = new QChart;

    QLineSeries *series;
    QPercentBarSeries *series2 = new QPercentBarSeries();


    if(ui->tableWidget_right->rowCount() == 0){

        for(int i = 0; i < 5; i++){
            set0_array[i] = 0;
            set1_array[i] = 0;
            set2_array[i] = 0;
        }

        ui->label_traffic->setText(QString::number(0));
        ui->label_redlight->setText(QString::number(0));
        ui->label_overspeed->setText(QString::number(0));

        series = new QLineSeries;

        QChart *chart=new QChart;
        chart->setTitle("Traffic");
        chart->addSeries(series);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->createDefaultAxes();
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QValueAxis *axisX = new QValueAxis;
        axisX->setRange(0, 10);
        axisX->setLabelFormat("%u");
        axisX->setGridLineVisible(true);
        axisX->setTickCount(10);
        axisX->setMinorTickCount(3);

        chart->setAxisX(axisX, series);

        ui->chartView->setChart(chart);
        ui->chartView->setRenderHint(QPainter::Antialiasing);

        QBarSet *set0 = new QBarSet("Traffic");
        QBarSet *set1 = new QBarSet("Redlight");
        QBarSet *set2 = new QBarSet("Overspeed");

        QPercentBarSeries *series2 = new QPercentBarSeries();
        series2->append(set0);
        series2->append(set1);
        series2->append(set2);

        QChart *chart2 = new QChart();
        chart2->addSeries(series2);
        chart2->setTitle("Offence Percentage");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QStringList categories;
        categories << "1" << "2" << "3" << "4" << "5";
        QBarCategoryAxis *axis2 = new QBarCategoryAxis();
        axis2->append(categories);
        chart2->createDefaultAxes();
        chart2->setAxisX(axis2, series2);

        chart2->legend()->setVisible(true);
        chart2->legend()->setAlignment(Qt::AlignBottom);

        ui->chartView_2->setChart(chart2);
        ui->chartView_2->setRenderHint(QPainter::Antialiasing);

        return;
    }

    QBarSet *set0 = new QBarSet("Traffic");
    QBarSet *set1 = new QBarSet("Redlight");
    QBarSet *set2 = new QBarSet("Overspeed");


    int count0 = 0;
    int count1 = 0;
    int count2 = 0;

    QMap<int, int> map;
    for(QStringList list : this->traffic){
        if(list.at(0) == this->right){
            QString cam_date_str = list.at(1);
            QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            QDate cam_date;
            cam_date.setDate(year, month, day);
            int count;
            count = (int)cam_date.daysTo(current_date);
            if(count <= 4 && count >=0){
                map.insert(count, list.at(2).toInt());
            }
        }
    }

    QMap<int, int> map2;
    for(QStringList list : this->redlight){
        if(list.at(0) == this->right){
            QString cam_date_str = list.at(1);
            QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            QDate cam_date;
            cam_date.setDate(year, month, day);
            int count;
            count = (int)cam_date.daysTo(current_date);
            if(count <= 4 && count >=0){
                map2.insert(count, list.at(2).toInt());
            }
        }
    }

    QMap<int, int> map3;
    for(QStringList list : this->overspeed){
        if(list.at(0) == this->right){
            QString cam_date_str = list.at(1);
            QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            QDate cam_date;
            cam_date.setDate(year, month, day);
            int count;
            count = (int)cam_date.daysTo(current_date);
            if(count <= 4 && count >=0){
                map3.insert(count, list.at(2).toInt());
            }
        }
    }

    QMap<int, int>::const_iterator i;
    for (i = map.constBegin(); i != map.constEnd(); ++i) {
        set0_array[set0->count()] -= i.value();
        count0 += set0_array[set0->count()];
        *set0 << set0_array[set0->count()];
    }

    for (i = map2.constBegin(); i != map2.constEnd(); ++i) {
        set1_array[set1->count()] -= i.value();
        count1 += set1_array[set1->count()];
        *set1 << set1_array[set1->count()];
    }

    for (i = map3.constBegin(); i != map3.constEnd(); ++i) {
        set2_array[set2->count()] -= i.value();
        count2 += set2_array[set2->count()];
        *set2 << set2_array[set2->count()];
    }


    ui->label_traffic->setText(QString::number(count0));
    ui->label_redlight->setText(QString::number(count1));
    ui->label_overspeed->setText(QString::number(count2));

    series2->append(set0);
    series2->append(set1);
    series2->append(set2);

    chart2->addSeries(series2);
    chart2->setTitle("Offence Percentage");
    chart2->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "1" << "2" << "3" << "4" << "5";
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);

    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView_2->setChart(chart2);
    ui->chartView_2->setRenderHint(QPainter::Antialiasing);




    for(int i = 0; i < ui->tableWidget_right->rowCount(); i++){

        series = new QLineSeries;
        series->setName(ui->tableWidget_right->item(i, 0)->text().split(":")[1].trimmed());

        QMap<int, int> map;
        for(QStringList list : this->traffic){
            if(list.at(0) == ui->tableWidget_right->item(i, 0)->text().split(":")[1].trimmed()){
                QString cam_date_str = list.at(1);
                QStringList s = cam_date_str.split(QRegExp("[-A-Z:]"));
                int year = s[0].toInt();
                int month = s[1].toInt();
                int day = s[2].toInt();
                QDate cam_date;
                cam_date.setDate(year, month, day);
                int count;
                count = (int)cam_date.daysTo(current_date);
                if(count <= 10){
                    map.insert(count, list.at(2).toInt());
                }
            }
        }

        QMap<int, int>::const_iterator it;
        for (it = map.constBegin(); it != map.constEnd(); ++it) {
            *series << QPointF(it.key(), it.value());
        }

        chart->addSeries(series);
    }

    chart->setTitle("Traffic");
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%u");
    axisX->setGridLineVisible(true);
    axisX->setTickCount(10);
    axisX->setMinorTickCount(3);
    chart->setAxisX(axisX, series);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);




    this->right.clear();
}

void MainClient::on_tableWidget_left_itemClicked(QTableWidgetItem *item){
    this->left = ui->tableWidget_left->item(item->row(), 0)
            ->text().split(":")[1].trimmed();
}

void MainClient::on_tableWidget_right_itemClicked(QTableWidgetItem *item){
    this->right = ui->tableWidget_right->item(item->row(), 0)
            ->text().split(":")[1].trimmed();
}

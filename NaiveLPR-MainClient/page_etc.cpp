#include "mainclient.h"
#include "ui_mainclient.h"



void MainClient::on_pushButton_diaplayETCvehicles_clicked()
{
    ui->label_vehicleWithArrearsTip->setText(QString::number(0));
    QStringList list;
    list.append("mc_ETCp_displayETCVehicles");
    sendMessage(list);
}


void MainClient::on_pushButton_diaplayETCvehicleonroad_clicked()
{
    ui->label_vehicleWithArrearsTip->setText(QString::number(0));
    QStringList list;
    list.append("mc_ETCp_displayOnRoadVehicles");
    sendMessage(list);

}




void MainClient::on_pushButton_diaplayETCPayHistory_clicked()
{
    ui->label_vehicleWithArrearsTip->setText(QString::number(0));
    QStringList list;
    list.append("mc_ETCp_displayPayHistory");
    sendMessage(list);

}



void MainClient::on_pushButton_rechargeBalance_clicked()
{

    QStringList list;
    QString plate = QString::number(ui->tableWidget_displayETCvehicleInfo->currentRow());
    QString recharge = ui->lineEdit_mc_ETCrechargeAmount->text();
    list.append("mc_recharge");
    list.append(plate);
    list.append(recharge);
    sendMessage(list);
}



void MainClient::on_pushButton_diaplayVehiclesWA_clicked()
{
    ui->label_vehicleWithArrearsTip->setText(QString::number(1));
    QStringList list;
    list.append("mc_displayVehiclesWA");
    sendMessage(list);
}




void MainClient::on_pushButton_Charge_clicked()
{
    if(ui->label_vehicleWithArrearsTip->text().toInt() == 0){
        QMessageBox::warning(this, tr("warning"), tr("\n必须先显示未缴费的车辆"), QMessageBox::Close, QMessageBox::Close);

    }

    QStringList list;
    list.append("mc_charge");


    sendMessage(list);

}


void MainClient::on_pushButton_ETCSearchAccount_clicked()
{
    QStringList list;
    if(ui->lineEdit_mc_ETCSearchAmount->text().isEmpty()){
        QMessageBox::warning(this, tr("warning"), tr("\n未输入搜索的车牌号"), QMessageBox::Close, QMessageBox::Close);
    }
   QString searchPlate = ui->lineEdit_mc_ETCSearchAmount->text();
   list.append("mc_ETCp_searchPlate");
   list.append(searchPlate);
   sendMessage(list);
}


void MainClient::on_pushButton_ETCplateDelete_clicked()
{
   if(QMessageBox::Yes==QMessageBox::question(this, "警告", "Confirm to delete this account ?",
                                              QMessageBox::Yes, QMessageBox::No)){
       int selectedRow = ui->tableWidget_displayETCvehicleInfo->currentRow();
       QString deletePlate = ui->tableWidget_displayETCvehicleInfo->item(selectedRow, 0)->text();
       QStringList list;
       list.append("mc_DeletePLate");
       list.append(deletePlate);
       sendMessage(list);
       ui->tableWidget_displayETCvehicleInfo->removeRow(selectedRow);
   }

}




void MainClient::on_pushButton_showmap_clicked()
{
    QtMap *qtMap = new QtMap("http://kousz.top/repo/project2/src/camera.html");
    connect(this, SIGNAL(webReturn(QVector<QStringList>)),
            qtMap, SLOT(webInit(QVector<QStringList>)));
    emit webReturn(this->cameras);
    qtMap->exec();
}


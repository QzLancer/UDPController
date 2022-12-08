#pragma once

#include <QtWidgets/QWidget>
#include "ui_SendAllFrame.h"
#include "CUDPService.h"

class SendAllFrame : public QWidget
{
    Q_OBJECT

public:
    SendAllFrame(QWidget *parent = nullptr);
    ~SendAllFrame();

public slots:
    void slotConnet();
    void slotConnet2();

private:
    Ui::SendAllFrameClass ui;
    CUDPService m_udpservice;
};

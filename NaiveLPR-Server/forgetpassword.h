#ifndef FORGETPASSWORD_H
#define FORGETPASSWORD_H

#include "TCPConnection.h"
#include "../packages/SmtpClient-for-Qt-1.1/src/SmtpMime"

class ForgetPassword
{
public:
    ForgetPassword();
    static int sendCode(QString username, QString email, QString securityCode);
    static int checkCode(QString username, QString pw1, QString pw2, QString code, QString securityCode);
};

#endif // FORGETPASSWORD_H

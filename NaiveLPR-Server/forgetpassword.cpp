#include "forgetpassword.h"

/**
 * send security code
 *
 * @author hzc
 * @param username username
 * @param email email
 */
int ForgetPassword::sendCode(QString username, QString email, QString securityCode){
    // First we need to create an SmtpClient object
    // We will use the Gmail's smtp server (smtp.gmail.com, port 465, ssl)

    if(email == "" || username == ""){
        return -1;
    }

    QSqlQuery query;
    SQLTool::search(query, "userdata", "username", username);
    if(query.next()){
        if(email != query.value(5).toString()){
            return -2;
        }
    }else{

        return -3;
    }

    SmtpClient smtp("smtp.163.com", 25);

    // We need to set the username (your email address) and password
    // for smtp authentification.

    smtp.setUser("15840084095@163.com");
    smtp.setPassword("1234abcd");

    // Now we create a MimeMessage object. This is the email.

    MimeMessage message;

    EmailAddress sender("15840084095@163.com", "Support");
    message.setSender(&sender);

    EmailAddress to(email, username);
    message.addRecipient(&to);

    message.setSubject("修改您的密码");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText("Hi, " + username + "\n\nYour security code is: " +
                 securityCode + ".\n");

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail

    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
        return -4;
    }

    if (!smtp.login()) {
        qDebug() << "Failed to login!" << endl;
        return -4;
    }

    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;
        return -4;
    }

    qDebug() << "Sent" << endl;
    return 0;
    smtp.quit();
}


/**
 * check code and update password
 *
 * @author hzc
 * @param username username
 * @param pw1 first entered password
 * @param pw2 second entered password
 * @param code security code entered by user
 * @param securityCode true security code
 */
int ForgetPassword::checkCode(QString username, QString pw1, QString pw2, QString code, QString securityCode){
    if(pw1 == "" || pw2 == ""){
        return -1;
    }

    if(code != securityCode){
        return -2;
    }

    if(pw1 != pw2){

        return -3;
    }

    SQLTool::update("userdata", "password", pw1, "username", username);

    QSqlQuery query;
    SQLTool::search(query, "blacklist", "username", username);
    if(query.next()){
        SQLTool::del("blacklist", "username", username);
    }
    return 0;
}

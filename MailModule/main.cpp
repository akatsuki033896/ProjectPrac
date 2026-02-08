#include "smtp_mail_sender.h"

int main() {
    SmtpMailSender::EmailInfo email_info;
    email_info.smtp_url = "smtp.qq.com:587";
    email_info.username = "2596085314@qq.com";
    email_info.password = "btsewfjkqyoheceb";
    email_info.from = "2596085314@qq.com";
    email_info.to = "2596085314@qq.com";
    email_info.subject = "Test Email";
    email_info.body = "<h1>This is a test email sent from C++.</h1>";
    email_info.is_html = true;

    SmtpMailSender::send_mail(email_info);
    return 0;
}
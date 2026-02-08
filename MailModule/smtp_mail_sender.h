#pragma once
#include <string>

namespace SmtpMailSender {
    struct EmailInfo {
        std::string smtp_url; // smtp服务器地址和端口
        std::string username;
        std::string password;
        std::string from;
        std::string to;
        std::string subject;
        std::string body;
        bool is_html = false; // 是否为html邮件
    };
    bool send_mail(EmailInfo& email_info); // 发送邮件
};
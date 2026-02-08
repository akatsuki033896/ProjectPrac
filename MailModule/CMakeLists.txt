cmake_minimum_required(VERSION 3.12)
project(MailModule)
add_executable(MailModule main.cpp smtp_mail_sender.cpp)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    CURL::libcurl
)
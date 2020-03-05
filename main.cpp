#include <QCoreApplication>

#include <iostream>
#include <string>
extern "C"
{
#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
}

#include<QDebug>
#include<_timeval.h>
#include<QThread>

using namespace std;

int main(int argc, char *argv[])
{
    qDebug()<<"in";
        QCoreApplication a(argc, argv);

        string RMQ_IP=argv[1];

        QString STR_RMQ_PORT=argv[2];

        string USER_NAME=argv[3];

        string PASSWORD=argv[4];

        int RMQ_PORT=STR_RMQ_PORT.toInt();


        string hostName =RMQ_IP;

//        string hostName ="127.0.0.1";
        int port = RMQ_PORT;
        amqp_socket_t *socket = nullptr;
        amqp_connection_state_t conn;
        conn = amqp_new_connection();
        socket = amqp_tcp_socket_new(conn);
        if(!socket){
            cout << "create socket failed!";
            exit(1);
        }
        if(amqp_socket_open(socket, hostName.c_str(), port)){

            cout << "opening TCP socket failed" << endl;
            exit(1);
        }
        //登录
        if(1 != amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, USER_NAME.c_str(), PASSWORD.c_str()).reply_type){
            cout << "login failed" << endl;
            exit(1);
        }
            amqp_channel_open(conn, 1);

            amqp_basic_consume_ok_t *msg = amqp_basic_consume(conn, 1, amqp_cstring_bytes("quetest(lv)"), amqp_cstring_bytes("quetest(lv)"), 0, 1, 0, amqp_empty_table);
            int count=0;
            while (1) {
                count++;
                amqp_envelope_t envelope;
                amqp_maybe_release_buffers(conn);
                timeval test;
                test.tv_sec = 1;
                test.tv_usec = 0;
                amqp_rpc_reply_t ret = amqp_consume_message(conn, &envelope, NULL, 1);
                qDebug()<< (char *)envelope.message.body.bytes<<count;
//                QThread::sleep(1);

            }

//-------------------------------------
#if 0
            amqp_connection_state_t connState = amqp_new_connection();
             amqp_socket_t *pSocket = amqp_tcp_socket_new(connState);
             if (!pSocket) {
              amqp_connection_close(connState, AMQP_REPLY_SUCCESS);
              amqp_destroy_connection(connState);
              std::cout << "跟消息服务器创建连接失败" << std::endl;
//              return;
             }
             int nConnStatus = amqp_socket_open(socket, hostName.c_str(), port);
             if (AMQP_STATUS_OK != nConnStatus) {
              amqp_connection_close(connState, AMQP_REPLY_SUCCESS);
              amqp_destroy_connection(connState);
//              return;
             }

             amqp_rpc_reply_t  rpcReply = amqp_login(connState, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
             if (AMQP_RESPONSE_NORMAL != rpcReply.reply_type)
             {
              std::cout << "登陆消息服务器失败" << std::endl;
//              return;
             }

             amqp_channel_open(connState, 1);
             amqp_basic_consume(connState, 1, amqp_cstring_bytes("passerby-000001"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

             amqp_frame_t frame;
             std::cout << "登陆消息服务器成功，开始接收数据" << std::endl;

             while (1)
             {
              amqp_envelope_t envelope;

              amqp_maybe_release_buffers(connState);
              timeval tvTimeout;
              tvTimeout.tv_sec = 10;
              tvTimeout.tv_usec = 0;
              amqp_rpc_reply_t ret = amqp_consume_message(connState, &envelope, &tvTimeout, 0);

              if (AMQP_RESPONSE_NORMAL != ret.reply_type)
              {
               if (AMQP_STATUS_SOCKET_ERROR == ret.library_error)
               {
                std::cout << "跟消息服务器连接中断，清理资源，重连连接" << std::endl;
                break;
               }
               if (AMQP_STATUS_TIMEOUT == ret.library_error)
               {
                std::cout << "等待消息服务器消息超时，继续等待" << std::endl;
                continue;
               }
               std::cout << "跟消息服务器连接出现异常，清理资源，重连连接" << std::endl;
               break;
              }
              else
              {
               std::string strRecvMsg((char*)envelope.message.body.bytes, envelope.message.body.len);
               std::cout << "接收到的抓拍信息:" << strRecvMsg<< std::endl;
               amqp_destroy_envelope(&envelope);
               continue;
              }
             }
             amqp_channel_close(connState, 1, AMQP_REPLY_SUCCESS);
             amqp_connection_close(connState, AMQP_REPLY_SUCCESS);
             amqp_destroy_connection(connState);
 #endif
//------------------------------------------
//        getchar();

    return a.exec();
}

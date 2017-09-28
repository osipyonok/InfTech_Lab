#ifndef MESSAGESTACK_H
#define MESSAGESTACK_H
#include<bits/stdc++.h>
#include<QString>
#include<QDebug>
#include<QStack>

using namespace std;

enum MessageType {Information , Warning , Error};

namespace Messages{
    extern QStack<pair<MessageType , QString>> stack;
}

class MessageStack
{
public:
    void addMessage(MessageType , QString);
private:
 //   MessageStack();
 //   static QStack<pair<MessageType , QString>> stack;
};

#endif // MESSAGESTACK_H

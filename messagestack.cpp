#include "messagestack.h"
#include<bits/stdc++.h>
#include<QString>
#include<QDebug>
#include<QStack>

using namespace std;

namespace Messages{
    QStack<pair<MessageType , QString>> stack;
}

//MessageStack::MessageStack(){}

//void MessageStack::addMessage(MessageType type, QString message){
 //   MessageStack::stack.push({type , message});
//    qDebug() << message << endl;
//}

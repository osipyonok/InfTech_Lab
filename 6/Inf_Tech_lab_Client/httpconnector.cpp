#include "httpconnector.h"

HttpConnector::HttpConnector()
{
    srand(time(0));
    for(int i = 0 ; i < 10 ; ++i)
        descriptor += (char)('a' + (rand() % 26));
    pManager = new QNetworkAccessManager(this);
    connect(pManager , SIGNAL(finished(QNetworkReply*)), this , SLOT(replyFinish(QNetworkReply*)));
    //pManager->moveToThread(this);
    //pManager->post( QNetworkRequest(QUrl("http://api.forismatic.com/api/1.0/")), QString("method=getQuote&format=xml").toUtf8());

}


void HttpConnector::run(){
    QUrl serviceUrl = QUrl("http://dry-beach-27886.herokuapp.com/put");
    QByteArray postData;
    postData.append("author=Client_2&");
    postData.append("query={\"command\": \"getDataBase3s\"}");
    pManager->post(QNetworkRequest(serviceUrl), postData);
    while (1) {

    }
}

void HttpConnector::replyFinish(QNetworkReply* reply){
    qDebug() << "hiii\n";
    QString answer = QString::fromUtf8(reply->readAll());
    qDebug() << answer << "\n";
}

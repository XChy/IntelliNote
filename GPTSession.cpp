#include "GPTSession.h"

GPTSession::GPTSession(QObject *parent)
    : QObject{parent}, APIKey(default_api_key){}

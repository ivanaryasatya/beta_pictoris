#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

class FirebaseHandler {

private:

    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;

    String email;
    String password;
    bool ready = false;

    bool retry(std::function<bool()> func, uint8_t attempts = 3) {

        for(uint8_t i=0;i<attempts;i++){
            if(func()) return true;
            delay(200);
        }

        return false;
    }

public:

    void begin(
        const char* apiKey,
        const char* dbURL,
        const char* userEmail,
        const char* userPassword
    ){

        config.api_key = apiKey;
        config.database_url = dbURL;

        email = userEmail;
        password = userPassword;

        auth.user.email = email;
        auth.user.password = password;

        Firebase.begin(&config,&auth);
        Firebase.reconnectWiFi(true);

        ready = true;
    }

    bool connected(){
        return Firebase.ready();
    }

    String error(){
        return fbdo.errorReason();
    }

    /* ========================
        SEND DATA
    ======================== */

    bool sendInt(String path, int value){
        return retry([&](){
            return Firebase.RTDB.setInt(&fbdo, path, value);
        });
    }

    bool sendFloat(String path, float value){
        return retry([&](){
            return Firebase.RTDB.setFloat(&fbdo, path, value);
        });
    }

    bool sendBool(String path, bool value){
        return retry([&](){
            return Firebase.RTDB.setBool(&fbdo, path, value);
        });
    }

    bool sendString(String path, String value){
        return retry([&](){
            return Firebase.RTDB.setString(&fbdo, path, value);
        });
    }

    bool sendJSON(String path, FirebaseJson &json){
        return retry([&](){
            return Firebase.RTDB.setJSON(&fbdo, path, &json);
        });
    }

    /* ========================
        GET DATA
    ======================== */

    int getInt(String path){

        if(retry([&](){
            return Firebase.RTDB.getInt(&fbdo,path);
        })){
            return fbdo.intData();
        }

        return 0;
    }

    float getFloat(String path){

        if(retry([&](){
            return Firebase.RTDB.getFloat(&fbdo,path);
        })){
            return fbdo.floatData();
        }

        return 0;
    }

    bool getBool(String path){

        if(retry([&](){
            return Firebase.RTDB.getBool(&fbdo,path);
        })){
            return fbdo.boolData();
        }

        return false;
    }

    String getString(String path){

        if(retry([&](){
            return Firebase.RTDB.getString(&fbdo,path);
        })){
            return fbdo.stringData();
        }

        return "";
    }

};